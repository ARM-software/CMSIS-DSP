#!/usr/bin/env python3
"""Convert a supported PyTorch nn.Module to CMSIS-DSP autodiff C++ code."""

from __future__ import annotations

import argparse
import importlib.util
import math
import re
import sys
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Any, Sequence

import torch
from torch import fx, nn


class ConversionError(RuntimeError):
    """Raised when a module cannot be represented by the autodiff API."""


@dataclass(frozen=True)
class Value:
    name: str
    shape: tuple[int, ...]

    @property
    def length(self) -> int:
        return math.prod(self.shape) if self.shape else 1


@dataclass(frozen=True)
class Parameter:
    name: str
    shape: tuple[int, ...]
    values: tuple[float, ...]
    trainable: bool
    matrix_view: bool = False
    initialization: str = "preserve"


@dataclass(frozen=True)
class Operation:
    kind: str
    output: Value
    inputs: tuple[str, ...]
    attributes: tuple[Any, ...] = ()


@dataclass(frozen=True)
class ConvertedModule:
    input: Value
    output_name: str
    values: tuple[Value, ...]
    parameters: tuple[Parameter, ...]
    operations: tuple[Operation, ...]


def _identifier(name: str) -> str:
    result = re.sub(r"[^A-Za-z0-9_]", "_", name)
    if not result or result[0].isdigit():
        result = "value_" + result
    return result


def _shape_text(shape: Sequence[int]) -> str:
    return "scalar" if not shape else " x ".join(str(v) for v in shape)


def _normalized_shape(node: fx.Node) -> tuple[int, ...]:
    value = node.meta.get("val")
    if not isinstance(value, torch.Tensor):
        raise ConversionError(
            f"node '{node.name}' does not produce a tensor with a known shape"
        )
    shape = tuple(int(dimension) for dimension in value.shape)
    if not shape or shape[0] != 1:
        raise ConversionError(
            f"node '{node.name}' has shape {shape}; the leading batch dimension "
            "must exist and equal 1"
        )
    embedded_shape = shape[1:]
    if len(embedded_shape) > 2:
        raise ConversionError(
            f"node '{node.name}' becomes {_shape_text(embedded_shape)} after "
            "removing batch 1; autodiff supports only scalars, vectors, and matrices"
        )
    return embedded_shape


def _tensor_values(tensor: torch.Tensor) -> tuple[float, ...]:
    flat = tensor.detach().cpu().reshape(-1).tolist()
    result = tuple(float(value) for value in flat)
    if not all(math.isfinite(value) for value in result):
        raise ConversionError("model parameters must contain only finite values")
    return result


def _node_argument(argument: Any, aliases: dict[fx.Node, str]) -> str:
    if not isinstance(argument, fx.Node) or argument not in aliases:
        raise ConversionError("operator arguments must be traced tensor values")
    return aliases[argument]


def _validate_vector_softmax(node: fx.Node, shape: tuple[int, ...], dim: int) -> None:
    if len(shape) != 1 or dim not in (-1, 1):
        raise ConversionError(
            f"softmax node '{node.name}' must operate on the feature dimension "
            "of a batch-1 vector; got embedded shape " + _shape_text(shape)
        )


