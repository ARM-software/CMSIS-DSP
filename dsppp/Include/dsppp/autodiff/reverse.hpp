// -*- C++ -*-
/** @file
 * @brief Allocation-free reverse-mode AD core and operator registry.
 */
#pragma once

#include <cstddef>
#include <cstdint>
#include <new>
#include <type_traits>

namespace arm_cmsis_dsp {
namespace autodiff {

#ifndef DSPPP_AUTODIFF_MAX_OPERATORS
#define DSPPP_AUTODIFF_MAX_OPERATORS 16
#endif

class Tape;
class OperatorAccess;

namespace detail {

struct Node;
using BackwardFunction = void (*)(Node &) noexcept;
using ResetFunction = void (*)(Node &) noexcept;

struct Node
{
    Node *previous;
    BackwardFunction backward;
    ResetFunction reset_gradient;
};

template <typename Operator>
inline constexpr unsigned char operator_token = 0U;

} // namespace detail

enum class Status
{
    ok,
    out_of_memory,
    tape_mismatch,
    invalid_output,
    operator_not_registered,
    operator_registry_full
};

enum class BufferRole
{
    input,
    parameter,
    intermediate
};

/** Non-owning view of caller values and an associated gradient buffer. */
class BufferView
{
public:
    BufferView() noexcept
        : values_(nullptr), gradients_(nullptr), length_(0U), tape_(nullptr),
          producer_(nullptr), role_(BufferRole::input)
    {
    }

    float *values() noexcept { return values_; }
    const float *values() const noexcept { return values_; }
    float *gradients() noexcept { return gradients_; }
    const float *gradients() const noexcept { return gradients_; }
    std::size_t length() const noexcept { return length_; }
    BufferRole role() const noexcept { return role_; }
    bool has_gradient() const noexcept { return gradients_ != nullptr; }

    float &operator[](std::size_t index) noexcept { return values_[index]; }
    const float &operator[](std::size_t index) const noexcept
    {
        return values_[index];
    }
    float gradient(std::size_t index) const noexcept
    {
        return gradients_ == nullptr ? 0.0F : gradients_[index];
    }

    /** Evaluate any expression supplied by a separately included operator. */
    template <typename Expression>
    BufferView &operator=(const Expression &expression) noexcept
    {
        expression.evaluate(*this);
        return *this;
    }

private:
    BufferView(float *values, float *gradients, std::size_t length,
               Tape *tape, BufferRole role) noexcept
        : values_(values), gradients_(gradients), length_(length), tape_(tape),
          producer_(nullptr), role_(role)
    {
    }

    float *values_;
    float *gradients_;
    std::size_t length_;
    Tape *tape_;
    detail::Node *producer_;
    BufferRole role_;

    friend class Tape;
    friend class OperatorAccess;
};

/** Non-owning row-major matrix parameter view. */
class MatrixView
{
public:
    MatrixView() noexcept : buffer_(), rows_(0U), columns_(0U) {}

    std::size_t rows() const noexcept { return rows_; }
    std::size_t columns() const noexcept { return columns_; }
    float *values() noexcept { return buffer_.values(); }
    const float *values() const noexcept { return buffer_.values(); }
    float *gradients() noexcept { return buffer_.gradients(); }
    const float *gradients() const noexcept { return buffer_.gradients(); }
    std::size_t length() const noexcept { return rows_ * columns_; }
    float &operator()(std::size_t row, std::size_t column) noexcept
    {
        return buffer_.values()[row * columns_ + column];
    }
    const float &operator()(std::size_t row,
                            std::size_t column) const noexcept
    {
        return buffer_.values()[row * columns_ + column];
    }
    float gradient(std::size_t row, std::size_t column) const noexcept
    {
        return buffer_.gradient(row * columns_ + column);
    }

private:
    MatrixView(const BufferView &buffer, std::size_t rows,
               std::size_t columns) noexcept
        : buffer_(buffer), rows_(rows), columns_(columns)
    {
    }

    BufferView buffer_;
    std::size_t rows_;
    std::size_t columns_;

    friend class Tape;
    friend class OperatorAccess;
};

/** Reverse-mode tape using caller-supplied storage and a fixed operator list. */
class Tape
{
public:
    static constexpr std::size_t maximum_registered_operators =
        DSPPP_AUTODIFF_MAX_OPERATORS;
    static_assert(maximum_registered_operators > 0U,
                  "The operator registry must contain at least one slot");
    static_assert((maximum_registered_operators &
                   (maximum_registered_operators - 1U)) == 0U,
                  "The operator registry capacity must be a power of two");

