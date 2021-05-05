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

void LocallyAlignedAntTechnique(std::vector<DataPoint> &data,
				Options options);

size_t preprocess(std::vector<DataPoint> &data,
		size_t threshold,
		float radius);

size_t RangeSearch(std::vector<DataPoint> &data,
		   float radius,
		   size_t threshold);
  
std::vector<unsigned int> groupdata(std::vector<DataPoint> const &data,
				    Options const &options);

void initializeAnts(std::vector<DataPoint> &data,
		    std::vector<DataPoint *> &ants,
		    std::vector<unsigned int> const &gd,
		    size_t medianValue);

void antSearch(std::vector<DataPoint> &data,
		  std::vector<DataPoint *> const &initialpoint,
		  Options const &options);

void initializeProgressBar(size_t size);
void updateProgressBar(size_t loop);
void completeProgressBar();

void evaporatePheromone(std::vector<DataPoint> &data,
			Options const &options);

#endif
