#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "readdata.ih"

vector<DataPoint> readdata(string name_of_file)
{
  ifstream infile(name_of_file);
  if (!infile)
    cerr << "Could not open file!\n";

  string line;
  int i = 0;
  DataPoint v;
  int linelength, lines, linee;
  vector<DataPoint> data;
  while(getline(infile,line)){
    linelength = line.length();
    lines = 0;
    i = 0;
    linee = line.find(',');
    while(linee>0){
      v[i] = stof(line.substr(lines,linee));
      i++;
      lines = linee+1;
      linee = line.find(',',linee+2);
    }
    v[2] = stof(line.substr(lines,linelength));
    data.push_back(v);
  }
  return data;
}

string getValue(ifstream &infile)
{
  string line;
  getline(infile, line);
  size_t lines = line.find(',');
  return line.substr(lines + 1, line.length());
}

Options readOptions(string name_of_file)
{
  ifstream infile(name_of_file);
  if (!infile)
    cerr << "Could not open file!\n";

  Options options;
  try
  {
    options.numberOfAntsX      = stoul(getValue(infile));
    options.numberOfAntsY      = stoul(getValue(infile));
    options.numberOfAntsZ      = stoul(getValue(infile));
    options.numberOfIterations = stoul(getValue(infile));
    options.numberOfSteps      = stoul(getValue(infile));
    options.beta               = stof(getValue(infile));
    options.kappa              = stof(getValue(infile));
    options.p_release          = stof(getValue(infile));
    options.EvapRate           = stof(getValue(infile));
    options.threshold          = stoi(getValue(infile));
    options.radius             = stof(getValue(infile));
    options.upperlimit         = stof(getValue(infile));
    options.lowerlimit         = stof(getValue(infile));
  }
  catch(int n)
  {
    cout << "check the option file. It may be empty!";
  }
  return options;
}

void writeCSV(string name_of_file, vector<DataPoint> const &data)
{
  ofstream outputfile;
  outputfile.open(name_of_file);
  for (size_t i = 0; i < data.size(); ++i)
    outputfile << data[i].getPheromone() << '\n';

  outputfile.close();
}
