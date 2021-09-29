#ifndef INCLUDED_MBMS_H
#define INCLUDED_MBMS_H

#include "Eigen/Core"
#include "Eigen/SVD"
#include <iosfwd>

void modifiedBlurringMeanShift(std::vector<std::vector<float>> &data,
			       size_t iter = 10,
			       float radius = 0.5,
			       float sigma = 0.25,
			       size_t k = 10);

void denoise(std::vector<std::vector<float>> const &data,
             std::vector<size_t> const &indices,
             std::vector<float> const &evalue,
             std::vector<std::vector<float>> const &evector,
	     std::vector<std::vector<float>> &dataUpdate,
	     float sigma,
	     size_t dix);

void localPCA(std::vector<std::vector<float>> const &data,
              std::vector<size_t> const &indices,
              std::vector<float> &eigenValue,
              std::vector<std::vector<float>> &eigenVector);

void dimensionalityApproximation(std::vector<float> const &evalue,
				 size_t &dim,
				 float &error);

void kernelFunMean(std::vector<std::vector<float>> const &data,
		   std::vector<size_t> const &indices,
		   std::vector<float> &kernelmean,
		   size_t idx,
		   float sigma);

std::vector<std::vector<float>> readCSV(std::string const &name_of_file, size_t dim);

void writeCSV(std::string const &name_of_file,
	      std::vector<std::vector<float>> vec);

#endif
