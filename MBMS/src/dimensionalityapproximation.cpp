#include "MBMS.ih"

void dimensionalityApproximation(vector<float> &evalue, int &dim, float &error)
{
  int D = evalue.size();
  dim = 0;
  error = 0;
  float Max = evalue[0] - evalue[1];

  for(int i = 1; i < D - 1; ++i)
    if (Max < evalue[i] - evalue[i + 1])
    {
      Max = evalue[i] - evalue[i + 1];
      dim = i;
    }

  if (Max < evalue[D - 1])
  {
    Max = evalue[D - 1];
    dim = D - 1;
  }

  for(int i = dim + 1; i < D; ++i)
    error += evalue[i];
}
