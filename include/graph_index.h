#ifndef GRAPH_INDEX
#define GRAPH_INDEX

#include "lsh.h"

// compare function for point objects
bool comp_points(point a, point b);

class Graph_index {
    protected:
        vector<vector<uchar>> imgs;
        int IMAGES;
        int PIXELS;
    
        vector<vector<int>> adj_matrix; //  Graph Index

    public:
        Graph_index(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_);
        ~Graph_index();
};


#endif //GRAPH_INDEX