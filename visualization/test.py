import matplotlib.pyplot as plt
import numpy as np
from numpy import *
import matplotlib.image as mpimg
from matplotlib import animation as anim
import os

def isInt(x):
    return np.equal(np.mod(x, 1), 0)

pathOutputDir		= '..\\output\\'
pathTestFiles   	= 'bixelweights_'

listdir 			= os.listdir(pathOutputDir)
relevantFilesT   	= list(filter(lambda x: x.startswith(pathTestFiles), listdir))

dataInt         = []

for file in relevantFilesT:
    dataInt.append(np.genfromtxt(pathOutputDir+file, delimiter=';'))  


for data in dataInt:
    p = 0
    for entry in data:
        if (not(isInt(entry))):
            p += 1
    print(p)

