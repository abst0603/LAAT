#include "LAAT.ih"


/**
 * Implementation of the Ant Colony algorithm proposed by Taghribi et al.
 * (2020).
 * See Algorithm 1 in the paper for a pseudocode of this algorithm.
 *
 * In this function and all of the functions evoked by this one we will refer
 * to the formulas as described by Taghribi et al. (2020).
 *
 * Reference:
 * Taghribi, A., Bunte, K., Smith, R., Shin, J., Mastropietro, M., Peletier,
 * R. F., & Tino, P. (2020). LAAT: Locally Aligned Ant Technique for detecting
 * manifolds of varying density. arXiv preprint arXiv:2009.08326.
 *
 * @param data vector containing all the data points to search
 * @param numberOfAntsX number of ants to be distributed along the `x` 
 *   dimension
 * @param numberOfAntsY number of ants to be distributed along the `y`
 *   dimension
 * @param numberOfAntsZ number of ants to be distributed along the `z`
 *   dimension
 * @param numberOfIterations number of tiems to apply ant search
 * @param numberOfSteps number of steps to run each ant at each iteration
 * @param threshold minimum number of nearest neighbours needed for a datapoint
 *   to be considered
 * @param radius radius within which to search for nearest neighbours
 * @param beta the inverse temperature as used in formula (8)
 * @param kappa tuning parameter for the relative importance of the influence
 * of the alignment and pheromone terms as used in formula (7)
 * @param p_release amount of pheromone to release on a data point at each 
 *   visit
 * @param evapRate rate at which to evaporate pheromone after each application
 *   of ant search
 * @param lowerlimit lower limit on the amount of pheromone of a data point
 * @param upperlimit upper limit on the amount of pheromone of a data point
 * @return vector containing the amount of pheromone for each data point after
 *   the application of LAAT
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
  cout << "Running the Locally Aligned Ant Technique algorithm on "
       << data.size() << " data points\n\n";
  // set amount of lost ants to zero, only used to provide warnings
  lostAnts = 0;

  // initialize parallelization with a default of 8 threads
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
  vector<unsigned int> gd =
    groupdata(data, numberOfAntsX, numberOfAntsY, numberOfAntsZ);
  
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

  cout << "Locally Aligned Ant Technique algorithm completed\n\n";
  return pheromone;
}
