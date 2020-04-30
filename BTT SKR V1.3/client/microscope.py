import serial
import re
import numpy as np
from time import sleep
import matplotlib.pyplot as plt


class Microscope:
    def __init__(self, port='/dev/ttyACM0', baudrate=250000):
        self.serial = serial.Serial('/dev/ttyACM0', 250000)

    def write(self, command):
        self.serial.write((command.upper() + ' \n').encode())
        sleep(0.05)
        return self.serial.read_all().decode()

    def read(self):
        return self.serial.read_all()

    def close(self):
        self.serial.close()

    def positions(self):
        txt = self.write('g95')
        pos = np.asarray(re.findall("\d+\.\d+", txt))
        return pos.astype(np.float)


def probe_positions(t=6, velocity=True):
  dat = []
  while True:
    pos = m.positions()
    dat.append(pos)
    if pos[0] > t:
        break
    else:
        sleep(0.002)

    dat = np.asarray(dat).T

    if velocity:
      plt.plot(dat[0, :-1], np.diff(dat[1])/np.diff(dat[0]), 'b.')
    else:
      plt.plot(dat[0], dat[1], 'b.')
    plt.show()


m = Microscope()

# m.write('g28 x')
m.write('M113 S1')
m.write('g93')
m.write(f'g0 x5')
m.write(f'g0 x0')

for i in range(20):
  print(m.read())
  sleep(0.5)

# probe_positions()
m.close()
