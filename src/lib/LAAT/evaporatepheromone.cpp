#include "LAAT.ih"

/**
 * Apply evaporation of pheromone as defined in formula (1) to all data points,
 * do no let pheromone rise above the upper limit or fall below the lower limit.
 *
 * @param data    vector containing the data points to apply evaporation of 
 *                pheromones to.
 * @param options struct containing the values of the hyper-parameters
 */
void evaporatePheromone(vector<float> &pheromone, float evapRate,
			float lowerlimit, float upperlimit)
{
  for (float &pher : pheromone)
  {
    float newPheromone = (1 - evapRate) * pher;
    
    if (newPheromone < lowerlimit)
      newPheromone = lowerlimit;
    
    if(newPheromone > upperlimit)
      newPheromone = upperlimit;

    pher = newPheromone;
  }
}
