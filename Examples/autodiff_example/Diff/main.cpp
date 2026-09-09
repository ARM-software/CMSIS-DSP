/*
 * Copyright 2026 Arm Limited and/or its affiliates.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "RTE_Components.h"
#include CMSIS_device_header

#include <dsppp/autodiff/reverse.hpp>
#include <dsppp/autodiff/operators/add.hpp>

#include <cstddef>
#include <cstdio>
#include <cstdlib>

using namespace arm_cmsis_dsp::autodiff;

namespace {

constexpr std::size_t length = 4U;

void print_vector(const char *label, const float *values)
{
    std::printf("%s = (%g, %g, %g, %g)\n", label,
                static_cast<double>(values[0]),
                static_cast<double>(values[1]),
                static_cast<double>(values[2]),
                static_cast<double>(values[3]));
}

void check_tape(const Tape<float> &tape)
{
    if (!tape.good())
    {
        std::printf("Autodiff failed (status=%u)\n",
                    static_cast<unsigned>(tape.status()));
        std::exit(EXIT_FAILURE);
    }
}

} // namespace

int main()
{
    Arena<2048, float> arena;
    Tape<float> &tape = arena.tape();
    tape.register_operator<AddOperator<float>>();

    float x_value[length] = {1.0F, 2.0F, 3.0F, 4.0F};
    float a_value[length] = {10.0F, 20.0F, 30.0F, 40.0F};
    float y_value[length] = {};

    // Inputs hold fixed data; parameters receive gradients in the backward pass.
    BufferView<float> x = tape.input(x_value);
    BufferView<float> a = tape.parameter(a_value);
    BufferView<float> y = tape.output(y_value);
    check_tape(tape);

    std::printf("Step 1: Register input x, parameters a, and output y.\n");
    print_vector("x", x.values());
    print_vector("a", a.values());
    print_vector("y (before evaluation)", y.values());

    std::printf("\nStep 2: Forward evaluation of y = a + x.\n");
    y = a + x;
    check_tape(tape);
    print_vector("y", y.values());

    std::printf("\nStep 3: Differentiate each y[i] with respect to a.\n");
    // A vector output has a Jacobian. Each backward pass computes J^T * seed.
    // Seed just output i to obtain row i of J: d y[i] / d a[j].
    // backward() clears the previous gradients before applying the new seed.
    for (std::size_t i = 0; i < length; ++i)
    {
        float seed[length] = {};
        seed[i] = 1.0F;
        std::printf("\nBackward pass for y[%u]:\n", static_cast<unsigned>(i));
        print_vector("seed", seed);
        if (!tape.backward(y, seed, length))
        {
            check_tape(tape);
            std::exit(EXIT_FAILURE);
        }
        print_vector("dy[i]/da", a.gradients());
    }

    std::printf("\nThe Jacobian dy/da is the 4 x 4 identity matrix.\n");
    std::printf("x is a fixed input: the tape does not store its gradients.\n");
    std::printf("\nStep 4: Finished. No training or parameter update.\n");
    print_vector("a", a.values());
    print_vector("x", x.values());
    print_vector("y", y.values());
    std::exit(EXIT_SUCCESS);
}
