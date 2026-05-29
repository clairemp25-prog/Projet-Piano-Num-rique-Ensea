import numpy as np
import matplotlib.pyplot as plt
from scipy.io.wavfile import write

sample_rate = 44100

def bruit_blanc(nb_samples):
    return np.random.uniform(-1, 1, nb_samples)


def karplus_strong(frequency, duration, sample_rate=44100, damping=0.5):
    N = int(sample_rate / frequency)
    buffer = bruit_blanc(N)
    output = []
    for i in range(int(duration * sample_rate)):
        current = buffer[0]
        output.append(current)
        # Le filtre moyenneur
        avg = damping * 0.5 * (buffer[0] + buffer[1])
        buffer = np.append(buffer[1:], avg)
    return np.array(output, dtype=np.float32)


note_la = karplus_strong(440, 2, damping=0.99)

write("test_piano_La.wav", sample_rate, note_la)

plt.plot(note_la)
plt.title('Signal de sortie - Note La')
plt.show()

Do = 261.6
Re_b = 277.2
Re = 293.7
Mi_b = 311.1
Mi = 329.6
Fa = 349.2
Sol_b = 370
Sol = 392
La_b = 415.3
La = 440
Si_b = 466.2
Si = 493.9

