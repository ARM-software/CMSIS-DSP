import numpy as np
from pylab import figure, clf, plot, xlabel, ylabel, xlim, ylim, title, grid, axes, show,semilogx, semilogy
from numpy import genfromtxt
ref_data = genfromtxt('input_example3.txt', delimiter=',')

figure()
plot(ref_data)

output_data = genfromtxt('../build/output_example3.txt', delimiter=',')

plot(output_data)
show()

print(ref_data.shape)
print(output_data.shape)
nb = output_data.shape[0] - 128

print("Comparison of input and output : max absolute error")
diff = output_data[128:] - ref_data[:nb]
print(np.max(np.abs(diff)))
