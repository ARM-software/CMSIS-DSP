# Introduction {#autodiff_introduction}

Automatic differentiation, or **autodiff**, helps an algorithm learn from its
errors. You describe a calculation and a measure of how far its result is from
the desired result. Autodiff then calculates how each adjustable parameter
affects that error. An optimizer uses this information to adjust the parameters
and try to reduce the error. You do not have to work out the derivative formulas
by hand.

CMSIS-DSP autodiff is **experimental**. Its focus is on-device fine-tuning using
existing CMSIS-DSP kernels. It is not a new machine-learning framework.

**When it helps**

Autodiff can help when an algorithm needs to adapt to new measurements or to a
particular device. For example, you might learn filter coefficients from a
reference signal, or fine-tune the last layer of a classifier using new
examples. The aim is to adjust selected parameters in a small computation,
without retraining an entire model.

**Why it uses the C++ API**

The DSP++ C++ API provides a higher-level way to combine existing CMSIS-DSP
kernels into new algorithms while keeping good performance. Its loop fusion
feature can combine several operations into a single loop, avoiding temporary
arrays and repeated passes over the data. Building autodiff on this API makes
it easier to write efficient fine-tuning algorithms from the existing kernels.
See the @ref dsppp_intro "DSP++ introduction" for an example of how this works.

See the @ref AutodiffExample "Iris classification example" for a complete
training and inference application.
