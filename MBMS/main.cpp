#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <nanoflann.hpp>
#include "utils.h"
using namespace nanoflann;
#include "KDTreeVectorOfVectorsAdaptor.h"

void convertTocloud(PointCloud<float> &point, std::vector<std::vector<float>> &data);


void Denoise(std::vector<std::vector<float>> &data,
             std::vector<unsigned int> &indices,
             std::vector<float> &evalue,
             std::vector<std::vector<float>> &evector,
             float sigma2, unsigned int idx, std::vector<std::vector<float>> &dataout);

void LocalPCA(std::vector<std::vector<float>> &data,
              std::vector<unsigned int> &indices,
              std::vector<float> &v,
              std::vector<std::vector<float>> &tmpvec);

void DimensionalityApproximation(std::vector<float> &evalue, int &dim, float &error);

void KernelFunMean(std::vector<std::vector<float>> &data,
              std::vector<unsigned int> &indices,
              std::vector<float> &kernelmean, int idx, float sigma);

std::vector<std::vector<float> > readCSV(std::string name_of_file, int dim);

void writeCSV(std::string name_of_file,
                std::vector<std::vector<float>> vec);

int main()
{

    srand(time(0));
    std::string outputname = "result";
    std::vector<std::vector<float>> data;
    std::vector<std::vector<float>> dataout;
    std::vector<unsigned int > neighborlist;
    std::vector<float> evalue;
    std::vector<std::vector<float>> evector;

    float radius = 0.5 ;
    radius = std::pow(radius,2);//Nanoflann library gets r^2 instead of r for rangesearch
    float sigma2 = 0.25 ;
    sigma2 = 2*std::pow(sigma2,2);
    unsigned int k = 10 ;
    int maxiter = 10 ;
    data = readCSV("data.csv", 3);
    dataout.resize(data.size(), std::vector<float>(data[0].size()));
    //RangeSearch initialization
    PointCloud<float> cloud;
    nanoflann::SearchParams params;
    std::vector<std::pair<unsigned long int,float> > RS_matches;
    typedef KDTreeSingleIndexAdaptor<
            L2_Simple_Adaptor<float, PointCloud<float> > ,
            PointCloud<float>,
            3 /* dim */
            > RS_kd_tree;

    //KNN
    typedef KDTreeVectorOfVectorsAdaptor< std::vector<std::vector<float> >, float >  KNN_kd_tree;
    std::vector<unsigned long int> ret_indexes(k);
	std::vector<float> out_dists_sqr(k);
	nanoflann::KNNResultSet<float> resultSet(k);

	//pca init
	evalue.assign(data[0].size(),0);
	evector.assign(data[0].size(),std::vector<float>(data[0].size(),0));

    for (int iter=0; iter<maxiter; iter++)
    {
        neighborlist.resize(data.size());
        std::cout << "starting " << iter+1 << "-th round" << std::endl ;
        // finding neighbors
        //RangeSearch(data, neighborlist, r2, k);
        convertTocloud(cloud, data);
        RS_kd_tree   RSindex(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
        RSindex.buildIndex();

        //KNN
        KNN_kd_tree   KNN_index(3, data, 10 /* max leaf */ );
        KNN_index.index->buildIndex();

        for (unsigned int idx = 0; idx<data.size();idx++){

            unsigned int nMatches = RSindex.radiusSearch(&data[idx][0], radius, RS_matches, params);
            if (nMatches>=k) {
                neighborlist.resize(nMatches);
                for (unsigned int i = 0; i < nMatches; i++)
                    neighborlist[i] = RS_matches[i].first ;

            } else
            {
                neighborlist.resize(k) ;
                resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);

                KNN_index.index->findNeighbors(resultSet, &data[idx][0], nanoflann::SearchParams(10));
                for (unsigned int i = 0; i < k; i++)
                    neighborlist[i] = ret_indexes[i];
            }

            //local PCA
            LocalPCA(data, neighborlist, evalue, evector) ;

            // denoising
            Denoise(data, neighborlist, evalue, evector, sigma2, idx, dataout);
        }
        //clear the cloud which was build for RangeSearch
        cloud.pts.clear();
        cloud.pts.shrink_to_fit();
        for (unsigned int i=0; i<data.size(); i++)
            for (unsigned int j=0; j<data[0].size(); j++)
                data[i][j] = dataout[i][j] ;
        // save
        writeCSV(outputname+std::to_string(iter)+".csv", data);
        neighborlist.clear();
        neighborlist.shrink_to_fit();
    }

    return 0;
}

void convertTocloud(PointCloud<float> &point, std::vector<std::vector<float>> &data)
{
	// Generating Random Point Cloud
	point.pts.resize(data.size());
	for (size_t i = 0; i < data.size(); i++)
	{
		point.pts[i].x = data[i][0];
		point.pts[i].y = data[i][1];
		point.pts[i].z = data[i][2];
	}
}

