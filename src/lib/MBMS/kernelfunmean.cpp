#include "MBMS.ih"

void kernelFunMean(vector<vector<float>> const &data,
		   vector<size_t> const &indices,
		   vector<float> &kernelMean,
		   size_t idx,
		   float sigma2)
{
  size_t D = data[0].size();
  kernelMean.assign(D, 0);

  float sumK = 0.0;
  vector<float> K(indices.size());
  for (size_t i = 0; i < indices.size(); ++i)
  {
    float dist2 = 0.0;
    for (size_t j = 0; j < D; ++j)
      dist2 += pow(data[idx][j] - data[indices[i]][j], 2);
    K[i] = exp(-dist2 / sigma2) ;
    sumK += K[i];
    if (sumK == 0)
    {
      cout << "exp(" << -dist2 << " / " << sigma2 << ") = " << exp(-dist2 / sigma2) << '\n';
      cout << "datapoint " << idx << " ["
	   << data[idx][0] << ','
	   << data[idx][1] << ','
	   << data[idx][2] << "]\n";
      cout << "neighbour "<< i << " -> " << indices[i] << " ["
	   << data[indices[i]][0] << ','
	   << data[indices[i]][1] << ','
	   << data[indices[i]][2] << "]\n";
    }
  }

  for (size_t i = 0; i < indices.size(); ++i)
    for (size_t j = 0; j < D; ++j)
    {
      if (sumK == 0)
	cout << "zero sumK\n";
      kernelMean[j] += K[i] * data[indices[i]][j] / sumK;
    }
}
