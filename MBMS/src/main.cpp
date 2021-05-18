#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "MBMS.ih"

int main()
{
  srand(time(0));
  string outputname = "result";
  vector<vector<float>> data;
  vector<vector<float>> dataout;
  vector<unsigned int> neighborlist;

  float radius = 0.5;
  radius = pow(radius, 2); // Nanoflann library gets r^2 instead of r for rangesearch
  float sigma2 = 0.25;
  sigma2 = 2 * pow(sigma2, 2);
  unsigned int k = 10;
  int maxiter = 10;
  data = readCSV("points.csv", 3);
  unsigned int dim = data[0].size();
  dataout.resize(data.size(), vector<float>(data[0].size()));
  //RangeSearch initialization
  PointCloud<float> cloud;
  nanoflann::SearchParams params;
  vector<pair<unsigned long int, float>> RS_matches;

  vector<unsigned long int> ret_indexes(k);
  vector<float> out_dists_sqr(k);
  nanoflann::KNNResultSet<float> resultSet(k);

  //pca init
  vector<float> evalue(dim);
  vector<vector<float>> evector;
  evector.assign(dim, vector<float>(dim, 0));

  for (int iter = 0; iter < maxiter; ++iter)
  {
    neighborlist.resize(data.size());
    cout << "starting " << iter + 1 << "-th round" << endl;
    // finding neighbors
    //RangeSearch(data, neighborlist, r2, k);
    convertTocloud(cloud, data);
    RS_kd_tree RSindex(3 /*dim*/, cloud, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
    RSindex.buildIndex();

    //KNN
    KNN_kd_tree KNN_index(3, data, 10 /* max leaf */ );
    KNN_index.index->buildIndex();

    for (unsigned int idx = 0; idx < data.size(); ++idx)
    {
      unsigned int nMatches = RSindex.radiusSearch(&data[idx][0], radius, RS_matches, params);
      if (nMatches >= k)
      {
	neighborlist.resize(nMatches);
	for (unsigned int i = 0; i < nMatches; ++i)
	  neighborlist[i] = RS_matches[i].first;

      }
      else
      {
	neighborlist.resize(k);
	resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);

	KNN_index.index->findNeighbors(resultSet, &data[idx][0], nanoflann::SearchParams(10));
	for (unsigned int i = 0; i < k; ++i)
	  neighborlist[i] = ret_indexes[i];
      }

      //local PCA
      localPCA(data, neighborlist, evalue, evector);

      // denoising
      denoise(data, neighborlist, evalue, evector, sigma2, idx, dataout);
    }
    
    //clear the cloud which was build for RangeSearch
    cloud.pts.clear();
    cloud.pts.shrink_to_fit();
    for (unsigned int i = 0; i < data.size(); ++i)
      for (unsigned int j = 0; j < data[0].size(); ++j)
	data[i][j] = dataout[i][j];
    // save
    writeCSV(outputname + to_string(iter) + ".csv", data);
    neighborlist.clear();
    neighborlist.shrink_to_fit();
  }

  return 0;
}
