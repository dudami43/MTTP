#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

using namespace std::chrono;

/**
 * Funcoes auxiliares
**/
// Pega o proximo item de um ladrao e retorna a posicao que sera incrementada na proxima iteracao
bool get_next(Instance& inst, std::vector<int>& choosed_thieves, std::vector<int>& actual_items)
{  
    int i = 0;
    for(; i < actual_items.size(); i++)
    {
        if( actual_items[i] < (inst.thieves[choosed_thieves[i]].items.size() - 1) )
        {
            actual_items[i]++;
            break;
        }
    }

    if(i == actual_items.size())
    {
        return false;
    }

    for(int j = i - 2; j >= 0 && i > 0; j--)
    {
        actual_items[i] = 0;
    }

    return true;
}

int number_of_empty_thieves(Instance& inst){
    int n_empty_thieves = 0;
    for(int i = 0; i < inst.thieves.size(); i++){
        if(inst.thieves[i].items.size() == 0) n_empty_thieves++;

    }
    return n_empty_thieves;
}

/**
 * Busca Local
**/
double first_improvement_shuffle(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance = inst;

    int n_thieves = 1;

    int n_empty_thieves = number_of_empty_thieves(inst);

    if(n_empty_thieves <= 1) return best_value;
    else if(n_empty_thieves == 2) n_thieves = 2;
    else 
    {
       n_thieves = rand() % n_empty_thieves;
       while(n_thieves < 2) n_thieves = rand() % n_empty_thieves;
    }
    
    std::vector<int> choosed_thieves;
    choosed_thieves.assign(n_thieves, 0);
    bool equals, empty_thief;
    for(int i = 0; i < n_thieves; i++)
    {
        equals = true;
        while(equals or empty_thief)
        {
            equals = false;
            empty_thief = false;

            choosed_thieves[i] = rand() % inst.thieves.size();
            if(inst.thieves[choosed_thieves[i]].items.size() == 0)
                empty_thief = true;

            for(int j = 0; j < i; j++)
            {
                if(choosed_thieves[i] == choosed_thieves[j])
                    equals = true;        
            }
        }
    }

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    std::vector<int> items_position;
    items_position.assign(choosed_thieves.size(), 0);
    while(true){

        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        inst.shuffles_thieves_items(n_thieves, choosed_thieves, items_position);

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
        if(!get_next(inst, choosed_thieves, items_position)) break;
    }
    
    return best_value;
}

