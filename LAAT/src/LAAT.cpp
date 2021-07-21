#include "LAAT.ih"


/**
 * Implementation of the Ant Colony algorithm proposed by Taghribi et al. (2020).
 * See Algorithm 1 in the paper for a pseudocode of this algorithm.
 *
 * When the function is done data points contain the amount of pheromone obtained 
 * in the search.
 *
 * In this function and all of the functions evoked by this one we will refer to the
 * formulas as described by Taghribi et al. (2020).
 *
 * Reference:
 * Taghribi, A., Bunte, K., Smith, R., Shin, J., Mastropietro, M., Peletier, R. F., 
 * & Tino, P. (2020). LAAT: Locally Aligned Ant Technique for detecting manifolds of
 * varying density. arXiv preprint arXiv:2009.08326.
 *
 * @param data    vector containing all the data points to search
 * @param options struct containing the values of the hyper-parameters
 */
vector<float> LocallyAlignedAntTechnique(vector<vector<float>> const &data,
					 Options options)
{
  // set amount of lost ants to zero, only used to provide warnings
  lostAnts = 0;
  
  omp_set_num_threads(8);
  Eigen::initParallel();

  vector<vector<unsigned int>> neighbourhoods;
  vector<vector<vector<float>>> eigenVectors;
  vector<vector<float>> eigenValues;
  
  // preprocess
  cout << "Preprocessing...\n";
  size_t medianvalue =
    preprocess(data, options.threshold, options.radius,
	       neighbourhoods, eigenVectors, eigenValues);
  
  // group the data into a corresponding sector for each ant
  vector<unsigned int> gd = groupdata(data, options);
  
  // iterative step
  cout << "\nPerforming ant search...\n";
  // initial locations of all ants at each iteration
  vector<unsigned int> antLocations(
    options.numberOfAntsX * options.numberOfAntsY * options.numberOfAntsZ);

  // initialize pheromone value for each data point to 1
  vector<float> pheromone(data.size(), 1.0);
  
  initializeProgressBar(options.numberOfIterations / 2);
  for (size_t loop = 0; loop < options.numberOfIterations; ++loop)
  {
    // place ants on random points as defined in formula (9)
    initializeAnts(neighbourhoods, gd, medianvalue, antLocations);

    antSearch(data, neighbourhoods, antLocations,
	      eigenVectors, eigenValues, options, pheromone);

    // apply evaporation of pheromone as defined in formula (1)
    evaporatePheromone(pheromone, options);
    
    updateProgressBar(loop);
  }
  completeProgressBar();

  // possibly warn user
  if (lostAnts)
    printWarning(antLocations.size() * options.numberOfIterations * options.numberOfSteps);

  return pheromone;
}
