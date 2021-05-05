import scipy.io as scio
import numpy as np
# change A to B and C for next datasets
tmp = scio.loadmat('dataC.mat')

data = tmp['dataC'][:,0:3]
labels = tmp['dataC'][:,4]
np.savetxt("dataC.csv", data, delimiter=",")
np.savetxt("labelC.csv", labels, delimiter=",")

# Uncomment if you like to see the dataset, each color show a different label, 1 and 2(cluster and filaments) are positive class and 3 and 4(wall and void) are negative class
# import matplotlib.pyplot as plt
# from mpl_toolkits.mplot3d import Axes3D
# fig = plt.figure()
# ax = plt.axes(projection='3d')
# ax.scatter3D(data[:,0], data[:,1], data[:,2], c=labels, cmap='YlGnBu', s=1 )
# plt.show()
