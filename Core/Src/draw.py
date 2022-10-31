import matplotlib.pyplot as plt
import numpy as np


fs = 21000
f = 440
t = 0.01

samples = np.arange(t * fs) / fs
sine = np.sin(2 * np.pi * f * samples)

f2 = 660
sine2 = np.sin(2 * np.pi * f2 * samples)


fig, axs = plt.subplots(1, 3)
axs[0].plot(sine)
axs[1].plot(sine2)
axs[2].plot((sine * 4 + sine2) / 5)
plt.show()
