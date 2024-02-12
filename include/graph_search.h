#include "gnns.h"
#include "mrng.h"


typedef struct {
    Files fil;
    int K;  // neighbours per point
    int E;  // neighbour candidates per point
    int R;  // random starts
    int N;  // nearest neighbours
    int l;  // nearest neighbour candidates for mrng
    int method; // GNNS or MRNG
}Graph_opt;

Graph_opt Graph_options(int argc, char** argv);

void GNNS_method(Graph_opt& opt);
void MRNG_method(Graph_opt& opt);