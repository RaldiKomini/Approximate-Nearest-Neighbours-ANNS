#include "../include/mrng.h"

// compare function for candidate objects
bool comp_cand(candidate a, candidate b) {return a.dist < b.dist;}

//  Checks if img_id is in vector R. Returns 1 if found, 0 if not found
bool candidate_find(vector<candidate>& R, int img_id) {
    for(int i= 0; i< R.size(); i++) {
        if(R[i].id == img_id)
            return 1;
    }
    return 0;
}

//  Creates Rp wrt point p
void Rp_create(vector<point>& Rp, vector<vector<uchar>>& imgs,  int p, int IMAGES, int PIXELS= 784) {
    for(int i= 0; i< IMAGES; i++) {
        if(p == i)  // r != p
            continue;
        double dist= p_norm(imgs[i], imgs[p], PIXELS, 2);
        point r= {dist, i};
        Rp.push_back(r);
    }

    std::sort(Rp.begin(), Rp.begin()+Rp.size(), comp_points);
}


MRNG::MRNG(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_):
    Graph_index(imgs_, IMAGES_, PIXELS_) {


    uint r= rand();
    LSH lsh(imgs, IMAGES, PIXELS, r, 150, 4, 5);

    adj_matrix.resize(IMAGES);
    for(int p= 0; p< IMAGES; p++) {  //  for each point p in dataset

        lsh.query(imgs[p]);
        vector<point> Lp= lsh.nearest_search(1);    // nearest neighbour of p obviously is in Lp

        bool inLp[IMAGES]{};
        inLp[Lp[0].id]= true;   //  mark nn of p

        vector<point> Rp;   // Contains all other points, sorted based on distance to p
        Rp_create(Rp, imgs, p, IMAGES, PIXELS);

        for(int rid= 0; rid< IMAGES-1; rid++) {  // r: all other points
            int r= Rp[rid].id;
            if(r == p || inLp[r] == true)   //  r != p and r must not already be p's neighbour
                continue;

            double pr= Rp[rid].dist;

            bool condition= true;
            int Lp_size= Lp.size();
            for(int t= 0; t< Lp_size; t++) {    // t: nearest points to p
                double pt= Lp[t].dist;

                if(pr <= pt)     // pr obviously not longest edge in triangle prt, so no need to calculate rt
                    continue;

                double rt= p_norm(imgs[r], imgs[Lp[t].id], PIXELS, 2);
                if(pr >= rt) {     // pr longest edge
                    condition= false;
                    break;
                }
            }

            if(condition == true) {     // add r to neighbours of p
                point t= {pr, r};
                Lp.push_back(t);
                inLp[r]= true;
            }
        }

        for(int i= 0; i< Lp.size(); i++)
            adj_matrix[p].push_back(Lp[i].id);

    }

    vector<uint> avg_PIXELS(PIXELS, 0);     //  keep sum in uint vector, since sum will surpass 255 bit limit
    for(int i= 0; i< IMAGES; i++) {
        for(int j= 0; j< PIXELS; j++)
            avg_PIXELS[j]+= imgs[i][j];
    }
    
    vector<uchar> centroid_start_point(PIXELS);     //  transfer to uchar, since division will be < 255
    for(int j= 0; j< PIXELS; j++)
        centroid_start_point[j]= avg_PIXELS[j]/IMAGES;

    lsh.query(centroid_start_point);        // approximate nn of centroid
    start_node= lsh.nearest_search(1)[0];     // save starting node to give to navigating node, for each new query
}

MRNG::~MRNG() {};


void MRNG::query(vector<uchar> &query_, int Lcand, int metric) {
    R.clear();
    
    double nav_dist= p_norm(imgs[start_node.id], query_, PIXELS, 2);
    point nav_node= {nav_dist, start_node.id};   // navigating point starts from starting point
    R.push_back({nav_node.dist, nav_node.id, 1});
    
    int i= 1;
    int p;
    while(1) {

        vector<int> neighbors= adj_matrix[nav_node.id];
        for(int n= 0; n< neighbors.size(); n++) {   // for every neighbour n of p
            if(candidate_find(R, neighbors[n]) == false) {    // if n is not in R
                double dist= p_norm(imgs[neighbors[n]], query_, PIXELS, 2);  // distance of n to query to be used for sorting R
                R.push_back({dist, neighbors[n], 0});
                i++;
            }
        }

        std::sort(R.begin(), R.begin()+R.size(), comp_cand);
        if(i >= Lcand)
            break;
        
        //  Choose nearest unchecked point to query
        for(p= 0; p< R.size(); p++) {
            if(R[p].marked == 0) {
                nav_node= {R[p].dist, R[p].id};
                R[p].marked= 1;
                break;
            }
        }
        //  extreme condition: if no neighbours were inserted (because they were
        //  already in R) and all points in R are marked, nav_node does not change and
        //  query falls in infinite loop
        if(p == R.size())
            break;
    }
}

vector<point> MRNG::nearest_search(int N) {
    vector<point> ann(N);
    for(int i= 0; i< N; i++)
        ann[i]= {R[i].dist, R[i].id};
    return ann;
}