def inspect_module(module: nn.Module, input_shape: Sequence[int]) -> ConvertedModule:
    """Export and validate a module, returning an architecture-neutral model."""
    shape = tuple(int(value) for value in input_shape)
    if any(value <= 0 for value in shape):
        raise ConversionError("all input dimensions must be positive")
    if len(shape) > 2:
        raise ConversionError(
            "input_shape excludes batch and may describe only a scalar, vector, or matrix"
        )

    model_parameters = dict(module.named_parameters())
    first_parameter = next(iter(model_parameters.values()), None)
    dtype = first_parameter.dtype if first_parameter is not None else torch.float32
    device = first_parameter.device if first_parameter is not None else torch.device("cpu")
    example = torch.zeros((1, *shape), dtype=dtype, device=device)
    try:
        exported = torch.export.export(module, (example,))
    except Exception as error:
        raise ConversionError(f"PyTorch ATen export failed: {error}") from error

    graph = exported.graph_module.graph
    specs = {spec.arg.name: spec for spec in exported.graph_signature.input_specs}
    aliases: dict[fx.Node, str] = {}
    shapes: dict[str, tuple[int, ...]] = {}
    values: list[Value] = []
    parameters: list[Parameter] = []
    parameter_indices: dict[str, int] = {}
    operations: list[Operation] = []
    input_value: Value | None = None
    output_name: str | None = None

    def tensor_for_spec(spec: Any) -> torch.Tensor:
        target = str(spec.target)
        kind = spec.kind.name
        if kind in ("PARAMETER", "BUFFER"):
            tensor = exported.state_dict[target]
        elif kind == "CONSTANT_TENSOR":
            tensor = exported.constants[target]
        else:
            raise ConversionError(f"unsupported lifted input kind {kind}")
        if not isinstance(tensor, torch.Tensor):
            raise ConversionError(f"lifted input '{target}' is not a tensor")
        return tensor

    def unique_parameter_name(target: str) -> str:
        base = _identifier(target)
        name = base
        suffix = 2
        while name in parameter_indices:
            name = f"{base}_{suffix}"
            suffix += 1
        return name

    for node in graph.nodes:
        if node.op != "placeholder":
            continue
        spec = specs.get(str(node.target))
        if spec is None:
            raise ConversionError(f"placeholder '{node.name}' has no graph signature entry")
        if spec.kind.name == "USER_INPUT":
            if input_value is not None:
                raise ConversionError("exactly one tensor input is supported")
            input_value = Value("input", _normalized_shape(node))
            aliases[node] = input_value.name
            shapes[input_value.name] = input_value.shape
            continue
        tensor = tensor_for_spec(spec)
        parameter_shape = tuple(int(value) for value in tensor.shape)
        if len(parameter_shape) > 2:
            raise ConversionError(
                f"parameter '{spec.target}' has unsupported shape {parameter_shape}"
            )
        name = unique_parameter_name(str(spec.target))
        source_parameter = model_parameters.get(str(spec.target))
        parameter = Parameter(
            name,
            parameter_shape,
            _tensor_values(tensor),
            bool(source_parameter is not None and source_parameter.requires_grad),
        )
        parameter_indices[name] = len(parameters)
        parameters.append(parameter)
        aliases[node] = name
        shapes[name] = parameter_shape

    if input_value is None:
        raise ConversionError("exactly one tensor input is supported")

    def mark_parameter(name: str, *, matrix_view: bool = False,
                       initialization: str = "preserve") -> None:
        index = parameter_indices.get(name)
        if index is None:
            raise ConversionError(f"'{name}' is not a lifted model parameter")
        current = parameters[index]
        parameters[index] = replace(
            current,
            matrix_view=current.matrix_view or matrix_view,
            initialization=(
                initialization
                if current.initialization == "preserve"
                else current.initialization
            ),
        )

    def new_output(node: fx.Node) -> Value:
        value = Value(_identifier(node.name), _normalized_shape(node))
        values.append(value)
        aliases[node] = value.name
        shapes[value.name] = value.shape
        return value

    def add_scalar_constant(value: Any, node_name: str) -> str:
        try:
            scalar = float(value)
        except (TypeError, ValueError) as error:
            raise ConversionError(
                f"ATen node '{node_name}' has a non-numeric scalar operand"
            ) from error
        if not math.isfinite(scalar):
            raise ConversionError(f"ATen node '{node_name}' has a non-finite scalar operand")
        name = unique_parameter_name(node_name + "_constant")
        parameter_indices[name] = len(parameters)
        parameters.append(Parameter(name, (), (scalar,), False))
        shapes[name] = ()
        return name

    def operand(argument: Any, node_name: str) -> tuple[str, tuple[int, ...], bool]:
        if isinstance(argument, fx.Node):
            if argument not in aliases:
                raise ConversionError(f"ATen node '{node_name}' uses an unsupported value")
            name = aliases[argument]
            return name, shapes[name], name in parameter_indices
        name = add_scalar_constant(argument, node_name)
        return name, (), True

    def add_arithmetic(node: fx.Node, kind: str) -> None:
        if len(node.args) < 2:
            raise ConversionError(f"ATen arithmetic node '{node.name}' needs two operands")
        if float(node.args[2] if len(node.args) > 2 else node.kwargs.get("alpha", 1.0)) != 1.0:
            raise ConversionError(
                f"ATen arithmetic node '{node.name}' uses unsupported alpha"
            )
        left_name, left_shape, left_parameter = operand(node.args[0], node.name)
        right_name, right_shape, right_parameter = operand(node.args[1], node.name)
        output = new_output(node)
        if left_shape == right_shape:
            if output.shape != left_shape:
                raise ConversionError(f"ATen arithmetic node '{node.name}' changed shape")
            operations.append(Operation(kind, output, (left_name, right_name)))
            return
        if kind in ("add", "multiply"):
            if not left_shape and left_parameter and right_shape == output.shape:
                scalar_name, value_name = left_name, right_name
            elif not right_shape and right_parameter and left_shape == output.shape:
                scalar_name, value_name = right_name, left_name
            else:
                raise ConversionError(
                    f"ATen node '{node.name}' uses broadcasting not supported by autodiff"
                )
            operations.append(Operation(
                "offset" if kind == "add" else "scale",
                output,
                (value_name, scalar_name),
            ))
            return
        raise ConversionError(
            f"ATen subtraction node '{node.name}' requires equal-shaped operands"
        )

    def add_dot(node: fx.Node) -> None:
        left_name, left_shape, _ = operand(node.args[0], node.name)
        right_name, right_shape, _ = operand(node.args[1], node.name)
        output = new_output(node)
        if len(left_shape) != 1 or left_shape != right_shape or output.shape:
            raise ConversionError(
                f"ATen node '{node.name}' is not a vector dot product"
            )
        operations.append(Operation("dot", output, (left_name, right_name)))

    aten = torch.ops.aten
    arithmetic = {
        aten.add.Tensor: "add",
        aten.add.Scalar: "add",
        aten.sub.Tensor: "sub",
        aten.sub.Scalar: "sub",
        aten.mul.Tensor: "multiply",
        aten.mul.Scalar: "multiply",
    }
    dot_targets = {aten.matmul.default, aten.inner.default, aten.dot.default}

    for node in graph.nodes:
        if node.op == "placeholder":
            continue
        if node.op == "output":
            returned = node.args[0]
            if not isinstance(returned, (tuple, list)) or len(returned) != 1:
                raise ConversionError("the module must return one tensor")
            output_name = _node_argument(returned[0], aliases)
            continue
        if node.op != "call_function":
            raise ConversionError(
                f"unsupported exported graph node '{node.name}' of kind '{node.op}'"
            )

        target = node.target
        if target == aten.linear.default:
            source = _node_argument(node.args[0], aliases)
            weight = _node_argument(node.args[1], aliases)
            source_shape = shapes[source]
            weight_shape = shapes[weight]
            if len(source_shape) != 1 or len(weight_shape) != 2:
                raise ConversionError(f"ATen linear node '{node.name}' requires vector input")
            if source_shape[0] != weight_shape[1]:
                raise ConversionError(f"ATen linear node '{node.name}' has incompatible dimensions")
            mark_parameter(weight, matrix_view=True, initialization="xavier")
            if len(node.args) < 3 or node.args[2] is None:
                bias = unique_parameter_name(node.name + "_bias")
                parameter_indices[bias] = len(parameters)
                parameters.append(
                    Parameter(bias, (weight_shape[0],), (0.0,) * weight_shape[0], False)
                )
                shapes[bias] = (weight_shape[0],)
            else:
                bias = _node_argument(node.args[2], aliases)
                if shapes[bias] != (weight_shape[0],):
                    raise ConversionError(f"ATen linear node '{node.name}' has invalid bias")
                mark_parameter(bias, initialization="zero")
            output = new_output(node)
            if output.shape != (weight_shape[0],):
                raise ConversionError(f"ATen linear node '{node.name}' has invalid output")
            operations.append(Operation("fully_connected", output, (source, weight, bias)))
        elif target == aten.relu.default:
            output = new_output(node)
            operations.append(Operation("relu", output, (_node_argument(node.args[0], aliases),)))
        elif target in (aten.softmax.int, aten._softmax.default):
            output = new_output(node)
            dimension = int(node.args[1])
            _validate_vector_softmax(node, output.shape, dimension)
            operations.append(Operation("softmax", output, (_node_argument(node.args[0], aliases),)))
        elif target == aten.dropout.default:
            source = _node_argument(node.args[0], aliases)
            probability = float(node.args[1])
            if not 0.0 <= probability < 1.0:
                raise ConversionError(f"ATen dropout node '{node.name}' has invalid probability")
            output = new_output(node)
            operations.append(Operation("dropout", output, (source,), (probability,)))
        elif target in arithmetic:
            add_arithmetic(node, arithmetic[target])
        elif target in dot_targets:
            add_dot(node)
        else:
            raise ConversionError(
                f"unsupported ATen operator at node '{node.name}': {target}"
            )

    if output_name is None:
        raise ConversionError("the exported module has no tensor output")
    return ConvertedModule(
        input_value,
        output_name,
        tuple(values),
        tuple(parameters),
        tuple(operations),
    )


