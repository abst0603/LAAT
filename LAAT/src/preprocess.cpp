#include "LAAT.ih"

/**
 * Perform preprocessing steps as defined in Algorithm 1.
 *
 * When the function is done relative preferences have been calculated for
 * each data point and it's neighbors.
 *
 * @param data vector containing the data points to preprocess
 * @param threshold amount of neighbors a data point should at least
 *        have to be included in the search
 * @param radius the radius of the neighborhood for each data point
 * @return median neighborhood size of all data points
 */
size_t preprocess(vector<DataPoint> &data,
		  size_t radius,
		  float threshold)
{
  // find the local neighborhood of all data points.
  size_t medianvalue = RangeSearch(data, radius, threshold);
  
  /*
    Compute Eigen-vectors and -values. Normalize eigen-values according
    to formula (3).
  */
  array<float, 3> eigenValues;
  array<array<float, 3>, 3> eigenVectors;
  for (size_t i = 0; i < data.size(); ++i)
  {
    DataPoint &dataPoint = data[i];
    if (dataPoint.isActive())
    {
      Eigen::MatrixXf X(dataPoint.getNeighbors().size(), 3);
      Eigen::RowVectorXf meanvec(3);

      for (size_t j = 0; j < dataPoint.getNeighbors().size(); ++j)
	for (size_t dim = 0; dim < 3; ++dim)
	  X(j, dim) = data[dataPoint.getNeighbors()[j]][dim];

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
      
      eigenValues = {{svalue(0), svalue(1), svalue(2)}};
      svalue.resize(0);

      for (size_t j = 0; j < 3; ++j)
	for (size_t k = 0; k < 3; ++k)
	  eigenVectors[j][k] = svector(j, k);

      dataPoint.setEigenValues(eigenValues);
      dataPoint.setEigenVectors(eigenVectors);
    }
  }

  return medianvalue;
}

