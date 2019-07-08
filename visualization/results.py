import matplotlib.pyplot as plt
import numpy as np
from numpy import *
import matplotlib.image as mpimg
from matplotlib import animation as anim
import os

pathOutputDir		= '..\\output\\'
pathBatchFiles   	= 'batch_'

listdir 			= os.listdir(pathOutputDir)
relevantFilesB   	= list(filter(lambda x: x.startswith(pathBatchFiles), listdir))

dataFitness         = []
dataFitnessCombined = []

# read batch files
for file in relevantFilesB:
    dataFitness.append(np.genfromtxt(pathOutputDir+file, delimiter=';'))  


for data in dataFitness:
    tmp = []
    for entry in data:
        tmp.append(entry[0] + entry[1])
    dataFitnessCombined.append(tmp)

for data in dataFitnessCombined:
    print(data)
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
cm = plt.get_cmap('gist_rainbow')
NUM_COLORS = 20
ax.set_prop_cycle('color', [cm(1.*i/NUM_COLORS) for i in range(NUM_COLORS)])


for i in range(len(relevantFilesB)):
    ax.scatter([w[0] for w in dataFitness[i]], [w[1] for w in dataFitness[i]], s=12, marker='o', label=i)
    #ax.plot([w[0] for w in dataFitness[i]], [w[1] for w in dataFitness[i]], label=i)
    ax.legend()


plt.show()