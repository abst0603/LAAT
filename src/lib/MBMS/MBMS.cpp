#include "MBMS.ih"

void MBMS(vector<vector<float>> &data,
	  float radius,
	  float sigma,
	  size_t k)
{
  size_t D = data[0].size();

  // initialise k-d trees for neighbor search
  KNN_kd_tree KNN(D, data, 10 /* max leaf */);
  KNN.index->buildIndex();

  // temporary storage for updated data
  vector<vector<float>> dataOut;
  dataOut.assign(data.size(), vector<float>(D, 0));

  for (size_t idx = 0; idx < data.size(); ++idx)
  {
    vector<pair<size_t, float>> matches;

    // find neighbors
    size_t nMatches = KNN.index->radiusSearch(&data[idx][0],
					      radius,
					      matches,
					      nanoflann::SearchParams());

    vector<size_t> neighbors;  // stores indices of neighbors
    if (nMatches >= k)         // found a sufficient number of neighbors
    {
      neighbors.resize(nMatches);
      for (size_t i = 0; i < nMatches; ++i)
	neighbors[i] = matches[i].first;
    }
    else  // find k nearest neighbors
    {
      neighbors.resize(k);
      nanoflann::KNNResultSet<float> resultSet(k);
      vector<size_t> retIndices(k);
      vector<float> distsSqr(k);
      resultSet.init(&retIndices[0], &distsSqr[0]);
      for (unsigned int i = 0; i < k; ++i)
	neighbors[i] = retIndices[i];
    }

    // perform local PCA to find eigen-values and -vectors
    vector<float> eigenValue;
    vector<vector<float>> eigenVector;
    localPCA(data, neighbors, eigenValue, eigenVector);

    // subtract parallel motion
    denoise(data, neighbors, eigenValue, eigenVector, dataOut, sigma, idx);
  }

  // update data
  data = dataOut;
}
