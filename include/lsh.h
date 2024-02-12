#ifndef LSH_H
#define LSH_H
#include "methods.h"

class LSH: public Method {
    private:
        int r;      // random r used for multiplying with h_i(p)
        uint **ID;
        int L;      // Number of Hash Tables
        unordered_map<int, vector<int>> *htable;
        uint M;     // M used for modulo
        int TABLESIZE;

        // Calculates g(p)
        int calc_g(int htid, int K, vector<uchar> &imgs, vector<vector<double>> &v, double t[], int w, int dim, uint r, uint &ID, int M, int tablesize);
        int calc_g(int htid, int K, vector<double> &imgs, vector<vector<double>> &v, double t[], int w, int dim, uint r, uint &ID, int M, int tablesize);
    public:
        LSH(vector<vector<uchar>> &imgs_, int IMAGES, int PIXELS, int r, int w,  int K= 4, int L= 5);
        ~LSH();

        void query(vector<uchar> &query_, int metric=2);
        void query(vector<double> &query_, int metric=2);
};

typedef struct {
    Files fil;
    int K;  // #of h_i functions
    int L;  // #of hash tables
    int N;  // nearest
    int R;  // radius
}LSH_opt;

LSH_opt LSH_options(int argc, char** argv);

#endif /*   LSH_H   */