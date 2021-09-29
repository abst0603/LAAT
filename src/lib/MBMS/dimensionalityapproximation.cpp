#include "MBMS.ih"

/**
 * Approximate the dimensionality of the eigen vectors.
 *
 * @param evalue eigen values
 * @param dim will store dimensionality
 * @param error will store error
 */
void dimensionalityApproximation(vector<float> const &evalue,
				 size_t &dim,
				 float &error)
{
  size_t D = evalue.size();
  float max = evalue[0] - evalue[1];

  dim = 0;
  for (size_t i = 1; i < D - 1; ++i)
    if (max < evalue[i] - evalue[i + 1])
    {
      max = evalue[i] - evalue[i + 1];
      dim = i;
    }

  if (max < evalue[D - 1])
  {
    max = evalue[D - 1];
    dim = D - 1;
  }

  error = 0;
  for (size_t i = dim + 1; i < D; ++i)
    error += evalue[i];
}
