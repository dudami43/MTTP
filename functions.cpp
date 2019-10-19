#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

/**
 *Busca Local
**/
double first_improvement_swap(Instance& inst)
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

double first_improvement_trade_ungotted(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief = rand() % inst.thieves.size();

    // Procura dois itens que melhoram a solucao atual
    for(int i = 1; i < inst.items.size(); i++)
    {
        for(int j = i + 1; j < inst.items.size(); j++)
        {
            // Salva o estado da instancia
            initial_instance = inst;

            // Muda para o proximo vizinho
            inst.exchange_items(thief, i, j);

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

double first_improvement_move(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief = rand() % inst.thieves.size();

    // Procura uma posicao para uma cidade, de maneira a melhorar a solucao
    for(int i = 1; i < inst.thieves[thief].second.route.size(); i++)
    {
        for(int j = 1; j < inst.thieves[thief].second.route.size(); j++)
        {
            // Caso a nova posicao seja a mesma posicao, passa para a proxima tentativa
            if(i == j) continue;

            // Salva o estado da instancia
            initial_instance = inst;
            
            // Muda para o proximo vizinho
            inst.move_cities(thief, i, j);
            
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

double first_improvement_trade_thieves(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief_1 = rand() % inst.thieves.size();
    int thief_2 = rand() % inst.thieves.size();

    // Faz a troca de itens entre os ladroes
    for(int i = 0; i < inst.thieves[thief_1].second.items.size(); i++)
    {
        for(int j = 0; j < inst.thieves[thief_2].second.items.size(); j++)
        {
            // Salva o estado da instancia
            initial_instance = inst;

            std::cout << "entrou - ";
            
            // Muda para o proximo vizinho
            inst.swap_items_btw_thieves(thief_1, thief_2, i, j);
            
            std::cout << "saiu" << std::endl;

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

double localSearch(Instance& inst, std::string method)
{
    if(method.compare("swap"))
    {
        return first_improvement_swap(inst);
    }
    else if(method.compare("move"))
    {
        return first_improvement_move(inst);
    }
    else if(method.compare("trade_ungotted"))
    {
        return first_improvement_trade_ungotted(inst);
    }
    else if(method.compare("trade_btw_thieves"))
    {
        return first_improvement_trade_thieves(inst);
    }
}

double call_localSearch(Instance& instance, std::string method)
{
    double objectiveFunction = instance.objectiveFunction();
    double best = objectiveFunction;
    int cont = 0;
    while(true)
    {
        double result = localSearch(instance, method);
        if(result > best) 
        {
            cont = 0;
            best = result;
        }
        else cont++;
        if(cont >= 10) break;
    }
    return best;
/**
 * Heuristicas
 **/

double VNS(Instance& inst, int max_disturbance, bool verbose)
{

    // Inicializa o vector de vizinhanças
    std::vector<std::string> neighborhoods;
    neighborhoods.push_back("trade_ungotted");
    neighborhoods.push_back("swap");
    neighborhoods.push_back("move");
    neighborhoods.push_back("trade_btw_thieves");

    // Gera solucao inicial
    inst.greedySolution();

    // Seta instancia auxiliar
    Instance aux_inst;
    aux_inst = inst;

    // Inicializa variaveis da vizinhança e da melhor solucao
    int neighborhood = 0, n_disturbance = 0;
    double best_value = inst.objectiveFunction(), current_value;

    while(true)
    {
        // Aplica busca local com a vizinhanca atual
        /* for(auto thief: inst.thieves){
            std::cout << "Numero de itens: " << thief.second.items.size() << std::endl;
            std::cout << "Numero de pesos: " << thief.second.backpack_weight.size() << std::endl;
            std::cout << "Numero de cidades: " << thief.second.route.size() << std::endl;
        }
        std::cout << "start busca" << std::endl; */
        current_value = localSearch(inst, neighborhoods[neighborhood]);
        //std::cout << "end busca" << std::endl << std::endl;

        // if(verbose) std::cout << "Valor atual: " << current_value << std::endl;

        // Verifica se melhorou a solucao
        if(current_value > best_value) 
        {
            // Se melhorou, volta para a vizinhanca inicial
            best_value = current_value;
            aux_inst = inst;
            neighborhood = 0;
        }
        else
        {
            // Se piorou, tenta a proxima vizinhanca
            inst = aux_inst;
            neighborhood++;
        }

        // Caso n haja melhora na solucao, e a vizinhanca eh a ultima, entao perturba a solucao
        if(neighborhood == neighborhoods.size()){
            // Caso o numero de perturbacoes tenha chegado ao limite, entao termina o algoritmo
            if(n_disturbance >= max_disturbance) break;
            n_disturbance++;

            // Reseta a vizinhanca
            aux_inst = inst;
            neighborhood = 0;

            // Perturba
            inst.disturbe(5);
        }
    }

    return best_value;
}