def _float_literal(value: float) -> str:
    text = format(value, ".9g")
    if "e" not in text.lower() and "." not in text:
        text += ".0"
    return f"static_cast<Scalar>({text}F)"


def _initializer(parameter: Parameter) -> str:
    if len(parameter.shape) == 2:
        rows, columns = parameter.shape
        groups = []
        for row in range(rows):
            start = row * columns
            groups.append(
                "{" + ", ".join(_float_literal(v) for v in parameter.values[start:start + columns]) + "}"
            )
        return "{" + ",\n        ".join(groups) + "}"
    return "{" + ", ".join(_float_literal(v) for v in parameter.values) + "}"


def _array_suffix(shape: tuple[int, ...]) -> str:
    dimensions = shape if shape else (1,)
    return "".join(f"[{dimension}]" for dimension in dimensions)


def generate_cpp(
    converted: ConvertedModule,
    *,
    class_name: str = "GeneratedNetwork",
    namespace: str = "generated_autodiff",
    dtype: str = "float32",
    arena_bytes: int = 4096,
    export_mode: str = "trained",
) -> str:
    """Generate a reusable C++ class backed by CMSIS-DSP autodiff."""
    if dtype not in ("float32", "float16"):
        raise ConversionError("dtype must be float32 or float16")
    if arena_bytes <= 0:
        raise ConversionError("arena_bytes must be positive")
    if export_mode not in ("trained", "empty"):
        raise ConversionError("export_mode must be trained or empty")
    class_name = _identifier(class_name)
    namespace = _identifier(namespace)

    parameter_name_set = {parameter.name for parameter in converted.parameters}
    value_name_set = {value.name for value in converted.values}

    def view_name(name: str) -> str:
        if name == converted.input.name:
            return "input"
        if name in parameter_name_set:
            return name
        if name in value_name_set:
            return name + "_buffer"
        raise AssertionError(name)

    kinds = {operation.kind for operation in converted.operations}
    header_for = {
        "add": "add",
        "sub": "sub",
        "multiply": "multiply",
        "scale": "scale",
        "offset": "offset",
        "dot": "dot",
        "fully_connected": "fully_connected",
        "relu": "relu",
        "softmax": "softmax",
        "dropout": "dropout",
    }
    operator_for = {
        "add": "AddOperator<Scalar>",
        "sub": "SubOperator<Scalar>",
        "multiply": "MultiplyOperator<Scalar>",
        "scale": "ScaleOperator<Scalar>",
        "offset": "OffsetOperator<Scalar>",
        "dot": "DotOperator<Scalar>",
        "fully_connected": "FullyConnectedOperator<Scalar>",
        "relu": "ReluOperator<Scalar>",
        "softmax": "SoftmaxOperator<Scalar>",
        "dropout": "DropoutOperator<Scalar>",
    }
    includes = []
    if "softmax" in kinds:
        # Include these at global scope before the C++ expression headers.
        # Some architecture headers include the same C headers from their
        # namespace; the include guards must already have established the C
        # declarations globally.
        includes.extend(
            [
                "#include <dsp/fast_math_functions.h>",
                "#include <dsp/fast_math_functions_f16.h>",
            ]
        )
    includes.append("#include <dsppp/autodiff/reverse.hpp>")
    includes.extend(
        f"#include <dsppp/autodiff/operators/{header_for[kind]}.hpp>"
        for kind in sorted(kinds)
    )
    includes.extend(["#include <cstddef>", "#include <cstdint>"])

    scalar = "float" if dtype == "float32" else "float16_t"
    lines = [
        "// Generated by pytorch_to_autodiff.py. Do not edit parameter data by hand.",
        f"// Export mode: {export_mode}.",
        "#pragma once",
        *includes,
        "",
        f"namespace {namespace} {{",
        "using namespace arm_cmsis_dsp::autodiff;",
        "",
        f"class {class_name}",
        "{",
        "public:",
        f"    using Scalar = {scalar};",
        f"    static constexpr std::size_t arena_bytes = {arena_bytes}U;",
        f"    static constexpr std::size_t input_length = {converted.input.length}U;",
        "",
        f"    Scalar input_value[{converted.input.length}]{{}};",
    ]
    for parameter in converted.parameters:
        if export_mode == "empty" and parameter.trainable:
            lines.append(
                f"    Scalar {parameter.name}_value{_array_suffix(parameter.shape)}{{}};"
            )
        else:
            lines.extend(
                [
                    f"    Scalar {parameter.name}_value{_array_suffix(parameter.shape)} =",
                    f"        {_initializer(parameter)};",
                ]
            )
    for value in converted.values:
        lines.append(f"    Scalar {value.name}_value[{value.length}]{{}};")
    if "dropout" in kinds:
        lines.append("    DropoutGenerator dropout_generator{0x6D2B79F5U};")
    lines.extend(
        [
            "",
            "    Arena<arena_bytes, Scalar> arena{};",
            "    Tape<Scalar> &tape;",
            "    BufferView<Scalar> input;",
        ]
    )
    for parameter in converted.parameters:
        view_type = "MatrixView<Scalar>" if parameter.matrix_view else "BufferView<Scalar>"
        lines.append(f"    {view_type} {parameter.name};")
    for value in converted.values:
        lines.append(f"    BufferView<Scalar> {view_name(value.name)};")

    initializers = ["tape(arena.tape())", "input(tape.input(input_value))"]
    for parameter in converted.parameters:
        if len(parameter.shape) == 2 and not parameter.matrix_view:
            length = math.prod(parameter.shape)
            initializers.append(
                f"{parameter.name}(tape.parameter(&{parameter.name}_value[0][0], "
                f"{length}U))"
            )
        else:
            initializers.append(
                f"{parameter.name}(tape.parameter({parameter.name}_value))"
            )
    initializers.extend(
        f"{view_name(value.name)}(tape.output({value.name}_value))"
        for value in converted.values
    )
    lines.extend(["", f"    {class_name}()", "        : " + ",\n          ".join(initializers), "    {"])
    for kind in sorted(kinds):
        lines.append(f"        tape.register_operator<{operator_for[kind]}>();")
    if export_mode == "empty" and any(
        parameter.trainable for parameter in converted.parameters
    ):
        lines.append("        initialize_parameters();")
    lines.append("    }")

    if export_mode == "empty":
        lines.extend(
            [
                "",
                "    void initialize_parameters(std::uint32_t seed = 0x12345678U)",
                "    {",
                "        std::uint32_t state = seed;",
            ]
        )
        for parameter in converted.parameters:
            if not parameter.trainable:
                continue
            if len(parameter.shape) == 2:
                rows, columns = parameter.shape
                bound = math.sqrt(6.0 / (rows + columns))
                lines.extend(
                    [
                        f"        for (std::size_t row = 0; row < {rows}U; ++row)",
                        f"            for (std::size_t column = 0; column < {columns}U; ++column)",
                        "            {",
                        "                state = state * 1664525U + 1013904223U;",
                        "                const float unit = static_cast<float>(",
                        "                    (state >> 8U) & 0xffffU) / 65535.0F;",
                        f"                {parameter.name}_value[row][column] = static_cast<Scalar>(",
                        f"                    (unit * 2.0F - 1.0F) * {format(bound, '.9g')}F);",
                        "            }",
                    ]
                )
            else:
                length = math.prod(parameter.shape) if parameter.shape else 1
                lines.extend(
                    [
                        f"        for (std::size_t i = 0; i < {length}U; ++i)",
                        f"            {parameter.name}_value[i] = Scalar{{}};",
                    ]
                )
        lines.append("    }")

    lines.extend(["", "    bool forward()", "    {"])
    for operation in converted.operations:
        output = view_name(operation.output.name)
        if operation.kind == "fully_connected":
            source, weight, bias = operation.inputs
            expression = (
                f"::arm_cmsis_dsp::autodiff::fully_connected("
                f"{view_name(source)}, {view_name(weight)}, {view_name(bias)})"
            )
        elif operation.kind in ("relu", "softmax"):
            expression = (
                f"::arm_cmsis_dsp::autodiff::{operation.kind}("
                f"{view_name(operation.inputs[0])})"
            )
        elif operation.kind == "add":
            expression = (
                f"{view_name(operation.inputs[0])} + "
                f"{view_name(operation.inputs[1])}"
            )
        elif operation.kind == "sub":
            expression = (
                f"{view_name(operation.inputs[0])} - "
                f"{view_name(operation.inputs[1])}"
            )
        elif operation.kind == "multiply":
            expression = (
                f"{view_name(operation.inputs[0])} * "
                f"{view_name(operation.inputs[1])}"
            )
        elif operation.kind in ("scale", "offset"):
            expression = (
                f"::arm_cmsis_dsp::autodiff::{operation.kind}("
                f"{view_name(operation.inputs[0])}, "
                f"{view_name(operation.inputs[1])})"
            )
        elif operation.kind == "dot":
            expression = (
                f"::arm_cmsis_dsp::autodiff::dot("
                f"{view_name(operation.inputs[0])}, "
                f"{view_name(operation.inputs[1])})"
            )
        elif operation.kind == "dropout":
            probability = format(operation.attributes[0], ".9g")
            expression = (
                f"::arm_cmsis_dsp::autodiff::dropout("
                f"{view_name(operation.inputs[0])}, dropout_generator, "
                f"{probability}F)"
            )
        else:
            raise AssertionError(operation.kind)
        lines.append(f"        {output} = {expression};")
    lines.extend(
        [
            "        return tape.good();",
            "    }",
            "",
            "    BufferView<Scalar> output() const",
            "    {",
            f"        return {view_name(converted.output_name)};",
            "    }",
            "",
            "    template <typename Optimizer>",
            "    bool add_parameters(Optimizer &optimizer)",
            "    {",
            "        bool result = true;",
        ]
    )
    for parameter in converted.parameters:
        if parameter.trainable:
            lines.append(f"        result = optimizer.add({parameter.name}) && result;")
    lines.extend(
        [
            "        return result;",
            "    }",
            "};",
            "",
            f"}} // namespace {namespace}",
            "",
        ]
    )
    return "\n".join(lines)


