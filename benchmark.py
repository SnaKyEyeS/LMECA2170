
import os,sys
import subprocess
import random

version = "V0"
dirs = os.listdir('./')

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

minp = 100
maxp = 100000


x = [0]
y = [0]


with open('./Data/' + version + "-" + str(idx),'w+') as f:
  while True:
    nbPoint = random.randint(minp,maxp)
    bashCommand = "./build/bin/myexec -b " + str(nbPoint)
    process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()
    if error is not None:
      print(error)
    else:
      timeExec = int(output.decode('UTF-8').split('\n')[0])
      f.write(str(nbPoint) + " " + str(timeExec) + "\n")