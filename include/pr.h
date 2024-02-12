#ifndef PR_H
#define PR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <unordered_map>
#include <chrono>
#include "unistd.h"     //  getopt()
#include <string.h>
#include <math.h>


using namespace std::chrono;
using namespace std;

typedef unsigned char uchar;
typedef unsigned int uint;

typedef struct{
    double dist;
    int id;
}point;


/*  Basic Functions  */
//  Distance between vectors p,q
double p_norm(vector<uchar> p, vector<uchar> q, int dim, int norm);
double p_norm(vector<uchar> p, vector<double> q, int dim, int norm);
double p_norm(vector<double> p, vector<double> q, int dim, int norm);

//  Dot Product
double dot_prod(vector<uchar> a, vector<uchar> b, int dim);
double dot_prod(vector<int> a, vector<int> b, int dim);
double dot_prod(vector<uchar> a, vector<double> b, int dim);
double dot_prod(vector<double> a, vector<double> b, int dim);

// Reverse bit order from Little to Big Endian
int reverseInt(int i);

/*      mnist dataset read functions      */
void meta_read(fstream &fin, int* meta);
void image_read(fstream &fin, int pixels, vector<uchar> &imgs);
void get_labels(char* file, int total_labels, uchar* labels);

#endif /* PR_H */