#include "LAAT.ih"

/**
 * Group the data into one sector for each ant, group each data point
 * into it's corresponding sector.
 *
 * @param data    vector containing the data to group
 * @param options struct containing the values of the hyper-parameters
 * @return        vector containing the sector number for each data point
 */
vector<unsigned int> groupdata(vector<DataPoint> const &data,
			       Options const &options)
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
  vector<float> xlims(options.numberOfAntsX, 0);
  vector<float> ylims(options.numberOfAntsY, 0);
  vector<float> zlims(options.numberOfAntsZ, 0);
  
  for (size_t i = 0; i < options.numberOfAntsX; ++i)
    xlims[i] = minx + (i + 1) * (maxx - minx) / options.numberOfAntsX;

  for (size_t i = 0; i < options.numberOfAntsY; ++i)
    ylims[i] = miny + (i+1)*(maxy - miny)/options.numberOfAntsY;

  for (size_t i = 0; i < options.numberOfAntsZ; ++i)
    zlims[i] = minz + (i+1)*(maxz - minz)/options.numberOfAntsZ;

  /*
    Because of floating point percision several points might be bigger than 
    the highest threshold, Thus to make sure all points will belong to at 
    least one group we increase the final limit by an epsilon
  */
  xlims[options.numberOfAntsX - 1] += 0.0001;
  ylims[options.numberOfAntsY - 1] += 0.0001;
  zlims[options.numberOfAntsZ - 1] += 0.0001;

  // assign sector number to each data point
  vector<unsigned int> groupedData(data.size());
  for (size_t i = 0; i < data.size(); ++i)
  {
    for (size_t j = 0; j < options.numberOfAntsX; ++j)
      if (data[i][0] < xlims[j])
      {
	groupedData[i] = j;
	break;
      }
    
    for (size_t j = 0; j < options.numberOfAntsY; ++j)
      if (data[i][1] < ylims[j])
      {
	groupedData[i] = groupedData[i] + j * options.numberOfAntsX;
	break;
      }

    for (size_t j = 0; j < options.numberOfAntsZ; ++j)
      if (data[i][2] < zlims[j])
      {
	groupedData[i] =
	  groupedData[i] + j * options.numberOfAntsX * options.numberOfAntsY;
	break;
      }
  }
  return groupedData;
}
