#include "../include/graph_search.h"

Graph_opt Graph_options(int argc, char** argv) {
    int opt;
    Graph_opt options;
    *(options.fil.filnam)= '\0';
    *(options.fil.qfil)= '\0';
    *(options.fil.outf)= '\0';

    options.K= 50;
    options.E= 30;
    options.R= 1;
    options.N= 1;
    options.l= 20;
    options.method= 0;

    while( (opt= getopt(argc, argv, "d:q:k:E:R:N:l:m:o:")) != -1) {
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
            case 'E':
                options.E= atoi(optarg);
                break;
            case 'R':
                options.R= atoi(optarg);
                break;
            case 'N':
                options.N= atoi(optarg);
                break;
            case 'l':
                options.l= atoi(optarg);
                break;
            case 'm':
                options.method= atoi(optarg);
                break;
            case 'o':
                strcpy(options.fil.outf, optarg);
                break;
        }
    }
    if(*(options.fil.filnam)== '\0' || *(options.fil.qfil)== '\0' || *(options.fil.outf)== '\0') {
        cout<<"Give i/o or querry file\n";
        exit(1);
    }
    if(options.method == 0) {
        cout<<"Give method (1: GNNS, 2: MRNG)\n";
        exit(1);
    }
    if(options.E > options.K) {
        cout<<"parameters E > K were given, but E must be at most equal to K. Giving E=K\n";
        options.E= options.K;
    }
    if(options.l < options.N) {
        cout<<"parameters l < N were given, but l must be at least equal to N. Giving l=N\n";
        options.l= options.N;
    }

    options.fil.sendto.open(options.fil.outf);
    
    return options;
}





//  ##########################################
//  ###############    GNNS    ###############
//  ##########################################

void GNNS_method(Graph_opt& opt) {
    fstream fin;

    fin.open(opt.fil.filnam, ios::in | ios::binary);

    if(!fin) {
        perror("open");
        exit(1);
    }
    int meta[4];    // metadata: 0->magic, 1->#images, 2->#pixels_row, 3->#pixels_cols
    meta_read(fin, meta);

    const int images= meta[1];
    int pixels= meta[2]*meta[3];    //  pixels per image

    vector<vector<uchar>> imgs(images);   // 2d vector, where for each image/point: 784 pixels (60000*784)
    for (int i= 0; i < images; i++) {
        imgs[i]= vector<uchar>(pixels);
        image_read(fin, pixels, imgs[i]);
    }
    fin.close();

    srand(time(NULL));

    GNNS gnns(imgs, images, pixels, opt.K);


    //#########################     QUERY      ######################
    char msg[64];   //  read query file from command line, at end of the loop
    strcpy(msg, opt.fil.qfil);
    do {

        fstream qfin;
        qfin.open(msg, ios::in | ios::binary);

        if(!qfin) {
            perror("open");
            exit(1);
        }
        int qmeta[4];    // metadata: 0->magic, 1->#files, 2->#rows, 3->#columns
        meta_read(qfin, qmeta);

        double MAF= MAXFLOAT;  // for calculating MAF
        double MAF_avg= 0;
        chrono::steady_clock approx;
        double ttime_approximate= 0;    // tAverageApproximate

        chrono::steady_clock brute;
        double ttime_brute= 0;      // tTrueApproximate

        vector<uchar> query(pixels);   // query image
        int num_of_queries = 10;
        int T= 50;  // greedy steps
        for(int q= 0; q< num_of_queries; q++) {
            image_read(qfin, pixels, query);
            opt.fil.sendto<<"Query "<<q<<":\n";

            //  GNNS
            auto start= approx.now();
            gnns.query(query, 2, opt.E, opt.R, T);
            vector<point> AN= gnns.nearest_search(10);
            auto stop= approx.now();

            auto approx_time= static_cast<chrono::duration<double>>(stop- start);
            ttime_approximate+= (double)approx_time.count();

            //  Brute Force
            start= brute.now();
            vector<point> NN= brute_nearest(imgs, query, opt.N, 2);
            stop= brute.now();

            ttime_brute += (double)(static_cast<chrono::duration<double>>(stop- start).count());

            //  Find MAF
            double temp= AN[0].dist/NN[0].dist;
            MAF_avg+= temp;
            if(temp < MAF)
                MAF= temp;

            for(int n= 0; n< opt.N; n++) {
                opt.fil.sendto<<"Nearest neighbor-"<<n+1<<": "<<AN[n].id<<'\n';
                opt.fil.sendto<<"distanceGKNN: "<<AN[n].dist<<'\n';
                opt.fil.sendto<<"distanceTrue: "<<NN[n].dist<<'\n';
            }
        }
        qfin.close();

        opt.fil.sendto<<"\ntAverageApproximate: "<<ttime_approximate/num_of_queries<<" sec" << endl;
        opt.fil.sendto<<"tAverageTrue: "<<ttime_brute/num_of_queries<<" sec" << endl;
        opt.fil.sendto << "MAF: " << MAF << endl;
        opt.fil.sendto << "MAF Average: " << MAF_avg/num_of_queries << endl;
        
        cout<<"Give query file or exit with \"No\""<<endl;
        cin>>msg;
    }while(strcmp(msg, "No"));
}





