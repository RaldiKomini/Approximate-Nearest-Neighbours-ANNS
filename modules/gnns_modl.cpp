#include "../include/gnns.h"


//  Adds neighbours of Yt in S, and returns the one nearest to Query
point add_neighbours(vector<point>& S, vector<int> neighbours, vector<vector<uchar>>& imgs, int* marked, vector<uchar> q, int E) {
    int pixels= q.size();
    point min_point= {MAXFLOAT, -1};

    // Usually occurs for small dataset (~1k images)
    if(neighbours.size() < E)
        E= neighbours.size();
    
    for(int i= 0; i< E; i++) {
        double dist= p_norm(imgs[neighbours[i]], q, pixels, 2);

        if(dist< min_point.dist) {
            min_point.dist= dist;
            min_point.id= neighbours[i];
        }

        if(marked[neighbours[i]] == 0) {  // Don't add if already in S
            point p= {dist, neighbours[i]};
            S.push_back(p);
            marked[neighbours[i]]= 1;
        }
    }

    return min_point;
}


GNNS::GNNS(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int k):
    Graph_index(imgs_, IMAGES_, PIXELS_) {

    uint r= rand();
    LSH lsh(imgs, IMAGES, PIXELS, r, 150, 4, 5);

    adj_matrix.resize(IMAGES);
    for(int i = 0 ; i < IMAGES;i ++) {
        lsh.query(imgs[i]);
        vector<point> v = lsh.nearest_search(k);    //  neighbours of image
        
        //  for j=0 till v.size(), because lsh may find less than k points for small dataset
        for(int j = 0; j < v.size();j++){
            adj_matrix[i].push_back(v[j].id);
        }
    }
}

GNNS::~GNNS() {};



void GNNS::query(vector<uchar> &query_, int metric, int E, int R, int T) {
    S.clear();
    int marked[IMAGES]{};
    int tloops= 0;
    for(int r= 0; r< R; r++) {

        int yt_id= rand()%IMAGES;  // randomly chosen node
        double yt_dist= p_norm(imgs[yt_id], query_, PIXELS, 2);

        point Yt= {yt_dist, yt_id};
        for(int i= 0; i< T; i++) {
            tloops++;

            point Yt_new= add_neighbours(S, adj_matrix[Yt.id], imgs, marked, query_, E);
            if(Yt_new.dist > Yt.dist)   // end search if the new Yt gets us further from query
                break;

            Yt= Yt_new;
        }
    }
    std::sort(S.begin(), S.begin()+S.size(), comp_points);
}


vector<point> GNNS::nearest_search(int N) {
    vector<point> ann(S.begin(), S.begin()+N);
    return ann;
}