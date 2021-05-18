#include "MBMS.ih"

void localPCA(vector<vector<float>> &data,
              vector<unsigned int> &indices,
              vector<float> &v,
              vector<vector<float>> &tmpvec)
{
  unsigned int D = data[0].size();

  Eigen::MatrixXf X(indices.size(), D);
  Eigen::RowVectorXf meanvec(D);

  for (unsigned int j = 0; j < indices.size(); ++j)
    for (unsigned int d = 0; d < D; ++d)
      X(j, d) = data[indices[j]][d];

  for (unsigned int d = 0; d < D; ++d)
    meanvec(d) = X.middleCols<1>(d).mean();

  X = X.rowwise() - meanvec;
  meanvec.resize(0);//This is the destructor of meanvec

  Eigen::BDCSVD<Eigen::MatrixXf> svd(X, Eigen::ComputeThinV);
  X.resize(0, 0);//This is the destructor of X

  Eigen::RowVectorXf svalue(D);
  svalue = svd.singularValues();
  Eigen::MatrixXf svector(D, D);
  svector = svd.matrixV();           // eigenvector are in columns
  svalue = svalue.array().pow(2);    // eigen values are the second power of singular values
  svalue = svalue.array() / svalue.sum();  //Normalize eigen values

  for (unsigned int d = 0; d < D; ++d)
    v[d] = svalue(d);
  svalue.resize(0);

  for (unsigned int j = 0; j < D; ++j)
    for (unsigned int k = 0; k < D; ++k)
      tmpvec[j][k] = svector(j, k);
  svector.resize(0, 0);
}
