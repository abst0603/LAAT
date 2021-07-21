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
void antSearch(vector<vector<float>> const &data,
	       vector<vector<unsigned int>> const &neighbourhoods,
	       vector<unsigned int> const &antLocations,
	       vector<vector<vector<float>>> const &eigenVectors,
	       vector<vector<float>> const &eigenValues,
	       Options const &options,
  	       vector<float> &pheromone)
{
#pragma omp parallel for
  for (unsigned int antLocation : antLocations)
  {
    vector<float> accumulatedPheromone(data.size(), 0);
    
    unsigned int current = antLocation;

    vector<float> V;
    vector<float> P;
      
    for (size_t i = 1; i < options.numberOfSteps; ++i)
    {
      /*
	From current point, select the next point from it's neighbourhood
	with probabilities as defined in formula (8).
      */
      vector<float> const &currentPoint = data[current];
      vector<unsigned int> const &neighbourhood = neighbourhoods[current];
      vector<array<float, 3>> relativeDistances(neighbourhood.size());

      V.resize(neighbourhood.size());
      P.resize(neighbourhood.size());

      float sumPheromone = 0;  // total pheromone in local neighbourhood
      for (size_t neighbourIdx = 0; neighbourIdx < neighbourhood.size(); ++neighbourIdx)
      {
	vector<float> const &neighbour = data[neighbourhood[neighbourIdx]];
	sumPheromone += pheromone[neighbourhood[neighbourIdx]];
	
	for (size_t dim = 0; dim < 3; ++dim)
	  relativeDistances[neighbourIdx][dim] = neighbour[dim] - currentPoint[dim];
	
	// normalize distance
	float norm = sqrt(
	  relativeDistances[neighbourIdx][0] * relativeDistances[neighbourIdx][0] +
	  relativeDistances[neighbourIdx][1] * relativeDistances[neighbourIdx][1] +
	  relativeDistances[neighbourIdx][2] * relativeDistances[neighbourIdx][2]);

	for (float &dim : relativeDistances[neighbourIdx])
	  dim /= norm;
      }

      /*
	Compute alignment between the data point and it's neighbours with
	the eigen-directions.
      */
      vector<array<float, 3>> w(neighbourhood.size());
      // matrix multiplication
      for (size_t neighbour = 0; neighbour < neighbourhood.size(); ++neighbour)
	for (size_t j = 0; j < 3; ++j)
	  w[neighbour][j] = abs(
	    relativeDistances[neighbour][0] * eigenVectors[current][0][j] +
	    relativeDistances[neighbour][1] * eigenVectors[current][1][j] +
	    relativeDistances[neighbour][2] * eigenVectors[current][2][j]);

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
	For each data point, compute the preference of moving to it's neighbours
	according to formula (4).
      */
      vector<float> E(neighbourhood.size());
      float sumE = 0;
      // matrix-vector multiplication
      for (size_t j = 0; j < neighbourhood.size(); ++j)
      {
	E[j] = w[j][0] * eigenValues[current][0] +
	  w[j][1] * eigenValues[current][1] +
	  w[j][2] * eigenValues[current][2];
	sumE += E[j];
      }

      float sumV = 0.0;
      /*
	calculate movement preference from the current point to all
	of it's neighbours using formula (7).
      */    
      for (size_t j = 0; j < neighbourhood.size(); ++j)
      {
	unsigned int neighbour = neighbourhood[j];
	if (!neighbourhoods[neighbour].empty())  // neighbour is active
	{
	  // calculate normalized pheromone as defined in formula (6).
	  float normalizedPheromone =
	    pheromone[neighbour] / sumPheromone;

	  /*
	    Normalize movement preferences within the neighbourhood to obtain the
	    relative preference according to formula (5).
	  */
	  float preference = E[j] / sumE;

	  // formula (7)
	  V[j] = exp(
	    options.beta * ((1 - options.kappa) * normalizedPheromone +
			    options.kappa * preference)
	    );
	  sumV += V[j];
	}
	else
	  V[j] = 0;
      }
      
      /*
	Calculate jump probabilities to all of the current point's neighbours,
	store as cummulative probabilities for easy selection.
      */
      float cummulativeProbability = 0.0;
      size_t itemp = 0;
      float randnum = (float) rand() / (RAND_MAX);
      while ((itemp + 1) < neighbourhood.size())
      {
	cummulativeProbability += V[itemp] / sumV;
	if (cummulativeProbability > randnum)
	  break;
	
	++itemp;
      }

      current = neighbourhood[itemp];

      // if there are no active neighbours, update counter for sake of warning the user
      if (neighbourhoods[current].empty())
      {
        ++lostAnts;
	break;
      }

      // keep track of pheromone to be added
      accumulatedPheromone[current] += options.p_release;
    }
    
    /*
      Update pheromone on all visited points as defined in formula (10).
    */
    for (size_t idx = 0; idx < data.size(); ++idx)
      if (accumulatedPheromone[idx])
      {
	// ensure that no two threads will ever update the same value at the same time
      #pragma omp atomic
        pheromone[idx] += accumulatedPheromone[idx];
      }
  }
}
