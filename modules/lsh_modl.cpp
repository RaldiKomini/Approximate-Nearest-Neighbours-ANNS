#include "../include/lsh.h"


LSH_opt LSH_options(int argc, char** argv) {
    int opt;
    LSH_opt options;
    *(options.fil.filnam)= '\0';
    *(options.fil.qfil)= '\0';
    *(options.fil.outf)= '\0';
    options.K= 4;
    options.L= 5;
    options.N= 1;
    options.R= 10000;

    while( (opt= getopt(argc, argv, "d:q:k:L:o:N:R:")) != -1) {
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
            case 'L':
                options.L= atoi(optarg);
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
        cout<<"Give i/o or querry file\n";
        exit(1);
    }

    options.fil.sendto.open(options.fil.outf);
    
    return options;
}

LSH::LSH(vector<vector<uchar>> &imgs_, int IMAGES_, int PIXELS_, int r_, int w_, int K_, int L_):
    Method(imgs_, IMAGES_, PIXELS_, w_, K_), L(L_), r(r_), M(4294967291), TABLESIZE(IMAGES/8) {

    t= new double[L*K];  // double t[L*K];
    v.resize(L*K);
    for(int i= 0; i< L*K; i++)
        v[i].resize(PIXELS);

    hfunc_factors(w, t, v, L*K, PIXELS);
    
    htable= new unordered_map<int, vector<int>>[L];

    ID= new uint*[L]();     // "()" to initialize elements to 0
    for(int i= 0; i< L; i++) {
        ID[i]= new uint[IMAGES]();

    }

    for(int htid= 0; htid< L; htid++) {         // for each Hash Table,
        for(int i= 0; i< IMAGES; i++) {
            int g_p= calc_g(htid, K, imgs[i], v, t, w, PIXELS, r, ID[htid][i], M, TABLESIZE);     //  find  g(p)
            htable[htid][g_p].push_back(i);                 //  place p in bucket g(p)
        }
    }
}

LSH::~LSH() {
    for(int i= 0; i< L; i++)
        delete[] ID[i];

    delete[] ID;

    delete[] htable;
    delete[] t;
}

int LSH::calc_g(int htid, int K, vector<uchar> &imgs, vector<vector<double>> &v, double t[], int w, int dim, uint r, uint &ID, int M,  int tablesize) {
    for(int j= 0; j< K; j++) {
        uint h= calc_h(imgs, v[htid*K+j], t[htid*K+j], w, dim);
        h= h%M;
        ID+= (r*h)%M;    // ID(p)= sum(r*h(p)) mod M
    }
    int g_p= ID % tablesize;
    if(g_p < 0)
        g_p= -g_p;
    
    return g_p;
}


int LSH::calc_g(int htid, int K, vector<double> &imgs, vector<vector<double>> &v, double t[], int w, int dim, uint r, uint &ID, int M,  int tablesize) {
    for(int j= 0; j< K; j++) {
        uint h= calc_h(imgs, v[htid*K+j], t[htid*K+j], w, dim);
        h= h%M;
        ID+= (r*h)%M;    // ID(p)= sum(r*h(p)) mod M
    }
    int g_p= ID % tablesize;
    if(g_p < 0)
        g_p= -g_p;
    
    return g_p;
}

void LSH::query(vector<uchar> &query_, int metric) {
    nNearest.clear();

    uint qID[L]= {0};
    for(int htid= 0; htid< L; htid++) {     //  For each hash table
        int qg_p= calc_g(htid, K, query_, v, t, w, PIXELS, r, qID[htid], M, TABLESIZE);

        vector<int>nn= htable[htid][qg_p];      // vector with all images in same bucket as query

        for(int n: nn) {
            if(ID[htid][n] != qID[htid])        // check only neighbours with same ID as query (maybe no diff in time because majority has equal id to q)
                continue;
            double dist= p_norm(imgs[n], query_, PIXELS, metric);
            if(dist == 0)
                continue;

            point p;
            p.dist= dist;
            p.id= n;
            nNearest.push_back(p);
            
            if(nNearest.size() > 200*L)
                return;
        }
    }
}

void LSH::query(vector<double> &query_, int metric) {
    nNearest.clear();

    uint qID[L]= {0};
    for(int htid= 0; htid< L; htid++) {     //  For each hash table
        int qg_p= calc_g(htid, K, query_, v, t, w, PIXELS, r, qID[htid], M, TABLESIZE);

        vector<int>nn= htable[htid][qg_p];      // vector with all images in same bucket as query

        for(int n: nn) {
            if(ID[htid][n] != qID[htid])        // check only neighbours with same ID as query (maybe no diff in time because majority has equal id to q)
                continue;
            double dist= p_norm(imgs[n], query_, PIXELS, metric);
            if(dist == 0)
                continue;

            point p;
            p.dist= dist;
            p.id= n;
            nNearest.push_back(p);

            if(nNearest.size() > 200*L)
                return;
        }
    }
}