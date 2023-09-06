# Encoder_task2

---

It is code that measure the number of counts done by the encoder

## counts function

- the interrupt buttons will be used for the A signal and B signal in the encoder
- the function called when the interrupt change will be reversed between the ecoder A and B as the direction will be reversed so the wheel return back near to the previous position

```cpp
void ISR_ENCODER_FUN_1(){
  if(digital.Read(Encoder_A) != digital.Read(Encoder_B)){
    counts ++;
  }
  else 
    counts--;
}
```

- this is for the encoderA, just the if condition change for the encoder b

## Filter coefficients in python

the low pass filter will be used to remove the noise of this sensor

<aside>
💡 the important libaraty to get the transfer function of the sensor

</aside>

```python
from scipy import signal
import matplotlib.pyplot as plt
import numpy as np
import math
# import librosa
# import librosa.display

from IPython.display import Video
import IPython.display       as ipd
from   scipy.io     import wavfile
```

<aside>
💡 the given from the problem

</aside>

RPS = (max_speed/2*pi*wheel_radius)

PPS = RPS*PPR

```python
Pulse_per_rotation = 540       # PPR
wheel_radius       = 0.2       # meter
max_speed          = 0.5       # m/s

rotation_per_sec   = 0.4      # RPS
Pulse_per_sec      = 216      # PPS
```

<aside>
💡 the variable for the transfer function

</aside>

```python
critical_time = 2
cutoff_Freq   =220  # cut-off frequency more than PPS
sampling_Freq = 1000; # sampled at 1 kHz = 1000 samples / second

w0 = 2 * np.pi * cutoff_Freq;                # pole frequency (rad/s)
num = w0                                     # transfer function numerator coefficients
den = [1,w0]                                 # transfer function denominator coefficients
lowPass = signal.TransferFunction(num, den)
```

- dt convert the continous time sample to discrete time
- gpt is  **Gaussian-Butterworth filter method** it method used to filter the signal
- alpha is the factor of smoothing

```python
dt = 1.0/sampling_Freq;
discreteLowPass = lowPass.to_discrete(dt, method='gbt' ,alpha=0.5)
print(discreteLowPass)
```

<aside>
💡 then we will print the b and a variables

</aside>

```python
b = discreteLowPass.num;
a = -discreteLowPass.den;

print(f' b : {discreteLowPass.num}')
print(f' a : {-discreteLowPass.den}')
```

<aside>
💡 the final equation

</aside>

```python
y[n] = -0.747 y[n-1] + 0.874 x[n] + 0.874 x[n-1]
#y is the filtered sample
#y[n-1] is the previous filtered sample
#x is the input
#x[n-1] is the previous input
```

this equation is added to the .ino file and after each cycle i assign the previous filter and the previous input with the filtered and the input

```python
Previous_count = counts;
 Previous_filtered_count = Filtered_count;
```