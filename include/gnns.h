#include "graph_index.h"

class GNNS: public Graph_index{
    private:
        vector<point> S;    //  candidates for nearest neighbour

    public:
        GNNS(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int k_=50);
        ~GNNS();

        void query(vector<uchar> &query_, int metric, int E=30, int R=1, int T=20);
        vector<point> nearest_search(int N= 1);
};