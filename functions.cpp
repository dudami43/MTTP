#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

/**
 *Busca Local
**/
double localSearch(Instance& inst)
{
    double current_value = inst.objectiveFunction();
    std::cout << current_value << std::endl;
    double best_value = current_value;
    Instance prev_inst;
    bool improving = true;
    int tries = 0;
    while(improving && tries < 10)
    {
        prev_inst = inst;
        inst.swap_cities();
        if(inst.solutionValid())
        {
            current_value = inst.objectiveFunction();
            
            if(current_value > best_value)
            {
                best_value = current_value;
            }
            else
            {
                improving = false;
            }
        }
        else
        {
            prev_inst = inst;
            tries++;
        }
    }
    
    return best_value;
}