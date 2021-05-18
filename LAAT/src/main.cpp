#include "LAAT.ih"

int main(int argc, char **argv)
{
  // Use random seed, or constant seed for constant results
  srand(random_device()());
  //srand(42);

  // process input
  if (argc < 3)
  {
    cerr << "invalid number of arguments\n";
    exit(1);
  }
  string dataFileName = argv[1];
  string optionFileName = argv[2];
  string outputFileName;
  if (argc > 3)
    outputFileName = argv[3];
  else
    outputFileName = "pheromone.csv";

  // get data
  vector<DataPoint> data = readdata(dataFileName);

  // get options
  Options options = readOptions(optionFileName);

  // perform LAAT algorithm
  LocallyAlignedAntTechnique(data, options);

  // write results
  writeCSV(outputFileName, data);
}
