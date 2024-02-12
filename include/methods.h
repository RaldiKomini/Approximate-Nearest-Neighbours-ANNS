#ifndef METHODS_H
#define METHODS_H

#include "pr.h"
#include <algorithm>

class Method {
    protected:
        vector<vector<uchar>> &imgs;    //  Pixels for each image
        int IMAGES;     // Number of images
        int PIXELS;     // Number of pixels
        int w;      //  w used in h(p)
        int K;      //  LSH: Total h functions, Cube: Total dimensions {0,1}^K
        double* t;      // random t used in h(p)
        vector<vector<double>> v;   // random v used in h(p)
        vector<point> nNearest;

        Method(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int w_, int K_);

        //  Creates t and v, necessasry for h(p)
        void hfunc_factors(int w, double t[], vector<vector<double>> &v, int rows, int pixels_per_row);
        //  Calculates h(p)
        uint calc_h(vector<uchar> &imgs,  vector<double> &v, double t, int w,  int dim);
        uint calc_h(vector<double> &imgs,  vector<double> &v, double t, int w,  int dim);
    public:
        vector<point> nearest_search(int N);
        vector<int> range_search(double R);
};


typedef struct {
    char filnam[64];    //  input file
    char qfil[64];      //  query/config file
    char outf[64];      //  output file
    ofstream sendto;    // stream class of output file
}Files;

//  Nearest Neighbours with brute force
vector<point> brute_nearest(vector<vector<uchar>>  &imgs, vector<uchar> &query, int N, int metric);
vector<point> brute_nearest(vector<vector<uchar>>  &imgs, vector<double> &query, int N, int metric);

#endif /* METHODS_H */