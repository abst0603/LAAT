#include "LAAT.ih"

/**
 * Perform preprocessing steps as defined in Algorithm 1.
 *
 * When the function is done relative preferences have been calculated for
 * each data point and it's neighbours.
 *
 * @param data vector containing the data points to preprocess
 * @param radius the radius of the neighbourhood for each data point
 * @param threshold amount of neighbours a data point should at least
 *   have to be included in the search
 * @param neighbourhoods vector to store the neighbourhoods in
 * @param eigenVectors vector to store the eigen vectors in
 * @param eigenValues vector to store the eigen values in
 * @return median neighbourhood size of all data points
 */
size_t preprocess(vector<vector<float>> const &data,
		  size_t radius,
		  float threshold,
		  vector<vector<unsigned int>> &neighbourhoods,
		  vector<vector<vector<float>>> &eigenVectors,
		  vector<vector<float>> &eigenValues)
{
  // find the local neighbourhood of all data points.
  size_t medianvalue = rangeSearch(data, radius, threshold, neighbourhoods);
  
  /*
    Compute Eigen-vectors and -values. Normalize eigen-values according
    to formula (3).
  */
  eigenVectors.resize(data.size());
  eigenValues.resize(data.size());
  for (size_t i = 0; i < data.size(); ++i)
  {
    vector<unsigned int> const &neighbourhood = neighbourhoods[i];

    if (!neighbourhood.empty())
    {
      Eigen::MatrixXf X(neighbourhood.size(), 3);
      Eigen::RowVectorXf meanvec(3);

      for (size_t j = 0; j < neighbourhood.size(); ++j)
	for (size_t dim = 0; dim < 3; ++dim)
	  X(j, dim) = data[neighbourhood[j]][dim];

      for (size_t dim = 0; dim < 3; ++dim)
	meanvec(dim) = X.middleCols<1>(dim).mean();

      X = X.rowwise() - meanvec;
      meanvec.resize(0);  // destructor

      Eigen::BDCSVD<Eigen::MatrixXf> svd(X, Eigen::ComputeThinV);

      X.resize(0,0); // this is the destructor
      Eigen::RowVectorXf svalue(3);
      svalue = svd.singularValues();
      Eigen::Matrix3f svector;
      svector = svd.matrixV();          // eigen vectors are in columns
      svalue = svalue.array().pow(2);   // second power of singular values
      svalue = svalue.array() / svalue.sum();  // normalize eigen values

      eigenValues[i].resize(3);
      for (size_t d = 0; d < 3; ++d)
	eigenValues[i][d] = svalue[d];
      svalue.resize(0);

      eigenVectors[i].resize(3, vector<float>(3));
      for (size_t j = 0; j < 3; ++j)
	for (size_t k = 0; k < 3; ++k)
	  eigenVectors[i][j][k] = svector(j, k);
    }
  }

  return medianvalue;
}

