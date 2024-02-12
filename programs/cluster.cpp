#include "../include/cluster.h"


int main(int argc, char* argv[]) {
    Cluster_opt options= Cluster_options(argc, argv);
    //#########################     PREPROCESSING      ######################
    fstream fin;

    fin.open(options.fil.filnam, ios::in | ios::binary);

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
    /*      INITIALIZE  kmeans++    */
    int K= options.K;   // # Clusters

    vector<vector<double>> cK(K);    // Hold K centroids
    vector<vector<int>> team(K);     // for each cluster, insert image closest to it
    kmeans(imgs, images, pixels, cK, team, K, 2);



    //  Universally accepted maximum loops/epochs    
    int max_loop= 30;
    
    /*  Lloyd's Assignment   */
    chrono::steady_clock approx;
    auto start= approx.now();
    if(!strcmp(options.method, "Classic")) {
        options.fil.sendto<<"Algorithm: Lloyds\n";
        lloyds(imgs, team, cK, 2);
    }
    /*  Reverse LSH     */
    else if(!strcmp(options.method, "LSH")) {
        options.fil.sendto<<"Algorithm: Range Search LSH\n";
        rev_lsh(imgs, team, cK, options.k, options.L, 2);
    }
    else if(!strcmp(options.method, "Hypercube")) {
        options.fil.sendto<<"Algorithm: Range Search Hypercube\n";
        rev_cube(imgs, team, cK, options.D, options.M, options.probes, 2);
    }
    /*      END REV_CUBE     */
    else {
        options.fil.sendto<<"Give valid method (Clasic, LSH or Hypercube)\n";
        exit(1);
    }
    auto stop= approx.now();
    auto approx_time= static_cast<chrono::duration<double>>(stop- start);
    options.fil.sendto<<"clustering time: "<<(double)approx_time.count()<<"sec\n";
    cluster_stats(team, cK, pixels, options);

    Silhouette(imgs, team, cK, K, images, pixels, options, 2);
    if(options.Complete == 1)
        cluster_more_stats(team, cK, pixels, options);
    
    options.fil.sendto.close();
    return 0;
}