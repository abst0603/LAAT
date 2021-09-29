#include "LAAT.ih"

/**
 * Apply evaporation of pheromone as defined in formula (1) to all data 
 * points, do no let pheromone rise above the upper limit or fall below the
 * lower limit.
 *
 * @param data    vector containing the data points to apply evaporation of 
 *                pheromones to.
 * @param evapRate rate at which to evaporate pheromone after each application
 *   of ant search
 * @param lowerlimit lower limit on the amount of pheromone of a data point
 * @param upperlimit upper limit on the amount of pheromone of a data point
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
