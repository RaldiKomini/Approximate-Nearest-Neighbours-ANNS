#include "../include/hypercube.h"


Cube_opt Cube_options(int argc, char** argv) {
    int opt;
    Cube_opt options;
    *(options.fil.filnam)= '\0';
    *(options.fil.qfil)= '\0';
    *(options.fil.outf)= '\0';
    options.K= 4;
    options.M= 10;
    options.probes= 2;
    options.N= 1;
    options.R= 10000;

    const option longopts[]= {
        {"probes", required_argument,  0, 'p'},     // has to be first
        {0, required_argument, 0, 'd'},
        {0, required_argument, 0, 'q'},
        {0, required_argument, 0, 'k'},
        {0, required_argument,  0, 'M'},
        {0, required_argument,  0, 'o'},
        {0, required_argument,  0, 'N'},
        {0, required_argument,  0, 'R'},
        {0, 0, 0, 0}
    };
    int indx= 0;
    while( (opt= getopt_long(argc, argv, "d:q:k:M:p:o:N:R:", longopts, &indx)) != -1) {
        switch(opt) {
            case 'd':
                strcpy(options.fil.filnam, optarg);
                break;
            case 'q':
                strcpy(options.fil.qfil, optarg);
                break;
            case 'k':
                options.K= atoi(optarg);
                break;
            case 'M':
                options.M= atoi(optarg);
                break;
            case 'p':
                options.probes= atoi(optarg);
                break;
            case 'o':
                strcpy(options.fil.outf, optarg);
                break;
            case 'N':
                options.N= atoi(optarg);
                break;
            case 'R':
                options.R= atoi(optarg);
                break;
        }
    }

    if(*(options.fil.filnam)== '\0' || *(options.fil.qfil)== '\0' || *(options.fil.outf)== '\0') {
        cout<<"Give i/o or querry/config file\n";
        exit(1);
    }

    options.fil.sendto.open(options.fil.outf);
    
    return options;
}

Cube::Cube(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int w_, int K_):
    Method(imgs_, IMAGES_, PIXELS_, w_, K_) {
    
    t= new double[K];  // double t[D];
    v.resize(K);
    for(int i= 0; i< K; i++)
        v[i].resize(PIXELS);

    hfunc_factors(w, t, v, K, PIXELS);

    f= new int[K];
    for(int i= 0; i< K; i++) {
        f[i]= rand();      // f_i will be offset
    }

    img_to_cube= new bool*[IMAGES];
    for(int i= 0; i< IMAGES; i++)
        img_to_cube[i]= new bool[K];
    
    hypercube= new unordered_map<int, vector<int>>[1];  //  must declare # rows, even for 1 dimension
    for(int i= 0; i< IMAGES; i++) {     //  For each image p: Map p to {0,1}^K
        int k= calc_fh(img_to_cube[i], f, K, imgs[i], v, t, w, PIXELS);
        hypercube[0][k].push_back(i);
    }
}

Cube::~Cube() {
    for(int i= 0; i< K; i++)
        delete[] img_to_cube[i];
    delete[] img_to_cube;

    delete[] hypercube;
    delete[] f;
    delete[] t;
}

int Cube::calc_fh(bool img_to_cube[], int f[], int D, vector<uchar> &imgs,  vector<vector<double>> &v, double t[], int w,  int dim) {
    for(int j= 0; j< D; j++) {
        uint h= calc_h(imgs, v[j], t[j], w, dim);
        img_to_cube[j]= (h+f[j])%2;
    }
    return bin_to_dec(img_to_cube, D);
}

int Cube::calc_fh(bool img_to_cube[], int f[], int D, vector<double> &imgs,  vector<vector<double>> &v, double t[], int w,  int dim) {
    for(int j= 0; j< D; j++) {
        uint h= calc_h(imgs, v[j], t[j], w, dim);
        img_to_cube[j]= (h+f[j])%2;
    }
    return bin_to_dec(img_to_cube, D);
}

