import pandas as pd
import numpy as np
import CosmicWeb

data = pd.read_csv('~/datasets/dataA.csv', header=None).to_numpy()
pheromone = CosmicWeb.LAAT(data,
                           numberOfAntsX=4,
                           numberOfAntsY=5,
                           numberOfAntsZ=5,
                           numberOfIterations=100,
                           numberOfSteps=2500,
                           threshold=3,
                           radius=3,
                           beta=10,
                           kappa=0.8,
                           p_release=0.05,
                           evapRate=0.05,
                           lowerlimit=0.0001,
                           upperlimit=10)

np.savetxt("pheromone.csv", pheromone, fmt='%1.9f', delimiter='\n')
