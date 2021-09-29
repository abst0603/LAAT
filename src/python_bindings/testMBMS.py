import pandas as pd
import numpy as np
import CosmicWeb

data = pd.read_csv('~/pythonAnt/results/points.csv', header=None).to_numpy()
newData = CosmicWeb.MBMS(data)

np.savetxt("mbms_result.csv", newData, delimiter=",", fmt='%1.4f')
