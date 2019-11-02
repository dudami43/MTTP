#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"
#include "functions.h"

// Para compilar com g++11: g++ -std=c++11 main.cpp parser.cpp functions.cpp

int main(int argc, char **argv)
{
    double rng_seed;

    int ir_max;
    double size_rcl;

    int max_disturbance; 
    int n_disturbe;

    int pos_alg; //1 grasp 2 vns

    for(int i = 2; i < argc; i += 2) {
        if(strcmp(argv[i], "--seed") == 0) {
            sscanf(argv[i+1],"%lf", &rng_seed);
        }
        else if(strcmp(argv[i], "--IR_GRASP") == 0) {
            sscanf(argv[i+1],"%d", &ir_max);
        }
        else if(strcmp(argv[i], "--SIZE_RCL") == 0) {
            sscanf(argv[i+1],"%lf", &size_rcl);
        }
        else if(strcmp(argv[i], "--MAX_DIST") == 0) {
            sscanf(argv[i+1],"%d", &max_disturbance);
        }
        else if(strcmp(argv[i], "--N_DIST") == 0) {
            sscanf(argv[i+1],"%d", &n_disturbe);
        }
        if(strcmp(argv[i], "--alg") == 0) {
            pos_alg = i+1;
        }
    }

    // Inicializa a funcao randomica
    srand (rng_seed);    

    Instance instance;
    try
    {
        instance = get_instance(argv[1]);
        instance.thieves.clear();
        instance.addThief(1);
        if(strcmp(argv[pos_alg], "grasp") == 0)
        {
            std::cout << grasp(instance, 20, ir_max, size_rcl, false);
        }
        else if(strcmp(argv[pos_alg], "vns") == 0)
        {
            std::cout << VNS(instance, max_disturbance, n_disturbe);
        }
        
    }
    catch (char const *param)
    {
        std::cout << param << std::endl;
    }

    return 0;
}