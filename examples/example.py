import pandas as pd
import numpy as np
import laat

# read data from csv file
data = pd.read_csv('exampleDataSet.csv', header=None).to_numpy()

# run the LAAT algorithm on the collected data with the specified
# hyper-parameter values, store the resulting pheromone
pheromone = laat.LAAT(data,
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

threshold = 0.7  # pheromone threshold

# use the pheromone values and the threshold to select which datapoints to use
# for MBMS
selectedData = data[pheromone > threshold]

# run the MBMS algorithm on the selected data points with the specified
# hyper-parameter values, store the resulting shifted data
shiftedData = laat.MBMS(selectedData,
                        iter = 10,
                        radius = 3,
                        sigma = 1.5,
                        k = 10)

# write the result to a csv file
np.savetxt('exampleResult.csv', shiftedData, delimiter=',')
