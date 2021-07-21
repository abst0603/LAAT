#include "datapoint.ih"

/**
 * Constructor of DataPoint.
 */
DataPoint::DataPoint()
  :
  d_neighbours(0),
  d_eigenVectors(),
  d_eigenValues(),
  d_pheromone(1.0),
  d_active(true)
{}