void LocalPCA(std::vector<std::vector<float>> &data,
              std::vector<unsigned int> &indices,
              std::vector<float> &v,
              std::vector<std::vector<float>> &tmpvec)
{
    unsigned int D = data[0].size() ;

    Eigen::MatrixXf X(indices.size(),D);
    Eigen::RowVectorXf meanvec(D);

    for (unsigned int j=0;j<indices.size();j++)
        for (unsigned int d=0; d<D; d++)
            X(j,d) = data[indices[j]][d] ;

    for (unsigned int d=0; d<D; d++)
        meanvec(d) = X.middleCols<1>(d).mean();

    X = X.rowwise() - meanvec;
    meanvec.resize(0);//This is the destructor of meanvec

    Eigen::BDCSVD< Eigen::MatrixXf >svd( X, Eigen::ComputeThinV);
    X.resize(0,0);//This is the destructor of X

    Eigen::RowVectorXf svalue(D);
    svalue = svd.singularValues();
    Eigen::MatrixXf svector(D,D);
    svector = svd.matrixV();  // eigenvector are in columns
    svalue = svalue.array().pow(2);//eigen values are the second power of singular values
    svalue = svalue.array() / svalue.sum();//Normalize eigen values

    for (unsigned int d=0; d<D; d++)
        v[d] = svalue(d);
    svalue.resize(0);

    for (unsigned int j=0;j<D;j++)
        for (unsigned int k=0;k<D;k++)
            tmpvec[j][k] = svector(j,k);
    svector.resize(0,0);
}

void DimensionalityApproximation(std::vector<float> &evalue, int &dim, float &error)
{
    int D = evalue.size() ;
    dim=0 ;
    error = 0 ;
    float Max = evalue[0]-evalue[1];

    for(int i=1; i<D-1; i++)
        if (Max<evalue[i]-evalue[i+1])
        {
            Max = evalue[i]-evalue[i+1] ;
            dim = i ;
        }

    if (Max<evalue[D-1]) {
        Max = evalue[D-1] ;
        dim = D-1 ;
    }

    for(int i=dim+1; i<D; i++)
        error += evalue[i] ;
}


void KernelFunMean(std::vector<std::vector<float>> &data,
              std::vector<unsigned int> &indices,
              std::vector<float> &kernelmean, int idx, float sigma2)
{
    float dist2 , sumK=0;
    std::vector<float> K(indices.size()) ;
    unsigned int D = data[0].size() ;

    kernelmean.assign(D,0);

    for (unsigned int i=0; i<indices.size(); i++)
    {
        dist2 = 0 ;
        for (unsigned int j=0; j<D; j++)
            dist2 += pow(data[idx][j] - data[indices[i]][j],2) ;
        K[i] = exp(-dist2 / sigma2) ;
        sumK += K[i];
    }

    for (unsigned int i=0; i<indices.size(); i++)
        for (unsigned int j=0; j<D; j++)
            kernelmean[j] += (K[i]*data[indices[i]][j] / sumK) ;
}

void Denoise(std::vector<std::vector<float>> &data,
             std::vector<unsigned int> &indices,
             std::vector<float> &evalue,
             std::vector<std::vector<float>> &evector,
             float sigma2, unsigned int idx, std::vector<std::vector<float>> &dataupdate)
{
    unsigned int D = data[0].size() ;

    float error = 0 ;
    int dim = 0 ;

    std::vector<float> kernelmean(D) ;
    std::vector<std::vector<float>> tmpproj;
    float deltaX=0 ;
    tmpproj.resize(D, std::vector<float>(D,0));

    // ****** computing the dimensionality
    DimensionalityApproximation(evalue, dim, error);

    // ****** Projection matrix: (I - UU^T)
    if (error!=0) {
        //assiging values to an eye matrix
        tmpproj.assign(D,std::vector<float>(D,0));
        for (unsigned int j=0; j<D; j++){
            tmpproj[j][j] = 1;//I
            for (unsigned int k=0; k<D; k++)
                for (unsigned int t=0; t<=dim; t++)
                    tmpproj[j][k] -= (evector[j][t]*evector[k][t]) ;}

        // ****** Kernel function
        KernelFunMean(data, indices, kernelmean, idx, sigma2);

        // ****** updating the data set
        for (unsigned int j=0; j<D; j++)
        {
            deltaX=0 ;
            for (unsigned int k=0; k<D; k++)
                deltaX += tmpproj[j][k] * (-data[idx][k] + kernelmean[k]);
            dataupdate[idx][j] = data[idx][j] + deltaX ;
        }
    }
    else {
        for (unsigned int j=0; j<D; j++)
            dataupdate[idx][j] = data[idx][j] ;
    }

}

void writeCSV(std::string name_of_file,std::vector<std::vector<float>> vec)
{
    unsigned int N = vec.size();
    unsigned int D = vec[0].size();

    std::ofstream outputfile;

    outputfile.open(name_of_file);
    for(unsigned int i=0;i<N;i++){
        for(unsigned int j=0;j<D-1;j++){
            outputfile<<vec[i][j]<<',';
        }
        outputfile<<vec[i][D-1]<<'\n';
    }
    outputfile.close();
}

std::vector<std::vector<float> > readCSV(std::string name_of_file, int dim){
    std::ifstream infile(name_of_file);
    if (!infile)
    {
       std::cerr << "Could not open file!"  <<  std::endl;
    }
    std::string line;
    int i = 0;
    int linelength, lines, linee;
    std::vector<float> v;
    std::vector<std::vector<float> > data;

    v.resize(dim) ;

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
        v[dim-1] = stof(line.substr(lines,linelength));
        data.push_back(v);
    }
    return data;
}
