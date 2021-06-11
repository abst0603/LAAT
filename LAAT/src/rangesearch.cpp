#include "LAAT.ih"
#include "../lib/nanoflann/nanoflann.hpp"
#include "../lib/nanoflann/utils.h"
#include "../lib/nanoflann/KDTreeVectorOfVectorsAdaptor.h"
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
 *                  included
 * @return          the median number of neighbors over all data points
 */
size_t RangeSearch(vector<DataPoint> &data, float radius, size_t threshold)
{
  // nanoflann library uses r^2 as the radius
  radius = radius * radius;

  typedef KDTreeVectorOfVectorsAdaptor<
    vector<DataPoint>, float> KDTree;
  
  KDTree KNN(data[0].size() /* dim */, data);
  KNN.index->buildIndex();

  vector<size_t> sizes(data.size());
  for (size_t idx = 0; idx < data.size(); ++idx)
  {
    vector<pair<size_t, float>> matches;
    DataPoint &dataPoint = data[idx];

    // find neighbors
    size_t nMatches = KNN.index->radiusSearch(&dataPoint[0],
					      radius,
					      matches,
					      SearchParams());
    sizes[idx] = nMatches - 1;
    if (nMatches > threshold)
    {
      // store neighbors
      dataPoint.resizeNeighbors(nMatches - 1);
      for (size_t j = 1; j < nMatches; ++j)  // add all neighbors but not the point itself
	dataPoint.addNeighbor(j - 1, matches[j].first);
    }
    else
      dataPoint.deactivate();
  }
  return median(sizes);
}

size_t median(vector<size_t> &data)
{
  size_t n = data.size() / 2;
  nth_element(data.begin(), data.begin() + n, data.end());
  return data[n];
}
