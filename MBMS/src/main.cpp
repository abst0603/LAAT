#include "MBMS.ih"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "please provide a data file name\n";
    exit(1);
  }
  
  srand(random_device()());
  float radius = 0.5;
  radius = pow(radius, 2); // Nanoflann library gets r^2 instead of r for rangesearch
  float sigma2 = 0.25;
  sigma2 = 2 * pow(sigma2, 2);
  unsigned int k = 10;
  int maxiter = 10;

  vector<vector<float>> data = readCSV(argv[1], 3);

  for (int iter = 0; iter < maxiter; ++iter)
  {
    cout << "starting round " << iter + 1 << '\n';
    MBMS(data, radius, sigma2, k);
  }

  writeCSV("result.csv", data);
}
