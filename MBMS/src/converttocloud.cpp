#include "MBMS.ih"

void convertTocloud(PointCloud<float> &point, vector<vector<float>> &data)
{
  // Generating Random Point Cloud
  point.pts.resize(data.size());
  for (size_t i = 0; i < data.size(); i++)
  {
    point.pts[i].x = data[i][0];
    point.pts[i].y = data[i][1];
    point.pts[i].z = data[i][2];
  }
}