def convert_module(module: nn.Module, input_shape: Sequence[int], **options: Any) -> str:
    """Validate ``module`` and return generated CMSIS-DSP autodiff C++."""
    return generate_cpp(inspect_module(module, input_shape), **options)


def _parse_shape(text: str) -> tuple[int, ...]:
    if text.strip().lower() in ("", "scalar"):
        return ()
    try:
        return tuple(int(value) for value in re.split(r"[x,]", text))
    except ValueError as error:
        raise argparse.ArgumentTypeError("shape must look like 4 or 3,4") from error


def _load_factory(path: Path, factory_name: str) -> nn.Module:
    path = path.resolve()
    spec = importlib.util.spec_from_file_location("pytorch_autodiff_input", path)
    if spec is None or spec.loader is None:
        raise ConversionError(f"cannot import model file '{path}'")
    module = importlib.util.module_from_spec(spec)
    sys.path.insert(0, str(path.parent))
    try:
        spec.loader.exec_module(module)
    finally:
        sys.path.pop(0)
    factory = getattr(module, factory_name, None)
    if not callable(factory):
        raise ConversionError(f"'{path}' has no callable '{factory_name}'")
    model = factory()
    if not isinstance(model, nn.Module):
        raise ConversionError(f"'{factory_name}' must return torch.nn.Module")
    return model


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("model", type=Path, help="Python file containing a model factory")
    parser.add_argument("--factory", default="create_model", help="zero-argument model factory")
    parser.add_argument(
        "--input-shape",
        required=True,
        type=_parse_shape,
        help="shape excluding the mandatory batch dimension, for example 4 or 3,4",
    )
    parser.add_argument("-o", "--output", type=Path, help="output header (stdout if omitted)")
    parser.add_argument("--class-name", default="GeneratedNetwork")
    parser.add_argument("--namespace", default="generated_autodiff")
    parser.add_argument("--dtype", choices=("float32", "float16"), default="float32")
    parser.add_argument("--arena-bytes", type=int, default=4096)
    parser.add_argument(
        "--export-mode",
        choices=("trained", "empty"),
        default="trained",
        help="embed trained values, or emit an empty randomly initialized network",
    )
    arguments = parser.parse_args(argv)
    try:
        model = _load_factory(arguments.model, arguments.factory)
        generated = convert_module(
            model,
            arguments.input_shape,
            class_name=arguments.class_name,
            namespace=arguments.namespace,
            dtype=arguments.dtype,
            arena_bytes=arguments.arena_bytes,
            export_mode=arguments.export_mode,
        )
        if arguments.output is None:
            sys.stdout.write(generated)
        else:
            arguments.output.write_text(generated, encoding="utf-8")
    except (ConversionError, OSError) as error:
        parser.exit(1, f"error: {error}\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
