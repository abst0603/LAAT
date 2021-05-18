#include "MBMS.ih"

void kernelFunMean(vector<vector<float>> &data,
		   vector<unsigned int> &indices,
		   vector<float> &kernelmean, int idx, float sigma2)
{
  float dist2 , sumK=0;
  vector<float> K(indices.size());
  unsigned int D = data[0].size();

  kernelmean.assign(D, 0);

  for (unsigned int i = 0; i < indices.size(); ++i)
  {
    dist2 = 0;
    for (unsigned int j = 0; j < D; ++j)
      dist2 += pow(data[idx][j] - data[indices[i]][j], 2);
    K[i] = exp(-dist2 / sigma2) ;
    sumK += K[i];
  }

  for (unsigned int i = 0; i < indices.size(); ++i)
    for (unsigned int j = 0; j < D; ++j)
      kernelmean[j] += K[i] * data[indices[i]][j] / sumK;
}