//  ##########################################
//  ###############    MRNG    ###############
//  ##########################################

void MRNG_method(Graph_opt& opt) {
    fstream fin;

    fin.open(opt.fil.filnam, ios::in | ios::binary);

    if(!fin) {
        perror("open");
        exit(1);
    }
    int meta[4];    // metadata: 0->magic, 1->#images, 2->#pixels_row, 3->#pixels_cols
    meta_read(fin, meta);
    
    const int images= meta[1];
    int pixels= meta[2]*meta[3];    //  pixels per image

    vector<vector<uchar>> imgs(images);   // 2d vector, where for each image/point: 784 pixels (60000*784)
    for (int i= 0; i < images; i++) {
        imgs[i]= vector<uchar>(pixels);
        image_read(fin, pixels, imgs[i]);
    }
    fin.close();

    srand(time(NULL));

    MRNG mrng(imgs, images, pixels);


    //#########################     QUERY      ######################
    char msg[64];   //  read query file from command line, at end of the loop
    strcpy(msg, opt.fil.qfil);
    do {

        fstream qfin;
        qfin.open(opt.fil.qfil, ios::in | ios::binary);

        if(!qfin) {
            perror("open");
            exit(1);
        }
        int qmeta[4];    // metadata: 0->magic, 1->#files, 2->#rows, 3->#columns
        meta_read(qfin, qmeta);

        double MAF= MAXFLOAT;  // for calculating MAF
        double MAF_avg= 0;
        chrono::steady_clock approx;
        double ttime_approximate= 0;    // tAverageApproximate

        chrono::steady_clock brute;
        double ttime_brute= 0;      // tTrueApproximate

        vector<uchar> query(pixels);   // query image
        int num_of_queries = 10;
        int T= 50;
        for(int q= 0; q< num_of_queries; q++) {
            image_read(qfin, pixels, query);
            opt.fil.sendto<<"Query "<<q<<":\n";

            //  MRNG
            auto start= approx.now();
            mrng.query(query, opt.l);
            vector<point> AN= mrng.nearest_search(opt.N);
            auto stop= approx.now();

            auto approx_time= static_cast<chrono::duration<double>>(stop- start);
            ttime_approximate+= (double)approx_time.count();

            //  Brute Force
            start= brute.now();
            vector<point> NN= brute_nearest(imgs, query, opt.N, 2);
            stop= brute.now();

            ttime_brute += (double)(static_cast<chrono::duration<double>>(stop- start).count());

            //  Find MAF
            double temp= AN[0].dist/NN[0].dist;
            MAF_avg+= temp;
            if(temp < MAF)
                MAF= temp;

            for(int n= 0; n< opt.N; n++) {
                opt.fil.sendto<<"Nearest neighbor-"<<n+1<<": "<<AN[n].id<<'\n';
                opt.fil.sendto<<"distanceMRNG: "<<AN[n].dist<<'\n';
                opt.fil.sendto<<"distanceTrue: "<<NN[n].dist<<'\n';
            }
        }
        qfin.close();

        opt.fil.sendto<<"\ntAverageApproximate: "<<ttime_approximate/num_of_queries<<" sec" << endl;
        opt.fil.sendto<<"tAverageTrue: "<<ttime_brute/num_of_queries<<" sec" << endl;
        opt.fil.sendto << "MAF: " << MAF << endl;
        opt.fil.sendto << "MAF Average: " << MAF_avg/num_of_queries << endl;

        cout<<"Give query file or exit with \"No\""<<endl;
        cin>>msg;
    }while(strcmp(msg, "No"));
}