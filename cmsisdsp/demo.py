import numpy as np
import cmsisdsp.datatype as dt 
import cmsisdsp as dsp
import platform 

try:
    import matplotlib.pyplot as plt
    import matplotlib.animation as animation
except ImportError:
    print("matplotlib is not installed. Please install it to visualize the results.")
    print("You can install it using: pip install matplotlib")
    raise ImportError

if platform.system() == "Linux":
   print("On Linux, if you get an error like this:")
   print("  UserWarning: FigureCanvasAgg is non-interactive, and thus cannot be shown")
   print("You may need to install the Python interface to Tk with :")
   print("  sudo apt-get install python3-tk")

# sudo apt-get install python3-tk
# UserWarning: FigureCanvasAgg is non-interactive, and thus cannot be shown

# 1. Generate signal
duration = 20
FFT_SIZE = 512 
nb = FFT_SIZE
t = np.linspace(0, duration, FFT_SIZE*duration, endpoint=False)
signal = (duration-t)/duration*(np.sin(2 * np.pi * 10 * t) + 0.5 * np.random.randn(len(t)))  # 50 Hz sine + noise

# 2. Design simple low-pass FIR filter
#from scipy.signal import firwin
cutoff = 25  # Hz
num_taps = 29
#fir_coeffs = firwin(num_taps, cutoff / (FFT_SIZE / 2))
fir_coeffs = np.array([-0.00173373,  -0.00175053, -0.00178515, -0.00125729,  0.00061816,  0.00466105,
  0.01151756,  0.02146004,  0.03424345,  0.04905677,  0.06458896,  0.07920401,
  0.09119551,  0.09907224,  0.10181794,  0.09907224,  0.09119551,  0.07920401,
  0.06458896,  0.04905677,  0.03424345,  0.02146004,  0.01151756,  0.00466105,
  0.00061816, -0.00125729, -0.00178515, -0.00175053, -0.00173373])



# 3. Prepare FIR filter
firf32 = dsp.arm_fir_instance_f32()
state = np.zeros(len(fir_coeffs) + nb - 1, dtype=np.float32)
dsp.arm_fir_init_f32(firf32,len(fir_coeffs), fir_coeffs.astype(np.float32), state)

def filter(frame):
    return (dsp.arm_fir_f32(firf32,signal.astype(np.float32)[nb*frame:nb*(frame+1)]))

filtered = filter(0)

# 4. Prepare RFFT 
rfftf32=dsp.arm_rfft_fast_instance_f32()
S = dsp.arm_rfft_fast_init_f32(rfftf32,nb)
if S != 0:
    raise ValueError("FFT initialization failed")
if dsp.has_neon():
    tmp_nb = dsp.arm_rfft_tmp_buffer_size(dt.F32,nb,1)
    tmp = np.zeros(tmp_nb,dtype=np.float32)

def to_complex(s):
    res = np.zeros(len(s)//2,dtype=np.complex64)
    res.real = s[::2]
    res.imag = s[1::2]
    return res

def extract_nyquist(r):
    res = np.zeros(len(r)+1,dtype=np.complex64)
    res[:-1] = r
    res[-1]=complex(res[0].imag,0)
    res[0]=complex(res[0].real,0)
    return res

# 4. FFT
def compute_fft(sig):
    if dsp.has_neon():
        res = dsp.arm_rfft_fast_f32(rfftf32,sig,0,tmp=tmp)
        #res = dsp.arm_rfft_fast_f32(rfftf32,sig,0)
    else:
        res = dsp.arm_rfft_fast_f32(rfftf32,sig,0)
    fft = extract_nyquist(to_complex(res))
    return np.abs(fft)

# 5. Plot results
fig=plt.figure(figsize=(10, 6),num="Filtering example")

frame=0
times=t.astype(np.float32)[nb*frame:nb*(frame+1)]
sig = signal.astype(np.float32)[nb*frame:nb*(frame+1)]

a=plt.subplot(2, 2, 1)
plt.title("Original Signal")
line_sig=a.plot(times, sig)[0]
a.set_xlabel("s")

a=plt.subplot(2, 2, 2)
plt.title("Filtered Signal")
line_filtered=a.plot(times, filtered)[0]
a.set_xlabel("s")



a=plt.subplot(2, 2, 3)
plt.title("Original Spectrum")
line_fft_sig=a.plot(np.fft.rfftfreq(len(sig), 1/FFT_SIZE), compute_fft(sig))[0]
a.set_xlabel("Hz")



a=plt.subplot(2, 2, 4)
plt.title("Filtered Spectrum")
line_fft_filtered=a.plot(np.fft.rfftfreq(len(filtered), 1/FFT_SIZE), compute_fft(filtered))[0]
a.set_xlabel("Hz")



plt.tight_layout()
#plt.show()


def update(frame):
    times=t.astype(np.float32)[nb*frame:nb*(frame+1)]
    sig = signal.astype(np.float32)[nb*frame:nb*(frame+1)]
    filtered = filter(frame)
    #line_sig.set_xdata(times)
    line_sig.set_ydata(sig)

    #line_filtered.set_xdata(times)
    line_filtered.set_ydata(filtered)

    line_fft_sig.set_ydata(compute_fft(sig))
    line_fft_filtered.set_ydata(compute_fft(filtered))
    return(line_sig, line_filtered, line_fft_sig, line_fft_filtered)


ani = animation.FuncAnimation(fig=fig, func=update, frames=duration)
plt.show()