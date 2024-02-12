#ifndef HYPERCUBE_H
#define HYPERCUBE_H

#include "methods.h"
#include "getopt.h" //  getopt_long

class Cube: public Method {
    private:
        int* f;
        bool** img_to_cube;
        unordered_map<int, vector<int>>* hypercube;

        // Calculate f(h(p)). Converts {0,1} sequence of image p into an integer (key) and returns it
        int calc_fh(bool img_to_cube[], int f[], int D, vector<uchar> &imgs,  vector<vector<double>> &v, double t[], int w,  int dim);
        int calc_fh(bool img_to_cube[], int f[], int D, vector<double> &imgs,  vector<vector<double>> &v, double t[], int w,  int dim);
        
        // int myperm(bool A[], int bits, bool** R, unordered_map<int, vector<int>> *hypercube,  int probes);
        void perm(bool* A, unordered_map<int, vector<int>> *hypercube, int H, int ch, int pos, int& candidates, int size, int M, int probes, vector<bool>& adj);
    public:
        Cube(vector<vector<uchar>> &imgs_, int IMAGES, int PIXELS, int w, int K=14);
        ~Cube();

        void query(vector<uchar> &query_, int M= 10, int probes= 2, int metric=2);
        void query(vector<double> &query_, int M= 10, int probes= 2, int metric=2);
};


typedef struct {
    Files fil;
    int K;  //  cube dimension
    int M;  //  candidates for nearest
    int probes; // probes
    int N;  // nearest
    int R;  // radius
}Cube_opt;

Cube_opt Cube_options(int argc, char** argv);

//  Convert and return binary sequence to decimal
int bin_to_dec(bool img_to_cube[], int D);

#endif /*   HYPERCUBE_H */