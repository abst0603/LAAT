#ifndef ANTPCA_H
#define ANTPCA_H

#include <vector>
#include <array>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <random>

#include "Eigen/Core"
#include "Eigen/SVD"

// main function implementing the LAAT algorithm
std::vector<float> LocallyAlignedAntTechnique(
  std::vector<std::vector<float>> const &data,
  size_t numberOfAntsX,
  size_t numberOfAntsY,
  size_t numberOfAntsZ,
  size_t numberOfIterations,
  size_t numberOfSteps,
  size_t threshold,
  float radius,
  float beta,
  float kappa,
  float p_release,
  float evapRate,
  float lowerlimit,
  float upperlimit);

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
				    size_t numberOfAntsX,
				    size_t numberOfAntsY,
				    size_t numberOfAntsZ);

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
	       size_t numberOfSteps,
	       float beta,
	       float kappa,
	       float p_release,
  	       std::vector<float> &pheromone);
void evaporatePheromone(std::vector<float> &pheromone,
		        float evapRate,
			float lowerlimit,
			float upperlimit);

// functions to communicate with the user
void initializeProgressBar(size_t size);
void updateProgressBar(size_t loop);
void completeProgressBar();
void printWarning(size_t amountRuns);

#endif
