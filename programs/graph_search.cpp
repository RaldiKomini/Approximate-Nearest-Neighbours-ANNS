#include "../include/graph_search.h"


int main(int argc, char* argv[]) {
    Graph_opt opt= Graph_options(argc, argv);

    if(opt.method == 1)
        GNNS_method(opt);
    else if(opt.method == 2)
        MRNG_method(opt);


    return 0;
}