    Tape(void *storage, std::size_t bytes) noexcept
        : storage_(static_cast<unsigned char *>(storage)), capacity_(bytes),
          used_(0U), tail_(nullptr), recording_(true), status_(Status::ok),
          graph_begin_(0U), graph_marked_(false), registered_count_(0U),
          registered_operators_{}
    {
    }

    Tape(const Tape &) = delete;
    Tape &operator=(const Tape &) = delete;

    /** Register an operator type once. Registration uses no arena storage. */
    template <typename Operator>
    bool register_operator() noexcept
    {
        const void *token = &detail::operator_token<Operator>;
        const std::size_t first = operator_hash(token);
        for (std::size_t probe = 0U;
             probe < maximum_registered_operators; ++probe)
        {
            const std::size_t slot =
                (first + probe) & (maximum_registered_operators - 1U);
            if (registered_operators_[slot] == token)
            {
                return true;
            }
            if (registered_operators_[slot] == nullptr)
            {
                registered_operators_[slot] = token;
                ++registered_count_;
                return true;
            }
        }
        set_error(Status::operator_registry_full);
        return false;
    }

    template <typename Operator>
    bool is_operator_registered() const noexcept
    {
        const void *token = &detail::operator_token<Operator>;
        const std::size_t first = operator_hash(token);
        for (std::size_t probe = 0U;
             probe < maximum_registered_operators; ++probe)
        {
            const std::size_t slot =
                (first + probe) & (maximum_registered_operators - 1U);
            if (registered_operators_[slot] == token)
            {
                return true;
            }
            if (registered_operators_[slot] == nullptr)
            {
                return false;
            }
        }
        return false;
    }

    /** Release gradients and records. Operator registrations are preserved. */
    void reset() noexcept
    {
        used_ = 0U;
        tail_ = nullptr;
        recording_ = true;
        status_ = Status::ok;
        graph_begin_ = 0U;
        graph_marked_ = false;
    }

    /** Mark all current arena allocations as persistent training state. */
    void begin_graph() noexcept
    {
        graph_begin_ = used_;
        graph_marked_ = true;
        tail_ = nullptr;
        recording_ = true;
        status_ = Status::ok;
    }

    /** Reclaim only operation records, preserving views and their gradients. */
    bool rewind_graph() noexcept
    {
        if (!graph_marked_)
        {
            set_error(Status::invalid_output);
            return false;
        }
        used_ = graph_begin_;
        tail_ = nullptr;
        recording_ = true;
        status_ = Status::ok;
        return true;
    }

    std::size_t used() const noexcept { return used_; }
    std::size_t capacity() const noexcept { return capacity_; }
    Status status() const noexcept { return status_; }
    bool good() const noexcept { return status_ == Status::ok; }
    void clear_status() noexcept { status_ = Status::ok; }
    bool recording() const noexcept { return recording_; }
    void set_recording(bool enabled) noexcept { recording_ = enabled; }

    /** Generic active view; output() is clearer for application code. */
    BufferView view(float *values, std::size_t length) noexcept
    {
        if (length != 0U && values == nullptr)
        {
            set_error(Status::tape_mismatch);
            return BufferView(values, nullptr, length, this,
                              BufferRole::intermediate);
        }
        if (length > static_cast<std::size_t>(-1) / sizeof(float))
        {
            set_error(Status::out_of_memory);
            return BufferView(values, nullptr, length, this,
                              BufferRole::intermediate);
        }

        float *gradients = nullptr;
        if (length != 0U)
        {
            gradients = static_cast<float *>(
                allocate(length * sizeof(float), alignof(float)));
            if (gradients != nullptr)
            {
                for (std::size_t i = 0; i < length; ++i)
                {
                    gradients[i] = 0.0F;
                }
            }
        }
        return BufferView(values, gradients, length, this,
                          BufferRole::intermediate);
    }

    template <std::size_t Length>
    BufferView view(float (&values)[Length]) noexcept
    {
        return view(values, Length);
    }

    BufferView view(float *values, float *gradients,
                    std::size_t length) noexcept
    {
        if (length != 0U && (values == nullptr || gradients == nullptr))
        {
            set_error(Status::tape_mismatch);
        }
        return BufferView(values, gradients, length, this,
                          BufferRole::intermediate);
    }

    template <std::size_t Length>
    BufferView view(float (&values)[Length],
                    float (&gradients)[Length]) noexcept
    {
        return view(values, gradients, Length);
    }

    BufferView input(float *values, std::size_t length) noexcept
    {
        if (length != 0U && values == nullptr)
        {
            set_error(Status::tape_mismatch);
        }
        return BufferView(values, nullptr, length, this, BufferRole::input);
    }

    template <std::size_t Length>
    BufferView input(float (&values)[Length]) noexcept
    {
        return input(values, Length);
    }

    BufferView input(float &value) noexcept { return input(&value, 1U); }

