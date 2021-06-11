#ifndef READDATA_H
#define READDATA_H

#include <vector>
#include <array>
#include <cstddef>
#include <string>

class DataPoint;

struct Options
{
    size_t numberOfAntsX;
    size_t numberOfAntsY;
    size_t numberOfAntsZ;
    size_t numberOfIterations;
    size_t numberOfSteps;
    size_t threshold;
    float radius;
    float beta;
    float kappa;
    float p_release;
    float EvapRate;
    float upperlimit;
    float lowerlimit;
};

std::vector<DataPoint> readdata(std::string name_of_file);
void writeCSV(std::string name_of_file, std::vector<DataPoint> const &data);
Options readOptions(std::string name_of_file);

#endif
