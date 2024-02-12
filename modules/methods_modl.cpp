#include "../include/methods.h"


Method::Method(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int w_, int K_):
        imgs(imgs_), IMAGES(IMAGES_), PIXELS(PIXELS_), w(w_), K(K_) { }

void Method::hfunc_factors(int w, double t[], vector<vector<double>> &v, int rows, int pixels) {
    default_random_engine generator;
    normal_distribution<double> distrib(0,1);    // normal distribution for vector v

    for(int i= 0; i< rows; i++) {
        v[i]= vector<double>(pixels);
        for(int j= 0; j< pixels; j++) {
            v[i][j]= distrib(generator);    // v, with PIXEL dimensions, to be used for dot prod with p
        }
        t[i] = static_cast<double>(rand()-1) / static_cast<double>(RAND_MAX);
        t[i]= t[i]*w;   // rand t in [0,w)
    }
}

uint Method::calc_h(vector<uchar> &imgs, vector<double> &v, double t, int w, int dim) {
    uint h= (dot_prod(imgs, v, dim) + t)/static_cast<double>(w);   // unsigned int to prevent overflow
    return h;
}

uint Method::calc_h(vector<double> &imgs, vector<double> &v, double t, int w, int dim) {
    uint h= (dot_prod(imgs, v, dim) + t)/static_cast<double>(w);   // unsigned int to prevent overflow
    return h;
}

vector<point> Method::nearest_search(int N) {
    int size= nNearest.size();

    vector<point>Nearest;
    int visited[IMAGES]= { };    // avoid duplicate nearest neighbours ( { }: initialize with zeros)

    for(int i= 0; i< N; i++) {
        point p;
        p.dist= 500000.0;
        p.id= -1;
        for(int j= 0; j< size; j++) {
            if(nNearest[j].dist < p.dist && visited[nNearest[j].id] == 0) {
                p.dist= nNearest[j].dist;
                p.id= nNearest[j].id;
            }
        }
        // if all points have been checked before reaching N
        if(p.id == -1)
            return Nearest;

        Nearest.push_back(p);
        visited[p.id]=1;
    }
    return Nearest;
}

vector<int> Method::range_search(double R) {
    int size= nNearest.size();

    vector<int>Nearest;
    for(int j= 0; j< size; j++) {
        if(nNearest[j].dist <= R) {
            Nearest.push_back(nNearest[j].id);
        }
    }

    return Nearest;
}

vector<point> brute_nearest(vector<vector<uchar>>  &imgs, vector<uchar> &query, int N, int metric) {
    int images= imgs.size();
    int pixels= imgs[0].size();
    
    int visited[images]= { };
    vector<point> NN;
    for(int n= 0; n< N; n++) {
        point min;
        min.dist= MAXFLOAT;
        min.id= -1;
        for(int i= 0; i < images; i++) {
            double dist= p_norm(imgs[i], query, pixels, metric);
            if(dist < min.dist && !visited[i] && dist > 0) {
                min.dist= dist;
                min.id= i;
            }
        }
        NN.push_back(min);
        visited[min.id]= 1;
    }
    return NN;
}