#include "MBMS.ih"

/**
 * Implementation of the Manifold Blurring Mean Shift (MBMS) proposed by Wang
 * & Carreira-Perpinán (2010).
 * See Figure 1 in the paper for a pseudocode of this algorithm.
 *
 * Reference:
 * Wang, W., & Carreira-Perpinán, M. A. (2010, June). Manifold blurring mean
 * shift algorithms for manifold denoising. In 2010 IEEE Computer Society
 * Conference on Computer Vision and Pattern Recognition (pp. 1759-1766). IEEE.
 *
 * @param data vector containing all the data points to apply MBMS to, this
 *   data will be shifted as a result of the MBMS algorithm
 * @param iter amount of times to apply MBMS
 * @param radius radius within which to search for nearest neighbours
 * @param sigma scale of the shift
 * @param k number of nearest neighbours used to estimate the local tangent
 *   space
 */
void manifoldBlurringMeanShift(vector<vector<float>> &data,
			       size_t iter,
			       float radius,
			       float sigma,
			       size_t k)
{
  srand(random_device()());
  for (size_t i = 0; i < iter; ++i)
  {
    size_t D = data[0].size();

    // initialise k-d trees for neighbour search
    KNN_kd_tree KNN(D, data, 10 /* max leaf */);
    KNN.index->buildIndex();

    // temporary storage for updated data
    vector<vector<float>> dataOut;
    dataOut.assign(data.size(), vector<float>(D, 0));

    for (size_t idx = 0; idx < data.size(); ++idx)
    {
      vector<pair<size_t, float>> matches;

      // find neighbours
      size_t nMatches = KNN.index->radiusSearch(&data[idx][0],
						radius * radius,
						matches,
						nanoflann::SearchParams());

      vector<size_t> neighbours;  // stores indices of neighbours
      if (nMatches >= k)         // found a sufficient number of neighbours
      {
	neighbours.resize(nMatches);
	for (size_t i = 0; i < nMatches; ++i)
	  neighbours[i] = matches[i].first;
      }
      else  // find k nearest neighbours
      {
	neighbours.resize(k);
	nanoflann::KNNResultSet<float> resultSet(k);
	vector<size_t> retIndices(k);
	vector<float> distsSqr(k);
	resultSet.init(&retIndices[0], &distsSqr[0]);

	KNN.index->findNeighbors(
	  resultSet, &data[idx][0], nanoflann::SearchParams(10));
	for (unsigned int i = 0; i < k; ++i)
	  neighbours[i] = retIndices[i];
      }

      // perform local PCA to find eigen-values and -vectors
      vector<float> eigenValue;
      vector<vector<float>> eigenVector;
      localPCA(data, neighbours, eigenValue, eigenVector);

      // subtract parallel motion
      denoise(data, neighbours, eigenValue, eigenVector,
	      dataOut, sigma, idx);
    }

    // update data
    data = dataOut;
  }
}
