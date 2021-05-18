#include "MBMS.ih"

void denoise(vector<vector<float>> &data,
             vector<unsigned int> &indices,
             vector<float> &evalue,
             vector<vector<float>> &evector,
             float sigma2, unsigned int idx, vector<vector<float>> &dataupdate)
{
  unsigned int D = data[0].size();

  float error = 0 ;
  int dim = 0 ;

  vector<float> kernelmean(D);
  vector<vector<float>> tmpproj;
  float deltaX = 0;
  tmpproj.resize(D, vector<float>(D, 0));

  // ****** computing the dimensionality
  dimensionalityApproximation(evalue, dim, error);

  // ****** Projection matrix: (I - UU^T)
  if (error != 0)
  {
    //assiging values to an eye matrix
    tmpproj.assign(D, vector<float>(D, 0));
    for (unsigned int j = 0; j < D; ++j)
    {
      tmpproj[j][j] = 1; //I
      for (unsigned int k = 0; k < D; ++k)
	for (int t = 0; t <= dim; ++t)
	  tmpproj[j][k] -= (evector[j][t] * evector[k][t]);
    }

    // ****** Kernel function
    kernelFunMean(data, indices, kernelmean, idx, sigma2);

    // ****** updating the data set
    for (unsigned int j = 0; j < D; ++j)
    {
      deltaX = 0;
      for (unsigned int k = 0; k < D; ++k)
	deltaX += tmpproj[j][k] * (-data[idx][k] + kernelmean[k]);
      dataupdate[idx][j] = data[idx][j] + deltaX;
    }
  }
  else
    for (unsigned int j=0; j<D; j++)
      dataupdate[idx][j] = data[idx][j];
}