double first_improvement_swap(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int thief = rand() % inst.thieves.size();

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    // Procura duas cidades que melhoram a solucao atual
    for(int i = 1; i < inst.thieves[thief].route.size(); i++)
    {
        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        for(int j = i + 1; j < inst.thieves[thief].route.size(); j++)
        {
            if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

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

double first_improvement_move(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int thief = rand() % inst.thieves.size();

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    // Procura uma posicao para uma cidade, de maneira a melhorar a solucao
    for(int i = 1; i < inst.thieves[thief].route.size(); i++)
    {
        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        for(int j = 1; j < inst.thieves[thief].route.size(); j++)
        {
            if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

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

double first_improvement_trade_ungotten(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief = rand() % inst.thieves.size();

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    // Procura dois itens que melhoram a solucao atual
    for(int i = 1; i < inst.thieves[thief].items.size(); i++)
    {
        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        for(int j = i + 1; j < inst.items.size(); j++)
        {
            if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

            // Salva o estado da instancia
            initial_instance = inst;

            // Muda para o proximo vizinho
            inst.new_exchange(thief, i, j);

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

double first_improvement_remove_item(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int thief = rand() % inst.thieves.size();

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    // Procura duas cidades que melhoram a solucao atual
    for(int j = 0; j < inst.thieves[thief].items.size(); j++)
    {
        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        // Salva o estado da instancia
        initial_instance = inst;
        
        // Remove o item
        inst.remove_item(thief, j);

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
    
    return best_value;
}

double first_improvement_add_item(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int thief = rand() % inst.thieves.size();

    high_resolution_clock::time_point t_i_busca = high_resolution_clock::now();

    // Procura duas cidades que melhoram a solucao atual
    for(int j = 0; j < inst.items.size(); j++)
    {
        if(duration_cast<duration<double>>(high_resolution_clock::now() - t_i_busca).count() > 200) break;

        if(inst.caught_items[j] == 0)
        {
            // Salva o estado da instancia
            initial_instance = inst;
            
            // Remove o item
            inst.add_item(thief, j, true);

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
    if(method.compare("shuffle") == 0)
    {
        return first_improvement_shuffle(inst);
    }
    else if(method.compare("swap") == 0)
    {
        return first_improvement_swap(inst);
    }
    else if(method.compare("move") == 0)
    {
        return first_improvement_move(inst);
    }
    else if(method.compare("items") == 0)
    {
        return first_improvement_trade_ungotten(inst);
    }else if(method.compare("remove") == 0)
    {
        return first_improvement_remove_item(inst);
    }else if(method.compare("add") == 0)
    {
        return first_improvement_add_item(inst);
    } 
}

/**
 * Heuristicas
 **/

// VNS
double VNS(Instance& inst, int max_disturbance, int n_disturbe, bool verbose)
{
    // Adquire o tempo inicial
    high_resolution_clock::time_point t_i = high_resolution_clock::now();

    // Inicializa o vector de vizinhanças
    std::vector<std::string> neighborhoods;
    neighborhoods.push_back("items");
    neighborhoods.push_back("move");
    neighborhoods.push_back("shuffle");
    neighborhoods.push_back("swap");
    neighborhoods.push_back("remove");
    neighborhoods.push_back("add");

    // Gera solucao inicial
    inst.greedySolution();

    // Seta instancia auxiliar
    Instance aux_inst;
    aux_inst = inst;

    // Inicializa variaveis da vizinhança e da melhor solucao
    int neighborhood = 0, n_disturbance = 0;
    double best_value = inst.objectiveFunction(), current_value;

    // Enquanto nao passou 10 minutos
    while(duration_cast<duration<double>>(high_resolution_clock::now() - t_i).count() < 600)
    {
        current_value = localSearch(inst, neighborhoods[neighborhood]);

        if(verbose) std::cout << "Valor atual: " << current_value << std::endl;

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

            // Reseta a vizinhanca
            neighborhood = 0;

            // Perturba
            if(!inst.disturbe(n_disturbe)) break;
            n_disturbance++;
        }
    }

    return best_value;
}

// GRASP
void random_greedy(Instance& instance, int ir_max, double size_rlc)
{
    instance.cleanSolution();

    std::vector<std::vector<int>> best_options(instance.thieves.size());
    std::vector<int> better_item(instance.thieves.size());
    int current_city = 0;
    std::vector<int> aux_caught_items(instance.items.size(), 0);

    for(int j = 0; j < instance.thieves.size(); j++)
    {
        instance.thieves[j].route.push_back(current_city);
        instance.thieves[j].backpack_weight.push_back(0);
        best_options[j].assign(instance.items.size() * size_rlc, -1);
    }
    
    bool items_available = true;
    int iterations = 0;
    while(items_available && iterations < ir_max)
    {
        for(int i = 0; i < instance.thieves.size(); i++)
        {
            double smallest = -1;
            int smallest_index = 0;
            items_available = false;
            for(int j = 0; j < instance.items.size(); j++)
            {
                if(aux_caught_items[j] == 0 && ((instance.items[j].weight + instance.used_capacity) <= instance.max_capacity))
                {
                    items_available = true;
                    double cost_benefit = instance.items[j].value;
                    if(instance.cities_distance[current_city][instance.items[j].city_idx] != 0)
                    {
                        cost_benefit = instance.items[j].value/instance.cities_distance[current_city][instance.items[j].city_idx];
                    }
                    if(cost_benefit > smallest)
                    {
                        best_options[i][smallest_index] = j;
                        smallest = best_options[i][0];
                        for(int k = 0; k < best_options[i].size(); k++)
                        {
                            if(smallest > best_options[i][k])
                            {
                                smallest = best_options[i][k];
                                smallest_index = k;
                            }
                        }                    
                    }
                }
            }
            int get_this = rand() % best_options[i].size();

            while(get_this == -1 && items_available)
            {
                get_this = rand() % best_options[i].size();
            }

            current_city = instance.items[best_options[i][get_this]].city_idx;
            aux_caught_items[best_options[i][get_this]] = 1;
            instance.add_item(i, best_options[i][get_this], true);

            if(!items_available) break;
        }
        iterations++;
    }
    while(items_available)
    {
        for(int i = 0; i < instance.thieves.size(); i++)
        {
            float best_val = 0;
            int best_index = 0;
            float cur_val = 0;
            for(int j = 0; j < instance.items.size(); j++)
            {
                if(aux_caught_items[j] == 0)
                {
                    if(instance.cities_distance[current_city][instance.items[j].city_idx] != 0) 
                        cur_val = instance.items[j].value / instance.cities_distance[current_city][instance.items[j].city_idx];
                    else
                        cur_val = instance.items[j].value;
                    if(cur_val > best_val)
                    {
                        best_val = cur_val;
                        best_index = j;
                    }
                    items_available = true;
                }
                else
                {
                    items_available = false;
                }
            }
            better_item[i] = best_index;
            aux_caught_items[best_index] = 1;

            if(instance.items[best_index].weight < instance.max_capacity - instance.used_capacity)
            {
                current_city = instance.items[best_index].city_idx;
                // Pega o item
                instance.caught_items[best_index] = 1;
                instance.used_capacity += instance.items[best_index].weight;

                // Adiciona o item a mochila do ladrao
                instance.thieves[i].items.push_back(best_index);
                
                // Adiciona a cidade a rota
                auto pos = std::find(instance.thieves[i].route.begin(), instance.thieves[i].route.end(), instance.items[best_index].city_idx);
                if(pos == instance.thieves[i].route.end())
                {
                    instance.thieves[i].route.push_back(current_city);
                    instance.thieves[i].backpack_weight.push_back(instance.items[best_index].weight);
                }
                else
                {
                    int index = std::distance(instance.thieves[i].route.begin(), pos);
                    instance.thieves[i].backpack_weight[index] += instance.items[best_index].weight;
                }
            }
        }    
    }
}

void random_greedy2(Instance& instance, int ir_max, double size_rlc)
{
    instance.cleanSolution();

    std::vector<std::vector<int>> best_options(instance.thieves.size());
    std::vector<int> better_item(instance.thieves.size());
    int current_city = 0;
    std::vector<int> aux_caught_items(instance.items.size(), 0);

    for(int j = 0; j < instance.thieves.size(); j++)
    {
        instance.thieves[j].route.push_back(current_city);
        instance.thieves[j].backpack_weight.push_back(0);
        best_options[j].assign(instance.items.size() * size_rlc, -1);
    }
    
    bool items_available = true;
    int iterations = 0;
    while(items_available && iterations < ir_max)
    {
        for(int i = 0; i < instance.thieves.size(); i++)
        {
            double smallest = -1;
            int smallest_index = 0;
            items_available = false;
            for(int j = 0; j < instance.items.size(); j++)
            {
                if(aux_caught_items[j] == 0 && ((instance.items[j].weight + instance.used_capacity) <= instance.max_capacity))
                {
                    items_available = true;
                    double A = 1, B = 0.25, C = 1;
                    double cost_benefit = instance.items[j].value / (A*instance.cities_distance[current_city][instance.items[j].city_idx] *  C*instance.renting_ratio + B*instance.items[j].weight );
                            
                    if(cost_benefit > smallest)
                    {
                        best_options[i][smallest_index] = j;
                        smallest = best_options[i][0];
                        for(int k = 0; k < best_options[i].size(); k++)
                        {
                            if(smallest > best_options[i][k])
                            {
                                smallest = best_options[i][k];
                                smallest_index = k;
                            }
                        }                    
                    }
                }
            }
            int get_this = rand() % best_options[i].size();

            while(get_this == -1 && items_available)
            {
                get_this = rand() % best_options[i].size();
            }

            current_city = instance.items[best_options[i][get_this]].city_idx;
            aux_caught_items[best_options[i][get_this]] = 1;
            instance.add_item(i, best_options[i][get_this], true);

            if(!items_available) break;
        }
        iterations++;
    }
    while(items_available)
    {
        for(int i = 0; i < instance.thieves.size(); i++)
        {
            float best_val = 0;
            int best_index = 0;
            float cur_val = 0;
            for(int j = 0; j < instance.items.size(); j++)
            {
                if(aux_caught_items[j] == 0)
                {
                    double A = 1, B = 0.25, C = 1;
                    cur_val = instance.items[j].value / (A*instance.cities_distance[current_city][instance.items[j].city_idx] *  C*instance.renting_ratio + B*instance.items[j].weight );
                    
                    if(cur_val > best_val)
                    {
                        best_val = cur_val;
                        best_index = j;
                    }
                    items_available = true;
                }
                else
                {
                    items_available = false;
                }
            }
            better_item[i] = best_index;
            aux_caught_items[best_index] = 1;

            if(instance.items[best_index].weight < instance.max_capacity - instance.used_capacity)
            {
                current_city = instance.items[best_index].city_idx;
                // Pega o item
                instance.caught_items[best_index] = 1;
                instance.used_capacity += instance.items[best_index].weight;

                // Adiciona o item a mochila do ladrao
                instance.thieves[i].items.push_back(best_index);
                
                // Adiciona a cidade a rota
                auto pos = std::find(instance.thieves[i].route.begin(), instance.thieves[i].route.end(), instance.items[best_index].city_idx);
                if(pos == instance.thieves[i].route.end())
                {
                    instance.thieves[i].route.push_back(current_city);
                    instance.thieves[i].backpack_weight.push_back(instance.items[best_index].weight);
                }
                else
                {
                    int index = std::distance(instance.thieves[i].route.begin(), pos);
                    instance.thieves[i].backpack_weight[index] += instance.items[best_index].weight;
                }
            }
        }    
    }
}

double grasp(Instance& instance, int imax, int ir_max, double size_rlc, bool verbose, int func)
{
    Instance best_solution = instance;
    if(func == 1)
        random_greedy(best_solution, ir_max, size_rlc);
    else
        random_greedy2(best_solution, ir_max, size_rlc);

    double best_value = best_solution.objectiveFunction();

    if(verbose) std::cout << "Solucao inicial do GRASP: " << best_value << std::endl;

    int i = 0;
    while(i < imax)
    {
        Instance aux_instance = instance;
        if(func == 1)
            random_greedy(aux_instance, ir_max, size_rlc);
        else
            random_greedy2(aux_instance, ir_max, size_rlc);
            
        int cont = 0;
        double best = aux_instance.objectiveFunction();
        double result = localSearch(aux_instance, "items");
        double current = aux_instance.objectiveFunction();

        if(verbose) std::cout << "Iteracao " << i << ": " << current << std::endl;

        if(best_value < current)
        {
            best_value = current;
            best_solution = aux_instance;
        }
        i++;
    }
    if(verbose) std::cout << "Melhor valor encontrado: " << best_value << std::endl;
    instance = best_solution;

    return best_value;
} 