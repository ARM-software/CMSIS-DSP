var autodiff_main =
[
    [ "Introduction", "autodiff_introduction.html", null ],
    [ "Concepts and memory model", "autodiff_concepts.html", [
      [ "Tape and arena", "autodiff_concepts.html#autotoc_md54", null ],
      [ "Values, views, and gradients", "autodiff_concepts.html#autotoc_md55", null ],
      [ "Operator registration", "autodiff_concepts.html#autotoc_md56", null ],
      [ "Recording and RecordingScope", "autodiff_concepts.html#autotoc_md57", null ],
      [ "Reusing arena storage", "autodiff_concepts.html#autotoc_md58", null ],
      [ "Failure model", "autodiff_concepts.html#autotoc_md59", null ]
    ] ],
    [ "Worked implementation flow: `y = a * x`", "autodiff_implementation_flow.html", [
      [ "Complete example", "autodiff_implementation_flow.html#autotoc_md62", null ],
      [ "Objects before the forward pass", "autodiff_implementation_flow.html#autotoc_md63", null ],
      [ "Field map for reverse.hpp", "autodiff_implementation_flow.html#autotoc_md64", null ],
      [ "Forward assignment", "autodiff_implementation_flow.html#autotoc_md65", null ],
      [ "What the node links mean", "autodiff_implementation_flow.html#autotoc_md66", null ],
      [ "backward(y, seed, 2)", "autodiff_implementation_flow.html#autotoc_md67", [
        [ "Validate the root", "autodiff_implementation_flow.html#autotoc_md68", null ],
        [ "Reset graph gradients, then install the seed", "autodiff_implementation_flow.html#autotoc_md69", null ],
        [ "Traverse backward", "autodiff_implementation_flow.html#autotoc_md70", null ]
      ] ],
      [ "Why records store pointers rather than views", "autodiff_implementation_flow.html#autotoc_md71", null ]
    ] ],
    [ "Operators", "autodiff_operators.html", [
      [ "Element-wise arithmetic", "autodiff_operators.html#autotoc_md72", null ],
      [ "Dot, scalar scale, and scalar offset", "autodiff_operators.html#autotoc_md73", null ],
      [ "ReLU and softmax", "autodiff_operators.html#autotoc_md74", null ],
      [ "Signed int8 quantize/dequantize", "autodiff_operators.html#autotoc_md75", null ],
      [ "Losses", "autodiff_operators.html#autotoc_md76", null ],
      [ "Fully connected", "autodiff_operators.html#autotoc_md77", null ],
      [ "Matrix multiply", "autodiff_operators.html#autotoc_md78", null ],
      [ "Dropout", "autodiff_operators.html#autotoc_md79", null ],
      [ "Parameter initialization", "autodiff_operators.html#autotoc_md80", null ],
      [ "Common validation rules", "autodiff_operators.html#autotoc_md81", null ]
    ] ],
    [ "Optimizers", "autodiff_optimizers.html", [
      [ "Capacity arguments", "autodiff_optimizers.html#autotoc_md82", null ],
      [ "SGD", "autodiff_optimizers.html#autotoc_md83", null ],
      [ "RMSProp", "autodiff_optimizers.html#autotoc_md84", null ],
      [ "Adam", "autodiff_optimizers.html#autotoc_md85", null ],
      [ "Adding, clearing, and stepping", "autodiff_optimizers.html#autotoc_md86", null ],
      [ "Freezing", "autodiff_optimizers.html#autotoc_md87", null ],
      [ "Status and errors", "autodiff_optimizers.html#autotoc_md88", null ]
    ] ],
    [ "Training loop", "autodiff_training_loop.html", [
      [ "Complete linear-regression step", "autodiff_training_loop.html#autotoc_md93", null ],
      [ "The flow, in order", "autodiff_training_loop.html#autotoc_md94", [
        [ "Set up fixed state", "autodiff_training_loop.html#autotoc_md95", null ],
        [ "Mark persistent arena allocations", "autodiff_training_loop.html#autotoc_md96", null ],
        [ "Rewind before an iteration", "autodiff_training_loop.html#autotoc_md97", null ],
        [ "Run and record the forward graph", "autodiff_training_loop.html#autotoc_md98", null ],
        [ "Clear optimizer-managed parameter gradients", "autodiff_training_loop.html#autotoc_md99", null ],
        [ "Run reverse propagation", "autodiff_training_loop.html#autotoc_md100", null ],
        [ "Update parameters", "autodiff_training_loop.html#autotoc_md101", null ]
      ] ],
      [ "Batch accumulation", "autodiff_training_loop.html#autotoc_md102", null ],
      [ "Inference", "autodiff_training_loop.html#autotoc_md103", null ]
    ] ],
    [ "Fine-tuning an output layer after Ethos-U", "autodiff_ethos_cmsis_nn_fine_tuning.html", [
      [ "Keeping the interface parameters fixed", "autodiff_ethos_cmsis_nn_fine_tuning.html#autotoc_md60", null ],
      [ "Training and deployment", "autodiff_ethos_cmsis_nn_fine_tuning.html#autotoc_md61", null ]
    ] ],
    [ "Converting PyTorch modules", "autodiff_pytorch_conversion.html", [
      [ "Model constraints", "autodiff_pytorch_conversion.html#autotoc_md89", null ],
      [ "Running the converter", "autodiff_pytorch_conversion.html#autotoc_md90", null ],
      [ "Using generated code for training", "autodiff_pytorch_conversion.html#autotoc_md91", null ],
      [ "Iris comparison", "autodiff_pytorch_conversion.html#autotoc_md92", null ]
    ] ]
];