#include "../include/pr.h"

double p_norm(vector<uchar> p, vector<uchar> q, int dim, int norm) {
    double sum= 0;
    double dif;
    for(int i= 0; i< dim; i++){
        dif = (double)p[i]-(double)q[i];
        if(dif < 0)
            dif = -dif;
        for(int i= 1; i< norm; i++)     // faster than pow for small norm
            dif*= dif;
        sum+= dif;
    }
    double dist= pow(sum, 1.0/norm);
    return dist;
}

double p_norm(vector<uchar> p, vector<double> q, int dim, int norm) {
    double sum= 0;
    double dif;
    for(int i= 0; i< dim; i++){
        dif = (double)p[i]-(double)q[i];
        if(dif < 0)
            dif = -dif;
        for(int i= 1; i< norm; i++)
            dif*= dif;
        sum+= dif;
    }
    double dist= pow(sum, 1.0/norm);
    return dist;
}

double p_norm(vector<double> p, vector<double> q, int dim, int norm) {
    double sum= 0;
    double dif;
    for(int i= 0; i< dim; i++){
        dif = (double)p[i]-(double)q[i];
        if(dif < 0)
            dif = -dif;
        for(int i= 1; i< norm; i++)
            dif*= dif;
        sum+= dif;
    }
    double dist= pow(sum, 1.0/norm);
    return dist;
}

double dot_prod(vector<uchar> a, vector<uchar> b, int dim) {
    double prod= 0;
    for(int i= 0; i< dim; i++)
        prod+= (int)a[i] * (int)b[i];    // cast uchar to int to work

    return prod;
}

double dot_prod(vector<int> a, vector<int> b, int dim) {
    double prod= 0;
    for(int i= 0; i< dim; i++)
        prod+= a[i] * b[i];    // cast uchar to int to work

    return prod;
}

double dot_prod(vector<uchar> a, vector<double> b, int dim) {
    double prod= 0;
    for(int i= 0; i< dim; i++)
        prod+= (int)a[i] * b[i];    // cast uchar to int to work

    return prod;
}

double dot_prod(vector<double> a, vector<double> b, int dim) {
    double prod= 0;
    for(int i= 0; i< dim; i++)
        prod+= a[i] * b[i];

    return prod;
}

int reverseInt(int i) {
    return (i >> 24 & 0xff) + (i >> 8 &  0xff00) + (i << 8 & 0xff0000) + (i << 24 & 0xff000000);
}

void meta_read(fstream &fin, int* meta) {
    for(int i= 0; i< 4; i++) {
        fin.read((char*)&meta[i], sizeof(int)); // metadata extraction
        meta[i]= reverseInt(meta[i]);   // from little to big endian
    }
}

void image_read(fstream &fin, int pixels, vector<uchar> &imgs) {
    uchar pixel;
    for (int i= 0; i< pixels; i++) {
        fin.read((char*)&pixel, sizeof(uchar));     // extract 784 pixels for each image
        imgs[i]= pixel;
    }
}

void get_labels(char* file, int total_labels, uchar* labels) {
    fstream fin;
    fin.open(file, ios::in | ios::binary);

    int Lmeta[2];    // metadata: 0->magic, 1->#files, 2->#rows, 3->#columns
    for(int i= 0; i< 2; i++) {
        fin.read((char*)&Lmeta[i], sizeof(int)); // metadata extraction
        // Lmeta[i]= reverseInt(Lmeta[i]);   // from little to big endian
        // cout<<Lmeta[i]<<"\n";
    }

    for (int i= 0; i < total_labels; i++) {
        fin.read((char*)&labels[i], sizeof(uchar));
    }
    fin.close();
}