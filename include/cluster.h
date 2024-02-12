// #include "methods.h"
#ifndef CLUSTER_H
#define CLUSTER_H
#include "lsh.h"
#include "hypercube.h"

typedef struct {
    Files fil;
    char method[12];
    int K;      //  # of clusters
    int L;      //  # of hash tables
    int k;      //  # of h_i functions;
    int M;      //  candidates for nearest
    int D;      //  Cube dimension
    int probes; //  probes
    bool Complete;
}Cluster_opt;

Cluster_opt Cluster_options(int argc, char** argv);

bool cK_check(vector<vector<double>> &oldcK, vector<vector<double>> &newcK, double e, int dim, int K, int metric);

//  modified binary search for kmeans++
int bin_search(double A[], double newC, int images);

/*      ASsignment Algorithms   */
void lloyds(vector<vector<uchar>> &imgs, vector<vector<int>> &team, vector<vector<double>> &cK, int metric);
void rev_lsh(vector<vector<uchar>> &imgs, vector<vector<int>> &team, vector<vector<double>> &cK, int k, int L, int metric);
void rev_cube(vector<vector<uchar>> &imgs, vector<vector<int>> &team, vector<vector<double>> &cK, int D, int M, int probes, int metric);

void cluster_stats(vector<vector<int>> &team, vector<vector<double>> &cK, int pixels, Cluster_opt &options);
void cluster_more_stats(vector<vector<int>> &team, vector<vector<double>> &cK, int pixels, Cluster_opt &options);

void Silhouette(vector<vector<uchar>> &imgs, vector<vector<int>> &team, vector<vector<double>> &cK, int K, int IMAGES, int PIXELS, Cluster_opt &options, int metric);

void kmeans(vector<vector<uchar>> &imgs, int IMAGES, int PIXELS, vector<vector<double>> &cK, vector<vector<int>> &team, int K, int metric);

#endif /*CLUSTER_H*/