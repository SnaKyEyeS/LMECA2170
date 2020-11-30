import numpy as np
import matplotlib.pyplot as plt

filename = 'Data/V0-1'

x = []
y = []

with open(filename) as f:
  for line in f:
    x.append(int(line.split(" ")[0]))
    y.append(int(line.split(" ")[1])/1000000)

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.loglog(x,y, '.r')
x.sort()

X = np.linspace(x[0],x[-1],300)

print(X)
print(np.log(X))
ax.loglog(X,np.log2(X), '--b')
ax.plot(X,2*np.log2(X),'--g')
plt.show()