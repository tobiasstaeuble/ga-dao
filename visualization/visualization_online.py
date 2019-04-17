'''
Visualizes data used and produced in the course Computational Science II (UZH). 

The project's goal is to calculate and optimize aperture time for a given treatment
plan to be delivered by a generic irradation device that uses a multi-leaf collimator.

The optimization is realized by using the multi-objective
genetic algorithmic library (MOGAL) written by Jori Liesenborgs.

Author: Tobias Staeuble
Copyright: none
License: none
Version: 0.0.1
Maintainer: Tobias Staeuble
Email: tobias.staeuble@uzh.ch
Status: in development
'''

import matplotlib.pyplot as plt
import numpy as np
from numpy import *
import matplotlib.image as mpimg
from matplotlib import animation as anim
import os

# definition of source files
pathCT 				= '..\\problemset\\ct.csv'
pathDij 			= '..\\problemset\\dij.csv'
pathVOI 			= '..\\problemset\\voi.csv'
pathAngles 			= '..\\problemset\\resources\\angles.csv'
#pathBixelweights 	= '..\\problemset\\bixelweights.csv'
pathOutputDir		= '..\\output\\'
pathBixelweights 	= 'bixelweights_'
pathDoseMatrix		= 'dose_'
pathMinDoseMatrix	= '..\\problemset\\minDoseMatrix.csv'
pathVOIMatrix		= '..\\problemset\\voiAreaMatrix.csv'


listdir 			= os.listdir(pathOutputDir)
relevantFilesBW 	= list(filter(lambda x: x.startswith(pathBixelweights), listdir))
relevantFilesDose 	= list(filter(lambda x: x.startswith(pathDoseMatrix), listdir))

# load source files
dataCT 				= np.genfromtxt(pathCT, delimiter=',')
dataDij 			= np.genfromtxt(pathDij, delimiter=',')
dataVOI 			= np.genfromtxt(pathVOI, delimiter=',')
dataAngles 			= np.genfromtxt(pathAngles, delimiter=',')
dataBixelweights 	= np.genfromtxt(pathOutputDir + relevantFilesBW[0], delimiter=',')
dataDoseMatrix		= np.genfromtxt(pathOutputDir + relevantFilesDose[0], delimiter=',')
#dataMinDoseMatrix	= np.genfromtxt(pathMinDoseMatrix, delimiter=',')
#dataVOIMatrix		= np.genfromtxt(pathVOIMatrix, delimiter=',')

print(dataBixelweights);

# prepare plot
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.contour(dataVOI, cmap='jet')
ax.imshow(dataCT, interpolation='none', cmap='gray')

# dose directly
dataDoseMatrix = np.genfromtxt(pathOutputDir + relevantFilesDose[0], delimiter=',')
dataDoseMatrix = np.transpose(np.reshape(dataDoseMatrix, np.shape(dataCT)[::-1]))
dose_imm = ax.imshow(dataDoseMatrix, cmap='jet', interpolation='none', alpha=0.3)


# dose via bw
#dataBixelweights = np.genfromtxt(pathOutputDir + relevantFilesBW[0], delimiter=',')
#dose_bw = np.dot(np.matrix.transpose(dataDij),np.matrix.transpose(dataBixelweights))
#dose_bw = np.reshape(dose_bw, np.shape(dataCT)[::-1])
#dose_bw = np.transpose(dose_bw)
#dose_imb = ax.imshow(dose_bw, cmap='jet', interpolation='none', alpha=0.3)

def animate(i):
	global currentStepOfFile
	print(i)

	# dose directly
	dataDoseMatrix = np.genfromtxt(pathOutputDir + relevantFilesDose[i], delimiter=',')
	dataDoseMatrix = np.transpose(np.reshape(dataDoseMatrix, np.shape(dataCT)[::-1]))
	dose_imm.set_data(dataDoseMatrix)
	dose_imm.autoscale()
	# dose via bw
	#dataBixelweights = np.genfromtxt(pathOutputDir + relevantFilesBW[i], delimiter=',')
	#dose_bw = np.dot(np.matrix.transpose(dataDij),np.matrix.transpose(dataBixelweights))
	#dose_bw = np.reshape(dose_bw, np.shape(dataCT)[::-1])
	#dose_bw = np.transpose(dose_bw)
	#dose_imb.set_array(dose_bw)
	#dose_imb.autoscale()


anim = anim.FuncAnimation(fig, animate, frames=len(relevantFilesDose), interval=100, repeat=False)
#dataMinDoseMatrix = np.transpose(np.reshape(dataMinDoseMatrix, np.shape(dataCT)[::-1]))
#dataVOIMatrix = np.transpose(np.reshape(dataVOIMatrix, np.shape(dataCT)[::-1]))

#plt.imshow(dataMinDoseMatrix, cmap='jet', interpolation='none', alpha=0.6)
#plt.imshow(dataVOIMatrix, cmap='jet', interpolation='none', alpha=0.4)

#plt.contourf(maskedDose, cmap='jet', interpolation='none', alpha=0.4)

plt.show()

