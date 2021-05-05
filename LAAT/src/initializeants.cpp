#include "LAAT.ih"

/**
 * choose the initial points so that their neighborhood size is bigger
 * or equal to the median number of neighbors.
 *
 * @param data        vector containing all data points
 * @param ants        vector of ants to choose the initial points for
 * @param gd          vector containing the sector number for each data point
 * @param medianvalue the median number of neighbors over all data points
 */
void initializeAnts(vector<DataPoint> &data,
		    vector<DataPoint *> &ants,
		    vector<unsigned int> const &gd,
		    size_t medianvalue)
{
  // clear ants' locations
  ants.assign(ants.size(), NULL);
  size_t cnt = 0;
  size_t initializedAnts = 0;
  /*
    when count reaches the size of the data we assume there
    are some sectors without suitable datapoints. We then
    initialize the ants corresponding to this region with
    random suitable datapoints from other sectors.
  */
  while (initializedAnts < ants.size() && cnt < data.size())
  {
    size_t currentPos = rand() % data.size();

    if (data[currentPos].getNeighbors().size() > medianvalue
	&& ants[gd[currentPos]] == NULL)
    {
      ants[gd[currentPos]] = &data[currentPos];
      ++initializedAnts;
    }

    ++cnt;
  }

  if (initializedAnts != ants.size())
    // choose random points for uninitialized ants
    for (size_t j = 0; j < ants.size(); ++j)
      if (ants[j] == NULL)
	do
	  ants[j] = &data[rand() % data.size()];
	while (ants[j]->getNeighbors().size() <= medianvalue);
}
