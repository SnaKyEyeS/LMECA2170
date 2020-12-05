
import os,sys
import subprocess
import random
import numpy as np

version = "V1"
dirs = os.listdir('./')

def loguniform(low=0, high=1, size=None, base=10):
    """
    Return a random number following a loguniform distribution, from:
    https://stackoverflow.com/questions/43977717/how-do-i-generate-log-uniform-distribution-in-python
    """
    return np.power(base, np.random.uniform(low, high, size))


if 'Data' not in dirs:
  os.mkdir('Data')

dirs = os.listdir('./Data')
idx = 0
for f in dirs:
  if version in f:
    tmp = int(f.split("-")[1])
    if tmp > idx:
      idx = tmp

idx += 1

minp = 2
maxp = 6


x = [0]
y = [0]


with open('./Data/' + version + "-" + str(idx),'w+') as f:
  while True:
    nbPoint = int(loguniform(minp, maxp))
    bashCommand = "./build/bin/myexec -b " + str(nbPoint)
    process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()
    if error is not None:
      print(error)
    else:
      timeExec = int(output.decode('UTF-8').split('\n')[0])
      f.write(str(nbPoint) + " " + str(timeExec) + "\n")



