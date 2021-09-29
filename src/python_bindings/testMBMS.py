import pandas as pd
import numpy as np
import LAAT

data = pd.read_csv('~/pythonAnt/results/points.csv', header=None).to_numpy()
newData = LAAT.MBMS(data)

np.savetxt("mbms_result.csv", newData, delimiter=",", fmt='%1.4f')
