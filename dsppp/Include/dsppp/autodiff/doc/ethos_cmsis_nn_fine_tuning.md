# Fine-tuning an output layer after Ethos-U

A common on-device training arrangement is to run a fixed feature extractor on
Ethos-U and train only its final layer with CMSIS-DSP autodiff. During
training, Ethos-U produces a signed-int8 feature vector. The application
dequantizes that vector because the autodiff graph uses floating-point
buffers. Quantization-aware training (QAT) then makes the trainable layer
behave like the int8 layer that will eventually be deployed with Ethos-U or
CMSIS-NN.

The important interface contract is that the Ethos-U output and the trained
layer input use exactly the same quantization parameters. If the Ethos-U
output has scale `s_interface` and zero-point `z_interface`, the input Q/DQ
pair of the autodiff network must use those same fixed values:

```text
Ethos-U output during training                    Autodiff layer

q_ethos (int8) -- dequantize(s_interface, z_interface) --> x (float)
                 -- Q(s_interface, z_interface)
                 -- DQ(s_interface, z_interface) --> trainable layer
```

For an int8 value `q_ethos`, the floating-point training input is:

```text
x = (q_ethos - z_interface) * s_interface
```

Applying Q/DQ with the same parameters maps this value back to `q_ethos` and
then reconstructs the same floating-point value, apart from floating-point
rounding. The pair records the deployed input quantization in the training
graph and also handles values from other floating-point training sources with
the same clipping and rounding that the int8 layer will see.

## Keeping the interface parameters fixed

The interface scale and zero-point are metadata exported with the Ethos-U
network. They are a deployment constraint, not values for the output-layer
optimizer to learn. Copy them into persistent values and use them for both
operators:

```cpp
constexpr Int8Quantization activation_quantization =
    Int8Quantization::activation();

// Exact quantization metadata of the Ethos-U output tensor.
float interface_scale_value = ethos_output_scale;
float interface_zero_point_value = ethos_output_zero_point;

// Q/DQ currently requires parameter views so that its backward records have
// gradient storage. These views remain fixed because they are not added to
// the optimizer.
BufferView interface_scale = tape.parameter(interface_scale_value);
BufferView interface_zero_point = tape.parameter(interface_zero_point_value);

quantized_input = quantize(input, interface_scale, interface_zero_point,
                           activation_quantization);
dequantized_input = dequantize(quantized_input, interface_scale,
                               interface_zero_point,
                               activation_quantization);
```

Do **not** call `optimizer.add(interface_scale)` or
`optimizer.add(interface_zero_point)`. Add only the values that should be
learned, for example the new layer's weights, bias, weight scales, output
scale, and output zero-point:

```cpp
optimizer.add(weights);
optimizer.add(bias);
optimizer.add(weight_scale);
optimizer.add(output_scale);
optimizer.add(output_zero_point);
```

If the interface views were already added to an optimizer, they can instead
be kept unchanged with `freeze_parameters(optimizer, interface_scale,
interface_zero_point)`. Not adding them is simpler when they must remain fixed
for the entire training run.

Although `tape.input()` normally expresses a non-trainable value, it must not
currently be used for these two Q/DQ arguments. The quantize and dequantize
operators require parameter views for scale and, for asymmetric activation
quantization, zero-point. Whether a parameter is updated is controlled by the
optimizer, not by the presence of a Q/DQ node.

The copied scale must be positive and the zero-point must be the integer int8
zero-point from the Ethos-U tensor metadata. Q/DQ applies the
`Int8Quantization::activation()` constraints, but it must not be relied upon
to invent or recalibrate this fixed interface.

## Training and deployment

During training:

1. Run the fixed prefix on Ethos-U and obtain its int8 output.
2. Dequantize it with the Ethos-U output scale and zero-point.
3. Feed the resulting float vector to autodiff.
4. Apply input Q/DQ using the same fixed interface parameters.
5. Train the new layer through its weight Q/DQ and output Q/DQ pairs.

The new layer's weight quantization parameters are independent of the
interface. Its output scale and zero-point may also be learned unless the next
deployed operator imposes another fixed quantized interface. Export the
trained floating-point weights to symmetric per-output-channel int8 weights,
and export the bias as int32 with scale:

```text
bias_scale[channel] = s_interface * weight_scale[channel]
```

For inference, remove the training-only Q/DQ simulation and connect the int8
tensors directly:

```text
Ethos-U prefix -- q_ethos (int8) --> int8 trained layer
```

No data conversion is needed when all of the following match:

- signed-int8 element type;
- tensor shape, ordering, and memory layout;
- input scale equals the Ethos-U output scale;
- input zero-point equals the Ethos-U output zero-point.

For CMSIS-NN, configure the trained layer with
`input_offset = cmsis_nn_offset(interface_zero_point_value)`. Its output
requantization multiplier and shift are derived in the usual way from the
interface scale, per-channel weight scale, and layer output scale.

If either interface quantization parameter differs, direct connection is not
equivalent. An int8-to-int8 requantization step is then required; dequantizing
to float and quantizing again is another, usually less efficient, option. The
purpose of fixing the input Q/DQ parameters during fine-tuning is to avoid
both conversions in the deployed network.
