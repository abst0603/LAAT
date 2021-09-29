#include "LAAT.ih"
#include "nanoflann/nanoflann.hpp"
#include "utils/KDTreeVectorOfVectorsAdaptor.h"
using namespace nanoflann;

// auxiliary function to find the median of a vector
size_t median(vector<size_t> &data);

/**
 * Find the neighborhood of every data point, exclude data points that have
 * less neighbors than the threshold.
 *
 * This function uses the nanoflann library to find all of the neighbors in
 * an efficient way.
 *
 * When the function is done data points will contain the indices of all of
 * their neighbors.
 *
 * @param data      vector containing the data to find the neighborhoods of
 * @param radius    the radius of each neighborhood
 * @param threshold minimum number of neighbors needed for the data to be
 *   included
 * @param neighbourhoods vector to store the neighbourhoods in
 * @return          the median number of neighbors over all data points
 */
size_t rangeSearch(vector<vector<float>> const &data,
		   float radius,
		   size_t threshold,
  		   vector<vector<unsigned int>> &neighbourhoods)
{
  // nanoflann library uses r^2 as the radius
  radius = radius * radius;

  typedef KDTreeVectorOfVectorsAdaptor<
    vector<vector<float>>, float> KDTree;
  
  KDTree KNN(data[0].size() /* dim */, data);
  KNN.index->buildIndex();

  vector<size_t> sizes(data.size());
  neighbourhoods.resize(data.size());
  for (size_t idx = 0; idx < data.size(); ++idx)
  {
    vector<pair<size_t, float>> matches;
    vector<unsigned int> &neighbourhood = neighbourhoods[idx];

    // find neighbors
    size_t nMatches = KNN.index->radiusSearch(&data[idx][0],
					      radius,
					      matches,
					      SearchParams());
    sizes[idx] = nMatches - 1;
    if (nMatches > threshold)
    {
      // store neighbors
      neighbourhood.resize(nMatches - 1);

      // add all neighbors but not the point itself
      for (size_t j = 1; j < nMatches; ++j)
	neighbourhood[j - 1] = matches[j].first;
    }
  }
  return median(sizes);
}

size_t median(vector<size_t> &data)
{
  size_t n = data.size() / 2;
  nth_element(data.begin(), data.begin() + n, data.end());
  return data[n];
}
