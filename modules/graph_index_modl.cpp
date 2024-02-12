#include "../include/graph_index.h"

Graph_index::Graph_index(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_):
    imgs(imgs_), IMAGES(IMAGES_), PIXELS(PIXELS_) {}

Graph_index::~Graph_index() {}

bool comp_points(point a, point b) {return a.dist < b.dist;}