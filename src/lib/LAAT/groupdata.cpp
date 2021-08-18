#include "LAAT.ih"

/**
 * Group the data into one sector for each ant, group each data point
 * into it's corresponding sector.
 *
 * @param data    vector containing the data to group
 * @param options struct containing the values of the hyper-parameters
 * @return        vector containing the sector number for each data point
 */
vector<unsigned int> groupdata(vector<vector<float>> const &data,
			       size_t numberOfAntsX,
			       size_t numberOfAntsY,
			       size_t numberOfAntsZ)
{
  // compute extremes of the data
  float maxx = -numeric_limits<float>::infinity();
  float minx = numeric_limits<float>::infinity();
  float maxy = -numeric_limits<float>::infinity();
  float miny = numeric_limits<float>::infinity();
  float maxz = -numeric_limits<float>::infinity();
  float minz = numeric_limits<float>::infinity();
  for (size_t i = 0; i < data.size(); ++i)
  {
    if (data[i][0] > maxx)
      maxx = data[i][0];
    if (data[i][0] < minx)
      minx = data[i][0];
    if (data[i][1] > maxy)
      maxy = data[i][1];
    if (data[i][1] < miny)
      miny = data[i][1];
    if (data[i][2] > maxz)
      maxz = data[i][2];
    if (data[i][2] < minz)
      minz = data[i][2];
  }

  // create limits to separate sectors
  vector<float> xlims(numberOfAntsX, 0);
  vector<float> ylims(numberOfAntsY, 0);
  vector<float> zlims(numberOfAntsZ, 0);
  
  for (size_t i = 0; i < numberOfAntsX; ++i)
    xlims[i] = minx + (i + 1) * (maxx - minx) / numberOfAntsX;

  for (size_t i = 0; i < numberOfAntsY; ++i)
    ylims[i] = miny + (i+1)*(maxy - miny)/numberOfAntsY;

  for (size_t i = 0; i < numberOfAntsZ; ++i)
    zlims[i] = minz + (i+1)*(maxz - minz)/numberOfAntsZ;

  /*
    Because of floating point percision several points might be bigger than 
    the highest threshold, Thus to make sure all points will belong to at 
    least one group we increase the final limit by an epsilon
  */
  xlims[numberOfAntsX - 1] += 0.0001;
  ylims[numberOfAntsY - 1] += 0.0001;
  zlims[numberOfAntsZ - 1] += 0.0001;

  // assign sector number to each data point
  vector<unsigned int> groupedData(data.size());
  for (size_t i = 0; i < data.size(); ++i)
  {
    for (size_t j = 0; j < numberOfAntsX; ++j)
      if (data[i][0] < xlims[j])
      {
	groupedData[i] = j;
	break;
      }
    
    for (size_t j = 0; j < numberOfAntsY; ++j)
      if (data[i][1] < ylims[j])
      {
	groupedData[i] = groupedData[i] + j * numberOfAntsX;
	break;
      }

    for (size_t j = 0; j < numberOfAntsZ; ++j)
      if (data[i][2] < zlims[j])
      {
	groupedData[i] =
	  groupedData[i] + j * numberOfAntsX * numberOfAntsY;
	break;
      }
  }
  return groupedData;
}
