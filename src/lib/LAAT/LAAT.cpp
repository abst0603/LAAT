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
vector<float> LocallyAlignedAntTechnique(
  vector<vector<float>> const &data,
  size_t numberOfAntsX,
  size_t numberOfAntsY,
  size_t numberOfAntsZ,
  size_t numberOfIterations,
  size_t numberOfSteps,
  size_t threshold,
  float radius,
  float beta,
  float kappa,
  float p_release,
  float evapRate,
  float lowerlimit,
  float upperlimit)
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
    preprocess(data, threshold, radius,
	       neighbourhoods, eigenVectors, eigenValues);
  
  // group the data into a corresponding sector for each ant
  vector<unsigned int> gd = groupdata(data, numberOfAntsX, numberOfAntsY, numberOfAntsZ);
  
  // iterative step
  cout << "\nPerforming ant search...\n";
  // initial locations of all ants at each iteration
  vector<unsigned int> antLocations(
    numberOfAntsX * numberOfAntsY * numberOfAntsZ);

  // initialize pheromone value for each data point to 1
  vector<float> pheromone(data.size(), 1.0);
  
  initializeProgressBar(numberOfIterations / 2);
  for (size_t loop = 0; loop < numberOfIterations; ++loop)
  {
    // place ants on random points as defined in formula (9)
    initializeAnts(neighbourhoods, gd, medianvalue, antLocations);

    // perform ant search, spread pheromone on visited data points
    antSearch(data,
	      neighbourhoods,
	      antLocations,
	      eigenVectors,
	      eigenValues,
	      numberOfSteps,
	      beta,
	      kappa,
	      p_release,
	      pheromone);

    // apply evaporation of pheromone as defined in formula (1)
    evaporatePheromone(pheromone, evapRate, lowerlimit, upperlimit);
    
    updateProgressBar(loop);
  }
  completeProgressBar();

  // possibly warn user
  if (lostAnts)
    printWarning(antLocations.size() * numberOfIterations * numberOfSteps);

  return pheromone;
}
