/*
  All inline member functions of the class DataPoint.
 */

inline void DataPoint::resizeNeighbours(size_t size)
{
  d_neighbours.resize(size);
}

inline std::vector<unsigned int> const &DataPoint::getNeighbours()    const
{
  return d_neighbours;
}

inline void DataPoint::addNeighbour(size_t idx, unsigned int neighbourIdx)
{
  d_neighbours[idx] = neighbourIdx;
}

inline void DataPoint::setEigenVectors(eigenVectors_t &eigenVectors)
{
  d_eigenVectors = eigenVectors;
}

inline DataPoint::eigenVectors_t const &DataPoint::getEigenVectors() const
{
  return d_eigenVectors;
}

inline void DataPoint::setEigenValues(eigenValues_t &eigenValues)
{
  d_eigenValues = eigenValues;
}

inline DataPoint::eigenValues_t const &DataPoint::getEigenValues()    const
{
  return d_eigenValues;
}

inline void DataPoint::setPheromone(float pheromone)
{
  d_pheromone = pheromone;
}

inline float DataPoint::getPheromone()    const
{
  return d_pheromone;
}

inline float &DataPoint::pheromone()
{
  return d_pheromone;
}

inline void DataPoint::addPheromone(float pheromone)
{
  d_pheromone += pheromone;
}

inline bool DataPoint::isActive()    const
{
  return d_active;
}

inline void DataPoint::deactivate()
{
  d_active = false;
}
