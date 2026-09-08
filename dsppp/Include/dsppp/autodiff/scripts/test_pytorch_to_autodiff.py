import unittest

import torch
from torch import nn

from pytorch_to_autodiff import ConversionError, convert_module


class SupportedModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.first = nn.Linear(4, 8)
        self.relu = nn.ReLU()
        self.second = nn.Linear(8, 3)
        self.softmax = nn.Softmax(dim=-1)

    def forward(self, value):
        return self.softmax(self.second(self.relu(self.first(value))))


class ArithmeticModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.scale = nn.Parameter(torch.tensor(2.0))
        self.offset = nn.Parameter(torch.tensor(-0.5))
        self.dot_weight = nn.Parameter(torch.ones(4))

    def forward(self, value):
        original = value
        value = value + value
        value = value - original
        value = value * value
        value = value * self.scale
        value = value + self.offset
        return torch.matmul(value, self.dot_weight)


class ScalarConstantModel(nn.Module):
    def forward(self, value):
        return value * 2.0 + 1.0


class ConverterTest(unittest.TestCase):
    def test_supported_network(self):
        generated = convert_module(SupportedModel(), (4,), arena_bytes=2048)
        self.assertIn("Export mode: trained", generated)
        self.assertIn("fully_connected(input, first_weight, first_bias)", generated)
        self.assertIn("softmax(linear_1_buffer)", generated)
        self.assertIn("Arena<arena_bytes, Scalar>", generated)

    def test_empty_export_does_not_embed_trainable_values(self):
        model = nn.Linear(4, 3)
        with torch.no_grad():
            model.weight.fill_(0.123456789)
            model.bias.fill_(0.234567891)
        generated = convert_module(
            model, (4,), export_mode="empty"
        )
        self.assertIn("weight_value[3][4]{}", generated)
        self.assertIn("initialize_parameters();", generated)
        self.assertIn("unit * 2.0F - 1.0F", generated)
        self.assertNotIn("0.123456", generated)
        self.assertNotIn("0.234567", generated)

    def test_arithmetic_scale_offset_and_dot(self):
        generated = convert_module(ArithmeticModel(), (4,))
        self.assertIn("AddOperator<Scalar>", generated)
        self.assertIn("SubOperator<Scalar>", generated)
        self.assertIn("MultiplyOperator<Scalar>", generated)
        self.assertIn("ScaleOperator<Scalar>", generated)
        self.assertIn("OffsetOperator<Scalar>", generated)
        self.assertIn("DotOperator<Scalar>", generated)
        self.assertIn("dot(", generated)

    def test_scalar_constants_use_scale_and_offset(self):
        generated = convert_module(ScalarConstantModel(), (4,))
        self.assertIn("ScaleOperator<Scalar>", generated)
        self.assertIn("OffsetOperator<Scalar>", generated)
        self.assertIn("static_cast<Scalar>(2.0F)", generated)
        self.assertIn("static_cast<Scalar>(1.0F)", generated)

    def test_dropout_training_state(self):
        training = nn.Dropout(0.2).train()
        inference = nn.Dropout(0.2).eval()
        self.assertIn("DropoutOperator<Scalar>", convert_module(training, (4,)))
        self.assertIn("DropoutOperator<Scalar>", convert_module(inference, (4,)))
        self.assertIn("dropout(input, dropout_generator, 0.2F)",
                      convert_module(inference, (4,)))
        self.assertIn("DropoutOperator<Scalar>",
                      convert_module(nn.Dropout(0.0).eval(), (4,)))

    def test_unsupported_operator_stops_conversion(self):
        with self.assertRaisesRegex(ConversionError, "unsupported ATen operator"):
            convert_module(nn.Sequential(nn.Sigmoid()), (4,))

    def test_higher_rank_input_stops_conversion(self):
        with self.assertRaisesRegex(ConversionError, "only a scalar, vector, or matrix"):
            convert_module(SupportedModel(), (2, 1, 4))

    def test_linear_requires_vector(self):
        with self.assertRaisesRegex(ConversionError, "requires vector input"):
            convert_module(nn.Linear(4, 3), (2, 4))


if __name__ == "__main__":
    unittest.main()
