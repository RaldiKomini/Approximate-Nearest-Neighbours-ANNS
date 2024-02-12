#include "../include/hypercube.h"

int main(int argc, char* argv[]) {
    Cube_opt opt= Cube_options(argc, argv);
    //#########################     PREPROCESSING      ######################
    fstream fin;
    
    fin.open(opt.fil.filnam, ios::in | ios::binary);

    if(!fin) {
        perror("open");
        exit(1);
    }
    int meta[4];    // metadata: 0->magic, 1->#images, 2->#pixels_row, 3->#pixels_cols
    meta_read(fin, meta);
    int images= meta[1];
    int pixels= meta[2]*meta[3];    //  pixels per image

    vector<vector<uchar>> imgs(images);   // 2d vector, where for each image/point: 784 pixels (60000*784)
    for (int i= 0; i < images; i++) {
        imgs[i]= vector<uchar>(pixels);
        image_read(fin, pixels, imgs[i]);
    }
    fin.close();

    srand(time(NULL));

    
    uint w= 500;
    Cube cube(imgs, images, pixels, w, opt.K);

    //#########################     QUERY      ######################
    fstream qfin;
    
    qfin.open(opt.fil.qfil, ios::in | ios::binary);

    if(!qfin) {
        perror("open");
        exit(1);
    }
    int qmeta[4];    // metadata: 0->magic, 1->#files, 2->#rows, 3->#columns
    meta_read(qfin, qmeta);
    
    vector<uchar> query(pixels);   // query image
    for(int i= 0; i< 10; i++) {
        image_read(qfin, pixels, query);
        opt.fil.sendto<<"Query: "<<i+1<<endl;

        /*  BRUTE   FORCE   */
        chrono::steady_clock brute;

        auto brute_start= brute.now();
        vector<point> NN= brute_nearest(imgs, query, opt.N, 2);
        auto brute_stop= brute.now();
        
        auto brute_time= static_cast<chrono::duration<double>>(brute_stop- brute_start);


        /*  Hypercube  */
        chrono::steady_clock approx;
        auto start= approx.now();
        
        cube.query(query, opt.M, opt.probes, 2);
        vector<point> AN= cube.nearest_search(opt.N);    // Aproximate Nearest
        
        auto stop= approx.now();
        auto approx_time= static_cast<chrono::duration<double>>(stop- start);

        for(int n= 0; n< opt.N; n++) {
            opt.fil.sendto<<"Nearest neighbor-"<<n+1<<": "<<AN[n].id<<endl;
            opt.fil.sendto<<"distanceCube: "<<AN[n].dist<<endl;
            opt.fil.sendto<<"distanceTrue: "<<NN[n].dist<<endl;
        }

        opt.fil.sendto<<"tCube: "<<(double)approx_time.count()<<"sec"<<endl;
        opt.fil.sendto<<"tTrue: "<<(double)brute_time.count()<<"sec"<<endl;
        
        opt.fil.sendto<<"R-near neighbors:\n";
        vector<int> in_Radius= cube.range_search(opt.R);
        for(int i= 0; i< in_Radius.size(); i++) {
            opt.fil.sendto<<in_Radius[i]<<endl;
        }
        opt.fil.sendto<<"_____________\n";
    }
    qfin.close();
    opt.fil.sendto.close();
    return 0;
}