#ifndef INCLUDED_DATAPOINT_
#define INCLUDED_DATAPOINT_

#include <array>
#include <vector>
#include <cstddef>

/**
 * Auxiliary class that allows the storage of a data point, it's neighborhood,
 * preference values for each neighbor, pheromone value, and whether or not
 * the data point is active.
 */
class DataPoint: public std::array<float, 3>
{
  typedef std::array<std::array<float, 3>, 3> eigenVectors_t;
  typedef std::array<float, 3> eigenValues_t;
  
  std::vector<unsigned int> d_neighbors;
  eigenVectors_t d_eigenVectors;
  eigenValues_t d_eigenValues;
  float d_pheromone;
  bool d_active;

public:
  DataPoint();

  void resizeNeighbors(size_t size);
  void addNeighbor(size_t idx, unsigned int neighborIdx);
  std::vector<unsigned int> const &getNeighbors()    const;

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
