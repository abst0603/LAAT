#ifndef INCLUDED_MBMS_H
#define INCLUDED_MBMS_H

#include "KDTreeVectorOfVectorsAdaptor.h"
#include "Eigen/Core"
#include "Eigen/SVD"
#include "nanoflann.hpp"
#include "utils.h"
#include <iosfwd>

void convertTocloud(PointCloud<float> &point, std::vector<std::vector<float>> &data);


void denoise(std::vector<std::vector<float>> &data,
             std::vector<unsigned int> &indices,
             std::vector<float> &evalue,
             std::vector<std::vector<float>> &evector,
             float sigma2, unsigned int idx, std::vector<std::vector<float>> &dataout);

void localPCA(std::vector<std::vector<float>> &data,
              std::vector<unsigned int> &indices,
              std::vector<float> &v,
              std::vector<std::vector<float>> &tmpvec);

void dimensionalityApproximation(std::vector<float> &evalue, int &dim, float &error);

void kernelFunMean(std::vector<std::vector<float>> &data,
		   std::vector<unsigned int> &indices,
		   std::vector<float> &kernelmean, int idx, float sigma);

std::vector<std::vector<float>> readCSV(std::string const &name_of_file, size_t dim);

void writeCSV(std::string const &name_of_file,
	      std::vector<std::vector<float>> vec);

#endif
