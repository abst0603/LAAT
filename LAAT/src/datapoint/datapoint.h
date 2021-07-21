#ifndef INCLUDED_DATAPOINT_
#define INCLUDED_DATAPOINT_

#include <array>
#include <vector>
#include <cstddef>

/**
 * Auxiliary class that allows the storage of a data point, it's neighbourhood,
 * preference values for each neighbour, pheromone value, and whether or not
 * the data point is active.
 */
class DataPoint: public std::array<float, 3>
{
  typedef std::array<std::array<float, 3>, 3> eigenVectors_t;
  typedef std::array<float, 3> eigenValues_t;
  
  std::vector<unsigned int> d_neighbours;
  eigenVectors_t d_eigenVectors;
  eigenValues_t d_eigenValues;
  float d_pheromone;
  bool d_active;

public:
  DataPoint();

  void resizeNeighbours(size_t size);
  void addNeighbour(size_t idx, unsigned int neighbourIdx);
  std::vector<unsigned int> const &getNeighbours()    const;

  void setEigenVectors(eigenVectors_t &eigenVectors);
  eigenVectors_t const &getEigenVectors()    const;

  void setEigenValues(eigenValues_t &eigenValues);
  eigenValues_t const &getEigenValues()    const;
  
  void setPheromone(float pheromone);
  float getPheromone()    const;

  float &pheromone();
  void addPheromone(float pheromone);

  void deactivate();
  bool isActive()    const;
};

// include inline member function definitions
#include "memberfunctions.h"

#endif
