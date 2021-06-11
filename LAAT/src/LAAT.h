#ifndef ANTPCA_H
#define ANTPCA_H

#include <vector>
#include <array>
#include <numeric> // for std::accumulate
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <random>

#include "../lib/eigen/Core"
#include "../lib/eigen/SVD"
#include "readdata.h"
#include "datapoint/datapoint.h"

// main function implementing the LAAT algorithm
void LocallyAlignedAntTechnique(std::vector<DataPoint> &data,
				Options options);

// preprocessing functions
size_t preprocess(std::vector<DataPoint> &data,
		size_t radius,
		float threshold);
size_t RangeSearch(std::vector<DataPoint> &data,
		   float radius,
		   size_t threshold);
std::vector<unsigned int> groupdata(std::vector<DataPoint> const &data,
				    Options const &options);

// iterative functions
void initializeAnts(std::vector<DataPoint> &data,
		    std::vector<DataPoint *> &ants,
		    std::vector<unsigned int> const &gd,
		    size_t medianValue);
void antSearch(std::vector<DataPoint> &data,
	       std::vector<DataPoint *> const &initialpoint,
	       Options const &options);
void evaporatePheromone(std::vector<DataPoint> &data,
			Options const &options);

// functions to communicate with the user
void initializeProgressBar(size_t size);
void updateProgressBar(size_t loop);
void completeProgressBar();
void printWarning(size_t amountRuns);

#endif