    BufferView parameter(float *values, std::size_t length) noexcept
    {
        BufferView result = view(values, length);
        result.role_ = BufferRole::parameter;
        return result;
    }

    template <std::size_t Length>
    BufferView parameter(float (&values)[Length]) noexcept
    {
        return parameter(values, Length);
    }

    BufferView parameter(float &value) noexcept
    {
        return parameter(&value, 1U);
    }

    BufferView parameter(float *values, float *gradients,
                         std::size_t length) noexcept
    {
        BufferView result = view(values, gradients, length);
        result.role_ = BufferRole::parameter;
        return result;
    }

    template <std::size_t Length>
    BufferView parameter(float (&values)[Length],
                         float (&gradients)[Length]) noexcept
    {
        return parameter(values, gradients, Length);
    }

    BufferView parameter(float &value, float &gradient) noexcept
    {
        return parameter(&value, &gradient, 1U);
    }

    MatrixView parameter(float *values, std::size_t rows,
                         std::size_t columns) noexcept
    {
        if (columns != 0U && rows > static_cast<std::size_t>(-1) / columns)
        {
            set_error(Status::out_of_memory);
            return MatrixView();
        }
        return MatrixView(parameter(values, rows * columns), rows, columns);
    }

    template <std::size_t Rows, std::size_t Columns>
    MatrixView parameter(float (&values)[Rows][Columns]) noexcept
    {
        return parameter(&values[0][0], Rows, Columns);
    }

    MatrixView parameter(float *values, float *gradients, std::size_t rows,
                         std::size_t columns) noexcept
    {
        if (columns != 0U && rows > static_cast<std::size_t>(-1) / columns)
        {
            set_error(Status::out_of_memory);
            return MatrixView();
        }
        return MatrixView(parameter(values, gradients, rows * columns), rows,
                          columns);
    }

    template <std::size_t Rows, std::size_t Columns>
    MatrixView parameter(float (&values)[Rows][Columns],
                         float (&gradients)[Rows][Columns]) noexcept
    {
        return parameter(&values[0][0], &gradients[0][0], Rows, Columns);
    }

    BufferView output(float *values, std::size_t length) noexcept
    {
        return view(values, length);
    }

    template <std::size_t Length>
    BufferView output(float (&values)[Length]) noexcept
    {
        return output(values, Length);
    }

    BufferView output(float &value) noexcept { return output(&value, 1U); }

    BufferView output(float *values, float *gradients,
                      std::size_t length) noexcept
    {
        return view(values, gradients, length);
    }

    template <std::size_t Length>
    BufferView output(float (&values)[Length],
                      float (&gradients)[Length]) noexcept
    {
        return output(values, gradients, Length);
    }

    BufferView output(float &value, float &gradient) noexcept
    {
        return output(&value, &gradient, 1U);
    }

    bool backward(const BufferView &output, float seed = 1.0F) noexcept
    {
        if (output.length_ != 1U)
        {
            set_error(Status::invalid_output);
            return false;
        }
        return backward(output, &seed, 1U);
    }

    bool backward(const BufferView &output, const float *seed,
                  std::size_t seed_length) noexcept
    {
        if (status_ != Status::ok)
        {
            return false;
        }
        if (!valid(output) || output.producer_ == nullptr || seed == nullptr ||
            seed_length != output.length_)
        {
            set_error(Status::invalid_output);
            return false;
        }

        for (detail::Node *node = output.producer_; node != nullptr;
             node = node->previous)
        {
            node->reset_gradient(*node);
        }
        for (std::size_t i = 0; i < output.length_; ++i)
        {
            output.gradients_[i] = seed[i];
        }
        for (detail::Node *node = output.producer_; node != nullptr;
             node = node->previous)
        {
            node->backward(*node);
        }
        return true;
    }

private:
    static std::size_t operator_hash(const void *token) noexcept
    {
        std::uintptr_t value = reinterpret_cast<std::uintptr_t>(token);
        value ^= value >> 4U;
        value *= static_cast<std::uintptr_t>(0x9E3779B1U);
        value ^= value >> (sizeof(std::uintptr_t) * 4U);
        return static_cast<std::size_t>(value) &
               (maximum_registered_operators - 1U);
    }

    bool valid(const BufferView &view) const noexcept
    {
        return view.tape_ == this &&
               (view.length_ == 0U ||
                (view.values_ != nullptr &&
                 (view.role_ == BufferRole::input ||
                  view.gradients_ != nullptr)));
    }

    void set_error(Status error) noexcept
    {
        if (status_ == Status::ok)
        {
            status_ = error;
        }
    }

