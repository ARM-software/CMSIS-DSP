#!/usr/bin/env python3
"""PyTorch counterpart of dsppp/Examples/autodiff_iris.cpp."""

from __future__ import annotations

import argparse
import re
from pathlib import Path

import numpy as np
import torch
from torch import nn

from pytorch_to_autodiff import convert_module


INPUT_SIZE = 4
HIDDEN_SIZE = 8
CLASS_COUNT = 3
TRAINING_COUNT = 120


class IrisNetwork(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        self.hidden = nn.Linear(INPUT_SIZE, HIDDEN_SIZE)
        self.relu = nn.ReLU()
        self.output = nn.Linear(HIDDEN_SIZE, CLASS_COUNT)
        self.softmax = nn.Softmax(dim=-1)

    def logits(self, value: torch.Tensor) -> torch.Tensor:
        return self.output(self.relu(self.hidden(value)))

    def forward(self, value: torch.Tensor) -> torch.Tensor:
        return self.softmax(self.logits(value))


def create_model() -> nn.Module:
    """Factory used by pytorch_to_autodiff.py."""
    return IrisNetwork()


def load_dataset() -> tuple[torch.Tensor, torch.Tensor]:
    """Load the exact samples and normalization used by the C++ example."""
    header = Path(__file__).resolve().parents[4] / "Examples" / "iris_data.hpp"
    text = header.read_text(encoding="utf-8")
    pattern = re.compile(
        r"\{\{\s*([-+0-9.]+)F,\s*([-+0-9.]+)F,\s*"
        r"([-+0-9.]+)F,\s*([-+0-9.]+)F\s*\},\s*([0-2])\s*\}"
    )
    rows = pattern.findall(text)
    if len(rows) != 150:
        raise RuntimeError(f"expected 150 Iris samples in {header}, found {len(rows)}")
    features = np.asarray([[float(value) for value in row[:4]] for row in rows], dtype=np.float32)
    labels = np.asarray([int(row[4]) for row in rows], dtype=np.int64)
    mean = np.asarray([5.843333, 3.057333, 3.758000, 1.199333], dtype=np.float32)
    inverse_std = np.asarray([1.211678, 2.301971, 0.568374, 1.316322], dtype=np.float32)
    features = (features - mean) * inverse_std
    return torch.from_numpy(features), torch.from_numpy(labels)


def is_test_sample(index: int) -> bool:
    return (index % 50) % 5 == 0


def initialize_like_cpp(model: IrisNetwork, generator: torch.Generator) -> None:
    with torch.no_grad():
        for layer in (model.hidden, model.output):
            layer.weight.uniform_(-0.25, 0.25, generator=generator)
            layer.bias.zero_()


def train(model: IrisNetwork, epochs: int, seed: int) -> tuple[int, int]:
    features, labels = load_dataset()
    training_indices = torch.tensor(
        [index for index in range(len(labels)) if not is_test_sample(index)], dtype=torch.long
    )
    test_indices = torch.tensor(
        [index for index in range(len(labels)) if is_test_sample(index)], dtype=torch.long
    )
    generator = torch.Generator().manual_seed(seed)
    initialize_like_cpp(model, generator)
    optimizer = torch.optim.Adam(model.parameters(), lr=1.0e-2)
    loss_function = nn.CrossEntropyLoss(reduction="sum")

    model.train()
    for epoch in range(epochs):
        order = training_indices[torch.randperm(TRAINING_COUNT, generator=generator)]
        epoch_loss = 0.0
        for sample in order.tolist():
            optimizer.zero_grad()
            logits = model.logits(features[sample : sample + 1])
            loss = loss_function(logits, labels[sample : sample + 1])
            loss.backward()
            optimizer.step()
            epoch_loss += float(loss.detach())
        if (epoch + 1) % 20 == 0:
            print(f"epoch {epoch + 1}: mean loss={epoch_loss / TRAINING_COUNT:g}")

    model.eval()
    with torch.no_grad():
        detected = model(features[test_indices]).argmax(dim=1)
        correct = int((detected == labels[test_indices]).sum())
    print(f"final test accuracy={correct}/30 tests")
    return correct, len(test_indices)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--epochs",
        type=int,
        help="train for this many epochs; defaults to 120 when not exporting",
    )
    parser.add_argument("--seed", type=int, default=0x12345678)
    parser.add_argument("--export", type=Path, help="export trained model to an autodiff header")
    parser.add_argument("--dtype", choices=("float32", "float16"), default="float32")
    parser.add_argument("--arena-bytes", type=int, default=2048)
    arguments = parser.parse_args()
    if arguments.epochs is not None and arguments.epochs < 0:
        parser.error("--epochs must be nonnegative")

    torch.manual_seed(arguments.seed)
    model = IrisNetwork()
    trained = arguments.epochs is not None or arguments.export is None
    if trained:
        train(model, arguments.epochs if arguments.epochs is not None else 120,
              arguments.seed)
    else:
        model.eval()
    if arguments.export is not None:
        generated = convert_module(
            model,
            (INPUT_SIZE,),
            class_name="IrisNetwork",
            namespace="generated_iris",
            dtype=arguments.dtype,
            arena_bytes=arguments.arena_bytes,
            export_mode="trained" if trained else "empty",
        )
        arguments.export.write_text(generated, encoding="utf-8")
        print(f"wrote {arguments.export}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
