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

// main function implementing the LAAT algorithm
std::vector<float> LocallyAlignedAntTechnique(
  std::vector<std::vector<float>> const &data,
  Options options);

// preprocessing functions
size_t preprocess(std::vector<std::vector<float>> const &data,
		  size_t radius,
		  float threshold,
		  std::vector<std::vector<unsigned int>> &neighbourhoods,
		  std::vector<std::vector<std::vector<float>>> &eigenVectors,
		  std::vector<std::vector<float>> &eigenValues);
size_t rangeSearch(std::vector<std::vector<float>> const &data,
		   float radius,
		   size_t threshold,
		   std::vector<std::vector<unsigned int>> &neighbourhoods);
std::vector<unsigned int> groupdata(std::vector<std::vector<float>> const &data,
				    Options const &options);

// iterative functions
void initializeAnts(std::vector<std::vector<unsigned int>> const &neighbourhoods,
		    std::vector<unsigned int> const &gd,
		    size_t medianValue,
		    std::vector<unsigned int> &antLocations);
void antSearch(std::vector<std::vector<float>> const &data,
	       std::vector<std::vector<unsigned int>> const &neighbourhoods,
	       std::vector<unsigned int> const &antLocations,
	       std::vector<std::vector<std::vector<float>>> const &eigenVectors,
	       std::vector<std::vector<float>> const &eigenValues,
	       Options const &options,
  	       std::vector<float> &pheromone);
void evaporatePheromone(std::vector<float> &pheromone,
			Options const &options);

// functions to communicate with the user
void initializeProgressBar(size_t size);
void updateProgressBar(size_t loop);
void completeProgressBar();
void printWarning(size_t amountRuns);

#endif
