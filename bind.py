import pandas as pd
import numpy as np
import laat

data = pd.read_csv('LAAT/datasets/dataA.csv', header=None).to_numpy()
laat.example(data)
