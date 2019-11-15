#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

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

    std::vector<int> items_position;
    items_position.assign(choosed_thieves.size(), 0);
    while(true){

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

    // Procura duas cidades que melhoram a solucao atual
    for(int i = 1; i < inst.thieves[thief].route.size(); i++)
    {
        for(int j = i + 1; j < inst.thieves[thief].route.size(); j++)
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

double first_improvement_move(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int thief = rand() % inst.thieves.size();

    // Procura uma posicao para uma cidade, de maneira a melhorar a solucao
    for(int i = 1; i < inst.thieves[thief].route.size(); i++)
    {
        for(int j = 1; j < inst.thieves[thief].route.size(); j++)
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

double first_improvement_trade_ungotten(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance;
    int tries = 0;
    int thief = rand() % inst.thieves.size();

    // Procura dois itens que melhoram a solucao atual
    for(int i = 1; i < inst.thieves[thief].items.size(); i++)
    {
        for(int j = i + 1; j < inst.items.size(); j++)
        {
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

    // Procura duas cidades que melhoram a solucao atual
    for(int j = 0; j < inst.thieves[thief].items.size(); j++)
    {
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

    // Procura duas cidades que melhoram a solucao atual
    for(int j = 0; j < inst.items.size(); j++)
    {
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
    // Adquire o tempo em que comeca o algoritmo
    using namespace std::chrono;
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

    // Enquanto nao passou 10min de execucao
    while( duration_cast<duration<double>>(high_resolution_clock::now() - t_i).count() <= 600 )
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

// GA
void initial_pop(Instance& inst, std::vector<Instance>& population, int pop_size, bool verbose)
{
    for(int i = 0; i < pop_size; i++)
    {
        inst.cleanSolution();
        Instance instance = inst;
        random_greedy(instance, 2, 0.3);
        population.push_back(instance);
    }  
    if (true)
    {
        for(int i = 0; i < population.size(); i++)
        {
            std::cout << population[i].objectiveFunction() << " ";
        }
        std::cout << std::endl;
    }        
}

void selection(std::vector<Instance>& population, std::vector<Instance>& mating_pool, double alpha, int pop_size, bool verbose)
{
    int pool_size = (1 - alpha) * pop_size;
    
    for(int i = 0; i < pool_size; i++)
    {
        int c1 = rand() % population.size();
        int c2 = rand() % population.size();
        if(population[c1].objectiveFunction() > population[c2].objectiveFunction())
        {
            mating_pool.push_back(population[c1]);
        }
        else
        {
            mating_pool.push_back(population[c2]);
        }
    }
}

void crossover(std::vector<Instance>& mating_pool, std::vector<Instance>& next_gen, double alpha, int pop_size, bool verbose)
{
    int num_pairs = alpha*pop_size;
    for(int i = 0; i < num_pairs; i+=2)
    {
        int p1 = rand() % mating_pool.size();
        int p2 = rand() % mating_pool.size();

        int thief1 = rand() % mating_pool[p1].thieves.size();
        int thief2 = rand() % mating_pool[p2].thieves.size();

        next_gen.push_back(mating_pool[p1]);
        next_gen.push_back(mating_pool[p2]);

        int last_from_next = next_gen.size() - 1;

        Thief aux = next_gen[last_from_next].thieves[thief1];
        next_gen[last_from_next].thieves[thief1] = next_gen[last_from_next - 1].thieves[thief2];
        next_gen[last_from_next - 1].thieves[thief2] = aux;
       
    }

    for(int i = 0; i < next_gen.size(); i++)
    {
        next_gen[i].used_capacity = 0;
        for(int j = 0; j < next_gen[i].items.size(); j++)
        {
            next_gen[i].caught_items[j] = 0;
        }

        for(int j = 0; j < next_gen[i].thieves.size(); j++)
        {
            for(int k = 0; k < next_gen[i].thieves[j].items.size(); k++)
            {
                if(next_gen[i].caught_items[next_gen[i].thieves[j].items[k]] == 0)
                {
                    next_gen[i].used_capacity += next_gen[i].items[next_gen[i].thieves[j].items[k]].weight;
                    next_gen[i].caught_items[next_gen[i].thieves[j].items[k]] = 1;
                }
            }
        }
    }
        
}

void mutation(std::vector<Instance>& generation, double beta, int pop_size, bool verbose)
{
    int num_members = beta*pop_size;
    for(int i = 0; i < num_members; i++)
    {
        int choosed_thief = rand() % generation[i].thieves.size();
        int city_1 = rand() % generation[i].thieves[choosed_thief].route.size();
        int city_2 = rand() % generation[i].thieves[choosed_thief].route.size();
        if(city_1 == 0) city_1 = 1;
        if(city_2 == 0) city_2 = 1;
        int individual = rand() % generation.size();
        double p = ((double) rand() / (RAND_MAX)); 
        generation[individual].swap_cities(choosed_thief, city_1, city_2, true);
        if(p >= 0.5)
        {
            generation[individual].exchange_random_items();
        }
    }
    if(verbose)
    {
        for(int i = 0; i < generation.size(); i++)
        {
            std::cout << generation[i].objectiveFunction() << " ";
        }
        std::cout << std::endl;
    }
}

void validation(std::vector<Instance>& generation, bool verbose)
{
    for(int i = 0; i < generation.size(); i++)
    {
        for(int j = 0; j < generation[i].items.size(); j++)
        {
            std::vector<std::pair<int,int>> occur; //<ladrao, pos_item>
            if(generation[i].caught_items[j] == 1)
            {
                for(int k = 0; k < generation[i].thieves.size(); k++)
                {
                    auto pos = std::find(generation[i].thieves[k].items.begin(), generation[i].thieves[k].items.end(), j);
                    if(pos != generation[i].thieves[k].items.end())
                    {
                        int index = std::distance(generation[i].thieves[k].items.begin(), pos);
                        occur.push_back(std::make_pair(k,index));
                        break;
                    }
                }
            }
            if(occur.size() > 1)
            {
                for(int k = 0; k < occur.size() - 1; k++)
                {
                    int thief = occur[k].first;
                    int pos_item = occur[k].second;
                    generation[i].thieves[thief].items.erase(generation[i].thieves[thief].items.begin() + pos_item); 
                    auto pos = std::find(generation[i].thieves[thief].route.begin(), generation[i].thieves[thief].route.end(), generation[i].items[pos_item].city_idx);
                    if(pos != generation[i].thieves[thief].route.end())
                    {
                        int index_city = std::distance(generation[i].thieves[thief].route.begin(), pos);
                        generation[i].thieves[thief].backpack_weight[index_city] -= generation[i].items[pos_item].weight;
                    
                    }
                    generation[i].used_capacity -= generation[i].items[pos_item].weight;

                    if(verbose) std::cout << "Removi item " << pos_item << std::endl;

                    bool remove_city = true;
                    for(int l = 0; l < generation[i].thieves[thief].items.size(); l++)
                    {
                        int thief_item = generation[i].thieves[thief].items[l];
                        if(generation[i].items[pos_item].city_idx == generation[i].items[thief_item].city_idx && thief_item != pos_item)
                        {
                            remove_city = false;
                            break;
                        }
                    }
                    if(remove_city && pos != generation[i].thieves[thief].route.end())
                    {
                        generation[i].thieves[thief].route.erase(pos);
                        if(verbose) std::cout << "Removi cidade " << generation[i].items[pos_item].city_idx << std::endl;
                    }   

                    //tirar item da solução de todos - 1 ladrão
                    //se a cidade dele n tiver mais nenhum item, tirar a cidade
                    //tirar o peso da mochila
                }    
            }
        }
    }  
    if(verbose)
    {
        for(int i = 0; i < generation.size(); i++)
        {
            std::cout << generation[i].objectiveFunction() << " ";
        }
        std::cout << std::endl;  
    }  

}

double geneticAlgorithm(Instance& instance, int num_generations, int pop_size, double alpha, double beta, bool verbose)
{
    std::vector<Instance> population;
    std::vector<Instance> generation;
    
    initial_pop(instance, population, pop_size, verbose);
    double bestest = population[0].objectiveFunction();
    Instance best_individual;
    for(auto individuo: population)
    {
        double current = individuo.objectiveFunction();
        if(bestest < current){ bestest = current; best_individual = individuo;}
    }
    std::cout << "População inicial " << bestest << std::endl;
    instance = best_individual;
    return bestest;   
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