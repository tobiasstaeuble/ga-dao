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

# definition of source files
pathCT 				= '..\\problemset\\ct.csv'
pathDij 			= '..\\problemset\\dij.csv'
pathVOI 			= '..\\problemset\\voi.csv'
pathAngles 			= '..\\problemset\\resources\\angles.csv'
#pathBixelweights 	= '..\\problemset\\bixelweights.csv'
pathBixelweights 	= '..\\output\\bixelweights_01001'

# load source files
dataCT 				= np.genfromtxt(pathCT, delimiter=',')
dataDij 			= np.genfromtxt(pathDij, delimiter=',')
dataVOI 			= np.genfromtxt(pathVOI, delimiter=',')
dataAngles 			= np.genfromtxt(pathAngles, delimiter=',')
dataBixelweights 	= np.genfromtxt(pathBixelweights, delimiter=',')

print(dataBixelweights);

# display ct image
#plt.imshow(dataCT, interpolation='none', cmap='gray')
#print(np.shape(dataVOI))

# overlay vois
# plt.contour(dataVOI, cmap='jet')

# display Dij
# Dij = np.sum(dataDij, 0)
# Dij = np.reshape(Dij, np.shape(dataCT)[::-1])
# plt.imshow(np.transpose(Dij))

# display optimized dose
dose = np.dot(np.matrix.transpose(dataDij),np.matrix.transpose(dataBixelweights))
dose = np.reshape(dose, np.shape(dataCT)[::-1])
dose = np.transpose(dose)
print(dose)
#maskedDose = np.ma.masked_where(dose < 0, dose) # mask dose for transparency reasons

plt.imshow(dose, cmap='jet', interpolation='none', alpha=0.5)

#plt.imshow(maskedDose, cmap='jet', interpolation='none', alpha=0.5)
#plt.contourf(maskedDose, cmap='jet', interpolation='none', alpha=0.4)

plt.show()