void Cube::perm(bool* A, unordered_map<int, vector<int>> *hypercube, int H, int ch, int pos, int& candidates, int K, int M, int probes, vector<bool>& adj) {
    if(pos  == K)    // if reached last bit, go back to previous perm()
        return;
    if(adj.size()/K == probes || candidates >= M)  // if at least M candidates or all probes have been chosen, exit
        return;

    A[pos]= !A[pos];    // change bit
    ch++;   // number of changes
    if(ch <H)   // not enough bits changed
        perm(A, hypercube, H, ch, pos+1, candidates, K, M, probes, adj);

    if(ch == H) {   // number of bits changed == hamming distance; this is what we are looking for
        int k= bin_to_dec(A, K);
        if(hypercube[0][k].size() != 0) {    // Save vertex only if it stores any images
            
            for(int i= 0; i< K; i++)
                adj.push_back(A[i]);
            candidates+=hypercube[0][k].size();
        }
    }

    A[pos]= !A[pos];    // reverse change, so the next bit will
    ch--;
    perm(A, hypercube, H, ch, pos+1, candidates, K, M, probes, adj);   // go to next bit
}

void Cube::query(vector<uchar> &query_, int M, int probes, int metric) {
    if(probes <= 0)
        return;

    nNearest.clear();
    bool query_to_cube[K];
    int qbucket= calc_fh(query_to_cube, f, K, query_, v, t, w, PIXELS);


    vector<bool> adjacent;
    int candidates= 0;
    // probes--;
    // M-= hypercube[0][qbucket].size();
    for(int i= 1; i<= K; i++) {
        perm(query_to_cube, hypercube, i, 0, 0, candidates, K, M, probes, adjacent);
        // Done if: at least M points have been selected,
        // or number of vertices checked (probes) has been reached
        if(adjacent.size()/K >= probes || candidates >= M)
            break;

    }

    vector<int> cube_nn;    // vector with all images from same bucket as query
    bool Vertex[K];     // adjacent vertices to query_to_cube
    for(int i= 0; i< adjacent.size()/K; i++) {
        cube_nn= hypercube[0][qbucket];     // Extract images from the same bucket as query

        for(int n: cube_nn) {
            double dist= p_norm(imgs[n], query_, PIXELS, metric);
            if(dist == 0)
                continue;

            point p;
            p.dist= dist;
            p.id= n;
            nNearest.push_back(p);
        }

        for(int j= 0; j< K; j++)
            Vertex[j]= adjacent[i*K + j];

        qbucket= bin_to_dec(Vertex, K);
    }
}

void Cube::query(vector<double> &query_, int M, int probes, int metric) {
    if(probes <= 0)
        return;

    nNearest.clear();
    bool query_to_cube[K];
    int qbucket= calc_fh(query_to_cube, f, K, query_, v, t, w, PIXELS);


    vector<bool> adjacent;
    int candidates= 0;
    // probes--;       // verteces - query vertex
    // M-= hypercube[0][qbucket].size();   // Total candidates - candidates in query vertex
    for(int i= 1; i<= K; i++) {
        perm(query_to_cube, hypercube, i, 0, 0, candidates, K, M, probes, adjacent);
        // Done if: at least M points have been selected,
        // or number of vertices checked (probes) has been reached
        if(adjacent.size()/K >= probes || candidates >= M)
            break;

    }

    vector<int> cube_nn;    // vector with all images from same bucket as query
    bool Vertex[K];     // adjacent vertices to query_to_cube
    for(int i= 0; i< adjacent.size()/K; i++) {
        cube_nn= hypercube[0][qbucket];     // Extract images from the same bucket as query

        for(int n: cube_nn) {
            double dist= p_norm(imgs[n], query_, PIXELS, metric);
            if(dist == 0)
                continue;

            point p;
            p.dist= dist;
            p.id= n;
            nNearest.push_back(p);
        }

        for(int j= 0; j< K; j++)
            Vertex[j]= adjacent[i*K + j];

        qbucket= bin_to_dec(Vertex, K);
    }
}


int bin_to_dec(bool img_to_cube[], int D) {
    int k= 0;
    for(int j= D-1; j>= 0; j--) {      // Convert {0,1} sequence into an integer (key)
        if(j == 0)
            k+= img_to_cube[j];
        else
            k+= (2*img_to_cube[j]<<(j-1));
    }

    return k;
}