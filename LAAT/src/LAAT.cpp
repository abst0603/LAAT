#include "LAAT.ih"


/**
 * Implementation of the Ant Colony algorithm proposed by Taghribi et al. (2020).
 * See Algorithm 1 in the paper for a pseudocode of this algorithm.
 *
 * When the function is done data points contain the amount of pheromone obtained 
 * in the search.
 *
 * In this function and all of the function evoked by this one we will refer to the
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
void LocallyAlignedAntTechnique(vector<DataPoint> &data, Options options)
{
  // preprocess
  cout << "Preprocessing...\n";
  size_t medianvalue =
    preprocess(data, options.threshold, options.radius);
  
  // group the data into a corresponding sector for each ant
  vector<unsigned int> gd = groupdata(data, options);
  
  // iterative step
  cout << "\nPerforming ant search...\n";  
  vector<DataPoint *> ants(
    options.numberOfAntsX * options.numberOfAntsY * options.numberOfAntsZ);

  initializeProgressBar(options.numberOfIterations / 2);
  for (size_t loop = 0; loop < options.numberOfIterations; ++loop)
  {
    // place ants on random points as defined in formula (9)
    initializeAnts(data, ants, gd, medianvalue);

    antSearch(data, ants, options);

    // apply evaporation of pheromone as defined in formula (1)
    evaporatePheromone(data, options);
    
    updateProgressBar(loop);
  }
  completeProgressBar();
}
