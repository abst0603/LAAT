#include "LAAT.ih"

/**
 * Apply evaporation of pheromone as defined in formula (1) to all data points,
 * do no let pheromone rise above the upper limit or fall below the lower limit.
 *
 * @param data    vector containing the data points to apply evaporation of 
 *                pheromones to.
 * @param options struct containing the values of the hyper-parameters
 */
void evaporatePheromone(vector<float> &pheromone, Options const &options)
{
  for (float &pher : pheromone)
  {
    float newPheromone = (1 - options.EvapRate) * pher;
    
    if (newPheromone < options.lowerlimit)
      newPheromone = options.lowerlimit;
    
    if(newPheromone > options.upperlimit)
      newPheromone = options.upperlimit;

    pher = newPheromone;
  }
}
