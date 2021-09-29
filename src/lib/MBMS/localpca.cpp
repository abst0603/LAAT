#include "MBMS.ih"

/**
 * Perform local PCA using the given neighbourhood to find eigen-values and
 * -vectors.
 *
 * @param data vector containing the data points
 * @param indices indices of the neighbourhood of points
 * @param eigenValue will store the eigen values
 * @param eigenVector will store the eigen vectors
 */
void localPCA(vector<vector<float>> const &data,
              vector<size_t> const &indices,
              vector<float> &eigenValue,
              vector<vector<float>> &eigenVector)
{
  unsigned int D = data[0].size();

  eigenValue.resize(D);
  eigenVector.assign(D, vector<float>(D, 0));
  
  Eigen::MatrixXf X(indices.size(), D);
  Eigen::RowVectorXf meanvec(D);

  for (size_t j = 0; j < indices.size(); ++j)
    for (size_t dim = 0; dim < D; ++dim)
      X(j, dim) = data[indices[j]][dim];

  for (size_t dim = 0; dim < D; ++dim)
    meanvec(dim) = X.middleCols<1>(dim).mean();

  X = X.rowwise() - meanvec;
  meanvec.resize(0);  // destructor

  Eigen::BDCSVD<Eigen::MatrixXf> svd(X, Eigen::ComputeThinV);
  X.resize(0, 0);     // destructor
  
  Eigen::RowVectorXf svalue(D);
  svalue = svd.singularValues();
  Eigen::MatrixXf svector(D, D);
  svector = svd.matrixV();          // eigen vectors are in columns
  svalue = svalue.array().pow(2);   // second power of singular values
  svalue = svalue.array() / svalue.sum();  // normalize eigen values

  for (unsigned int dim = 0; dim < D; ++dim)
    eigenValue[dim] = svalue(dim);

  for (unsigned int j = 0; j < D; ++j)
    for (unsigned int k = 0; k < D; ++k)
      eigenVector[j][k] = svector(j, k);
}
