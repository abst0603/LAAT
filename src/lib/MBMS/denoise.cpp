#include "MBMS.ih"

/**
 * Shift the data points by subtracting the parallel motion.
 *
 * @param data vector containing all the data points
 * @param indices indices of the neighbours of the current point
 * @param evalue eigen values of the current point
 * @param evector eigen vectors of the current point
 * @param updatedData vector to store the updated data in
 * @param sigma scale of the shift
 * @param idx idx of the current point
 */
void denoise(vector<vector<float>> const &data,
             vector<size_t> const &indices,
             vector<float> const &evalue,
             vector<vector<float>> const &evector,
	     vector<vector<float>> &updatedData,
	     float sigma,
	     size_t idx)
{
  unsigned int D = data[0].size();

  // computing the dimensionality
  float error;
  size_t dim;
  dimensionalityApproximation(evalue, dim, error);

  // Projection matrix: (I - UU^T)
  if (error != 0)
  {
    vector<vector<float>> tmpproj(D, vector<float>(D, 0));
    for (size_t j = 0; j < D; ++j)
    {
      tmpproj[j][j] = 1; // I
      for (size_t k = 0; k < D; ++k)
	for (size_t t = 0; t <= dim; ++t)
	  tmpproj[j][k] -= (evector[j][t] * evector[k][t]);
    }

    // kernel function
    vector<float> kernelMean(D);
    kernelFunMean(data, indices, kernelMean, idx, 2 * sigma * sigma);

    // updating the data set
    for (size_t j = 0; j < D; ++j)
    {
      float deltaX = 0.0;
      for (size_t k = 0; k < D; ++k)
	deltaX += tmpproj[j][k] * (kernelMean[k] - data[idx][k]);
      updatedData[idx][j] = data[idx][j] + deltaX;
    }
  }
  else
    updatedData[idx] = data[idx];
}