    void *allocate(std::size_t bytes, std::size_t alignment) noexcept
    {
        if (storage_ == nullptr || alignment == 0U)
        {
            set_error(Status::out_of_memory);
            return nullptr;
        }
        const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(storage_);
        const std::uintptr_t current = base + used_;
        const std::size_t padding = static_cast<std::size_t>(
            (alignment - (current % alignment)) % alignment);
        if (used_ > capacity_)
        {
            set_error(Status::out_of_memory);
            return nullptr;
        }
        const std::size_t remaining = capacity_ - used_;
        if (padding > remaining || bytes > remaining - padding)
        {
            set_error(Status::out_of_memory);
            return nullptr;
        }
        used_ += padding;
        void *result = storage_ + used_;
        used_ += bytes;
        return result;
    }

    template <typename Record>
    Record *append(detail::BackwardFunction backward,
                   detail::ResetFunction reset_gradient) noexcept
    {
        static_assert(std::is_trivially_destructible<Record>::value,
                      "Tape records are discarded without destructors");
        void *memory = allocate(sizeof(Record), alignof(Record));
        if (memory == nullptr)
        {
            return nullptr;
        }
        Record *record = ::new (memory) Record();
        record->node.previous = tail_;
        record->node.backward = backward;
        record->node.reset_gradient = reset_gradient;
        tail_ = &record->node;
        return record;
    }

    unsigned char *storage_;
    std::size_t capacity_;
    std::size_t used_;
    detail::Node *tail_;
    bool recording_;
    Status status_;
    std::size_t graph_begin_;
    bool graph_marked_;
    std::size_t registered_count_;
    const void *registered_operators_[maximum_registered_operators];

    friend class OperatorAccess;
};

/** Narrow internal interface used by independently defined operators. */
class OperatorAccess
{
public:
    static Tape *tape(const BufferView &view) noexcept { return view.tape_; }
    static float *values(BufferView &view) noexcept { return view.values_; }
    static const float *values(const BufferView &view) noexcept
    {
        return view.values_;
    }
    static float *gradients(const BufferView &view) noexcept
    {
        return view.gradients_;
    }
    static std::size_t length(const BufferView &view) noexcept
    {
        return view.length_;
    }
    static BufferRole role(const BufferView &view) noexcept { return view.role_; }
    static detail::Node *producer(const BufferView &view) noexcept
    {
        return view.producer_;
    }
    static void set_producer(BufferView &view, detail::Node *node) noexcept
    {
        view.producer_ = node;
    }
    static bool valid(const Tape &tape, const BufferView &view) noexcept
    {
        return tape.valid(view);
    }
    static bool compatible(const Tape &tape, const BufferView &left,
                           const BufferView &right) noexcept
    {
        return valid(tape, left) && valid(tape, right) &&
               length(left) == length(right);
    }
    static const BufferView &buffer(const MatrixView &matrix) noexcept
    {
        return matrix.buffer_;
    }
    static std::size_t rows(const MatrixView &matrix) noexcept
    {
        return matrix.rows_;
    }
    static std::size_t columns(const MatrixView &matrix) noexcept
    {
        return matrix.columns_;
    }
    static bool recording(const Tape &tape) noexcept { return tape.recording_; }
    static Status status(const Tape &tape) noexcept { return tape.status_; }
    static void fail(Tape &tape, Status status) noexcept { tape.set_error(status); }

    template <typename Operator>
    static bool require(Tape &tape) noexcept
    {
        if (!tape.is_operator_registered<Operator>())
        {
            tape.set_error(Status::operator_not_registered);
            return false;
        }
        return true;
    }

    template <typename Record>
    static Record *append(Tape &tape, detail::BackwardFunction backward,
                          detail::ResetFunction reset_gradient) noexcept
    {
        return tape.append<Record>(backward, reset_gradient);
    }
};

class RecordingScope
{
public:
    RecordingScope(Tape &tape, bool enabled) noexcept
        : tape_(tape), previous_(tape.recording())
    {
        tape_.set_recording(enabled);
    }
    ~RecordingScope() noexcept { tape_.set_recording(previous_); }
    RecordingScope(const RecordingScope &) = delete;
    RecordingScope &operator=(const RecordingScope &) = delete;

private:
    Tape &tape_;
    bool previous_;
};

template <std::size_t Bytes>
class Arena
{
public:
    static_assert(Bytes > 0U, "An autodiff arena must contain storage");
    Arena() noexcept : storage_{}, tape_(storage_, Bytes) {}
    Arena(const Arena &) = delete;
    Arena &operator=(const Arena &) = delete;
    Tape &tape() noexcept { return tape_; }
    const Tape &tape() const noexcept { return tape_; }

private:
    alignas(std::max_align_t) unsigned char storage_[Bytes];
    Tape tape_;
};

} // namespace autodiff
} // namespace arm_cmsis_dsp
