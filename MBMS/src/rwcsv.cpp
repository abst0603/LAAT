#include "MBMS.ih"

void writeCSV(string const &fileName, vector<vector<float>> data)
{
  unsigned int N = data.size();
  unsigned int D = data[0].size();

  ofstream outputFile(fileName);

  for (unsigned int i = 0; i < N; ++i)
  {
    for (unsigned int j = 0; j < D - 1; ++j)
      outputFile << data[i][j] << ',';
    outputFile << data[i][D-1] << '\n';
  }
  outputFile.close();
}

vector<vector<float>> readCSV(string const &fileName, size_t dim)
{
  ifstream inFile(fileName);
  if (!inFile)
    cerr << "Could not open file\n";

  vector<float> datapoint(dim);
  vector<vector<float>> data;
  string line;
  while (getline(inFile, line))
  {
    stringstream ss(line);
    float val;
    for (size_t i = 0; i < dim; ++i)
    {
      ss >> val;
      datapoint[i] = val;

      // ignore commas
      if (ss.peek() == ',')
	ss.ignore();
    }
    data.push_back(datapoint);
  }
  inFile.close();
  return data;
}
