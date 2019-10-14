#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

/**
 *Busca Local
**/
double localSearch(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief = rand() % inst.thieves.size();

    // Procura duas cidades que melhoram a solucao atual
    for(int i = 1; i < inst.thieves[thief].second.route.size(); i++)
    {
        for(int j = i + 1; j < inst.thieves[thief].second.route.size(); j++)
        {
            // Salva o estado da instancia
            initial_instance = inst;

            // Muda para o proximo vizinho
            inst.swap_cities(thief, i, j);
            
            // Valida solucao
            if(inst.solutionValid())
            {
                // Avalia a nova solucao
                current_value = inst.objectiveFunction();
                
                // Verifica se eh melhor que a atual
                if(current_value > best_value)
                {
                    return current_value;
                }
            }

            // Caso nao melhore ou a nova solucao n seja valida, retorne a solucao inicial
            inst = initial_instance;
        }
    }
    
    return best_value;
}