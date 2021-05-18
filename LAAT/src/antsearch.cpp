#include "LAAT.ih"

/**
 * Perform ant search for all ants.
 *
 * When the function is done there is an increase of pheromone on data 
 * points dependent on how many times they were visited by ants
 *
 * This implementation uses OpenMP to perform ant search in parallel for
 * the different ants. Synchronization in the from of atomic operations
 * is used to ensure thread safety.
 *
 * @param data vector containing the data points to search on
 * @param ants vector containing for each ant the initial point to start
 *   searching from
 * @param options struct containing the values of the hyper-parameters
 */
void antSearch(vector<DataPoint> &data,
	       vector<DataPoint *> const &ants,
	       Options const &options)
{
#pragma omp parallel for
  for (size_t ant = 0; ant < ants.size(); ++ant)
  {
    vector<float> accumulatedPheromone(data.size(), 0);
    
    DataPoint *nextPoint = ants[ant];

    vector<float> V;
    vector<float> P;
      
    for (size_t i = 1; i < options.numberOfSteps; ++i)
    {
      /*
	From current point, select the next point from it's neighborhood
	with probabilities as defined in formula (8).
      */
      DataPoint &currentPoint = *nextPoint;
      size_t neighborhoodSize = currentPoint.getNeighbors().size();
      vector<array<float, 3>> relativeDistances(neighborhoodSize);

      V.resize(neighborhoodSize);
      P.resize(neighborhoodSize);

      float sumPheromone = 0;  // total pheromone in local neighborhood
      for (size_t neighborIdx = 0; neighborIdx < neighborhoodSize; ++neighborIdx)
      {
	DataPoint &neighbor = data[currentPoint.getNeighbors()[neighborIdx]];
	sumPheromone += neighbor.getPheromone();
	
	for (size_t dim = 0; dim < 3; ++dim)
	  relativeDistances[neighborIdx][dim] = neighbor[dim] - currentPoint[dim];
	
	// normalize distance
	float norm = sqrt(
	  relativeDistances[neighborIdx][0] * relativeDistances[neighborIdx][0] +
	  relativeDistances[neighborIdx][1] * relativeDistances[neighborIdx][1] +
	  relativeDistances[neighborIdx][2] * relativeDistances[neighborIdx][2]);

	for (float &dim : relativeDistances[neighborIdx])
	  dim /= norm;
      }

      /*
	Compute alignment between the data point and it's neighbors with
	the eigen-directions.
      */
      vector<array<float, 3>> w(neighborhoodSize);
      // matrix multiplication
      for (size_t neighbor = 0; neighbor < neighborhoodSize; ++neighbor)
	for (size_t j = 0; j < 3; ++j)
	  w[neighbor][j] = abs(
	    relativeDistances[neighbor][0] * currentPoint.getEigenVectors()[0][j] +
	    relativeDistances[neighbor][1] * currentPoint.getEigenVectors()[1][j] +
	    relativeDistances[neighbor][2] * currentPoint.getEigenVectors()[2][j]);

      /*
	Normalize alignment values to obtain relative weighting of the
	alignment according to formula (2).
      */
      for (array<float, 3> &element : w)
      {
	float sum = element[0] + element[1] + element[2];
        
	for (float &dim : element)
	  dim /= sum;
      }

      /*
	For each data point, compute the preference of moving to it's neighbors
	according to formula (4).
      */
      vector<float> E(neighborhoodSize);
      float sumE = 0;
      // matrix-vector multiplication
      for (size_t j = 0; j < neighborhoodSize; ++j)
      {
	E[j] = w[j][0] * currentPoint.getEigenValues()[0] +
	  w[j][1] * currentPoint.getEigenValues()[1] +
	  w[j][2] * currentPoint.getEigenValues()[2];
	sumE += E[j];
      }

      float sumV = 0.0;
      /*
	calculate movement preference from the current point to all
	of it's neighbors using formula (7).
      */    
      for (size_t j = 0; j < neighborhoodSize; ++j)
      {
	DataPoint &neighbor = data[currentPoint.getNeighbors()[j]];
	if (neighbor.isActive())
	{
	  // calculate normalized pheromone as defined in formula (6).
	  float pheromone = neighbor.getPheromone() / sumPheromone;

	  /*
	    Normalize movement preferences within the neighborhood to obtain the
	    relative preference according to formula (5).
	  */
	  float preference = E[j] / sumE;

	  // formula (7)
	  V[j] = exp(
	    options.beta * ((1 - options.kappa) * pheromone +
			    options.kappa * preference)
	    );
	  sumV += V[j];
	}
	else
	  V[j] = 0;
      }
      
      /*
	Calculate jump probabilities to all of the current point's neighbors,
	store as cummulative probabilities for easy selection.
      */
      float cummulativeProbability = 0.0;
      size_t itemp = 0;
      float randnum = (float) rand() / (RAND_MAX);
      while ((itemp + 1) < neighborhoodSize)
      {
	cummulativeProbability += V[itemp] / sumV;
	if (cummulativeProbability > randnum)
	  break;
	
	++itemp;
      }

      // store pointer to next data point to visit
      nextPoint = &data[currentPoint.getNeighbors()[itemp]];

      // if there are no active neighbors, update counter for sake of warning the user
      if (!nextPoint->isActive())
      {
        ++lostAnts;
	break;
      }

      // keep track of pheromone to be added
      accumulatedPheromone[currentPoint.getNeighbors()[itemp]] += options.p_release;
    }
    
    /*
      Update pheromone on all visited points as defined in formula (10).
    */
    for (size_t idx = 0; idx < data.size(); ++idx)
      if (accumulatedPheromone[idx])
      {
	// ensure that no two threads will every update the same value at the same time
      #pragma omp atomic
	data[idx].pheromone() += accumulatedPheromone[idx];
      }
  }
}
