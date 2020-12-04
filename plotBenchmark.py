import numpy as np
import matplotlib.pyplot as plt

filename = 'Data/V0-1'
filename2 = 'Data/V1-4'
filename3 = 'Data/V1-5'
x = []
y = []

x2 = []
y2 = []

x3 = []
y3 = []

with open(filename) as f:
  for line in f:
    x.append(int(line.split(" ")[0]))
    y.append(int(line.split(" ")[1])/1000000)

with open(filename2) as f:
  for line in f:
    x2.append(int(line.split(" ")[0]))
    y2.append(int(line.split(" ")[1])/1000000)

with open(filename3) as f:
  for line in f:
    x3.append(int(line.split(" ")[0]))
    y3.append(int(line.split(" ")[1])/1000000)

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.loglog(x,y, '.r')
ax.loglog(x2,y2, '.b')
ax.loglog(x3,y3, '.g')
plt.show()