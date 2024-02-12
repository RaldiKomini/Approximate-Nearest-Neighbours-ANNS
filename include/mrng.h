#include "graph_index.h"

typedef struct {
    double dist;    // distance to query
    int id;         // image id to find its neighbours in graph index
    bool marked;    // 0: not marked, 1: marked
} candidate;

class MRNG: public Graph_index{
    private:
        point start_node;   //  starting node of graph search, calculated in preprocessing
        vector<candidate> R;    //  Nearest Neighbour candidates

    public:
        MRNG(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_);
        ~MRNG();

        void query(vector<uchar> &query_, int Lcand=20, int metric=2);
        vector<point> nearest_search(int N= 1);
};