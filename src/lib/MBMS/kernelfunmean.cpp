#include "MBMS.ih"

void kernelFunMean(vector<vector<float>> const &data,
		   vector<size_t> const &indices,
		   vector<float> &kernelMean,
		   size_t idx,
		   float sigma)
{
  size_t D = data[0].size();
  kernelMean.assign(D, 0);

  float sumK = 0.0;
  vector<float> K(indices.size());
  for (size_t i = 0; i < indices.size(); ++i)
  {
    float dist2 = 0;
    for (size_t j = 0; j < D; ++j)
      dist2 += pow(data[idx][j] - data[indices[i]][j], 2);
    K[i] = exp(-dist2 / (2 * sigma * sigma)) ;
    sumK += K[i];
  }

  for (size_t i = 0; i < indices.size(); ++i)
    for (size_t j = 0; j < D; ++j)
      kernelMean[j] += K[i] * data[indices[i]][j] / sumK;
}
