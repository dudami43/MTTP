#include <bits/stdc++.h>
#include "classes.cpp"
#include "functions.h"

/**
 * Funcoes auxiliares
**/
// Pega o proximo item de um ladrao e retorna a posicao que sera incrementada na proxima iteracao
int get_next(Instance& inst, std::vector<int>& choosed_thieves, std::vector<int>& actual_items, int next_position)
{
    actual_items[next_position]++;

    for(int i = 0; i < next_position; i++)
    {
        actual_items[i] = 0;
        next_position = 0;
    }

    while( actual_items[next_position] == (inst.thieves[choosed_thieves[next_position]].items.size() - 1) )
    {
        next_position++;
    }

    if(next_position == choosed_thieves.size()) return -1;

    return next_position;
}

/**
 * Busca Local
**/
double first_improvement_shuffle(Instance& inst)
{
    double current_value, best_value = inst.objectiveFunction();
    Instance initial_instance = inst;

    int n_thieves = 1;
    if(inst.thieves.size() <= 1) return best_value;
    else if(inst.thieves.size() == 2) n_thieves = 2;
    else 
    {
       n_thieves = 2 + (rand() % inst.thieves.size() - 1);
    }
    
    std::vector<int> choosed_thieves;
    choosed_thieves.assign(n_thieves, 0);
    bool equals;
    for(int i = 0; i < n_thieves; i++)
    {
        equals = true;
        while(equals)
        {
            choosed_thieves[i] = rand() % inst.thieves.size();
            equals = false;
            for(int j = 0; j < i; j++)
            {
                if(choosed_thieves[i] == choosed_thieves[j])
                    equals = true;        
            }
        }
    }

    std::vector<int> items_position;
    items_position.assign(choosed_thieves.size(), 0);
    int next_increment = 0;
    while(next_increment != -1){
        
        inst.shuffles_thieves_items(n_thieves, choosed_thieves, items_position, true);

        std::cout << "1" << std::endl;

        // Valida solucao
        if(inst.solutionValid())
        {
            // Avalia a nova solucao
            current_value = inst.objectiveFunction();

            std::cout << "2" << std::endl;

            
            // Verifica se eh melhor que a atual
            if(current_value > best_value)
            {
                std::cout << "3" << std::endl;        
                return current_value;
            }
            std::cout << "4" << std::endl;

        }

        std::cout << "5" << std::endl;

        // Caso nao melhore ou a nova solucao n seja valida, retorne a solucao inicial
        inst = initial_instance;
        std::cout << "6" << std::endl;
        
        next_increment = get_next(inst, choosed_thieves, items_position, next_increment);
        
        std::cout << "7" << std::endl;

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
    }
}

/**
 * Heuristicas
 **/

// VNS
double VNS(Instance& inst, int max_disturbance, int n_disturbe, bool verbose)
{
    // Inicializa o vector de vizinhanças
    std::vector<std::string> neighborhoods;
    neighborhoods.push_back("items");
    neighborhoods.push_back("move");
    neighborhoods.push_back("shuffle");
    neighborhoods.push_back("swap");

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
        std::vector<std::vector<int>> better_items(instance.thieves.size()); //Cada vetor representa os n melhores itens pra um ladrão i
        std::vector<int> aux_caught_items(instance.items.size(), 0);
        bool items_remaining = true;
        int current_city = 0;
        for(int j = 0; j < instance.thieves.size(); j++)
        {
            instance.thieves[j].route.push_back(current_city);
            instance.thieves[j].backpack_weight.push_back(0);
        }
        while(items_remaining)
        {
            for(int j = 0; j < instance.thieves.size(); j++)
            {
                better_items[j].assign(instance.items.size()*0.2, -1);
                for(int k = 0; k < instance.items.size(); k++)
                {
                    float cur_val = instance.items[k].value;
                    float aux_val = 0;
                    float out_val = instance.items[better_items[j][0]].value;
                    int out_ind = 0;
                    if(aux_caught_items[k] == 0)
                    {
                        if(instance.cities_distance[current_city][instance.items[k].city_idx] != 0)                      
                            cur_val = instance.items[k].value / instance.cities_distance[current_city][instance.items[k].city_idx];
                        
                        for(int l = 0; l < better_items[j].size(); l++)
                        {
                            if(instance.cities_distance[current_city][instance.items[better_items[j][l]].city_idx] != 0)     
                                aux_val = instance.items[better_items[j][l]].value / instance.cities_distance[current_city][instance.items[better_items[j][l]].city_idx];
                            else
                                aux_val = instance.items[better_items[j][l]].value;

                            if(aux_val < out_val)
                            {
                                out_val = aux_val;
                                out_ind = l;
                            }
                        } // escolher qual valor pode ser substituido da lista de melhores
                        if(cur_val > out_val)
                        {
                            better_items[j][out_ind] = k;
                        }
                        items_remaining = true;
                    }
                    else
                    {
                        items_remaining = false;
                    } 
                }
                int item_to_take = rand() % better_items[j].size();
                
                while(better_items[j][item_to_take] == -1 && items_remaining)
                {
                    item_to_take = rand() % better_items[j].size();
                }
               
                if(better_items[j][item_to_take] != -1)
                {
                    
                    aux_caught_items[better_items[j][item_to_take]] = 1;
                    

                    if(instance.items[better_items[j][item_to_take]].weight < instance.max_capacity - instance.used_capacity)
                    {
                        
                        current_city = instance.items[better_items[j][item_to_take]].city_idx;
                        
                        // Pega o item
                        instance.caught_items[better_items[j][item_to_take]] = 1;
                        instance.used_capacity += instance.items[better_items[j][item_to_take]].weight;

                        // Adiciona o item a mochila do ladrao
                        instance.thieves[j].items.push_back(better_items[j][item_to_take]);
                        

                        // Adiciona a cidade a rota
                        auto pos = std::find(instance.thieves[j].route.begin(), instance.thieves[j].route.end(), instance.items[better_items[j][item_to_take]].city_idx);
                        if(pos == instance.thieves[j].route.end())
                        {
                            instance.thieves[j].route.push_back(current_city);
                            instance.thieves[j].backpack_weight.push_back(instance.items[better_items[j][item_to_take]].weight);
                        }
                        else
                        {
                            int index = std::distance(instance.thieves[j].route.begin(), pos);
                            instance.thieves[j].backpack_weight[index] += instance.items[better_items[j][item_to_take]].weight;
                        }
                    }   
                }   
            }
        }
        population.push_back(instance);
    }  
    if (verbose)
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
        int choosed_city = rand() % generation[i].thieves[choosed_thief].route.size();
        int new_pos = rand() % generation[i].thieves[choosed_thief].route.size();
        int individual = rand() % generation.size();
        std::cout << "Alterar instancia " << individual << " movendo a cidade " << choosed_city << " do ladrao " << choosed_thief << " para a posicao " << new_pos << std::endl; 
        double p = ((double) rand() / (RAND_MAX)); 
        std::cout << new_pos << " " << generation[i].thieves[choosed_thief].route.size() << std::endl;
        generation[individual].move_cities(choosed_thief, choosed_city, new_pos, true);
        std::cout << "Movi cidades\n";
        if(p >= 0.5)
        {
            std::cout << "Movi itens\n";
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
    generation = population;
    for(int i = 0; i < num_generations; i++)
    {
        std::vector<Instance> mating_pool;
        selection(generation, mating_pool, alpha, pop_size, verbose);
        for(auto individuo: mating_pool)
        {
            double current = individuo.objectiveFunction();
            if(bestest <= current){ bestest = current; best_individual = individuo;}
        }
        std::cout << "\nSelection " << bestest << std::endl;
        std::vector<Instance> next_generation;
        crossover(mating_pool, next_generation, alpha, pop_size, verbose);   
        for(auto individuo: next_generation)
        {
            double current = individuo.objectiveFunction();
            if(bestest <= current){ bestest = current; best_individual = individuo;}
        }
        std::cout << "\nCrossover " << bestest << std::endl;
        mutation(next_generation, beta, pop_size, verbose);
        for(auto individuo: next_generation)
        {
            double current = individuo.objectiveFunction();
            if(bestest <= current){ bestest = current; best_individual = individuo;}
        }
        std::cout << "\nMutation " << bestest << std::endl;
        validation(next_generation, verbose);
        generation = next_generation;   
        for(auto individuo: generation)
        {
            double current = individuo.objectiveFunction();
            if(bestest <= current){ bestest = current; best_individual = individuo;}
        }
        std::cout << "\nValidation " << bestest << std::endl;
    }
    instance = best_individual;
    std::cout << "Genetico " << bestest << std::endl;
    return bestest;   
}

// GRASP 

void random_greedy(Instance& instance, double size_rlc)
{
    instance.cleanSolution();

    std::vector<std::vector<int>> better_items(instance.thieves.size()); //Cada vetor representa os n melhores itens pra um ladrão i
    std::vector<int> aux_caught_items(instance.items.size(), 0);
    bool items_remaining = true;
    int current_city = 0;

    for(int j = 0; j < instance.thieves.size(); j++)
    {
        instance.thieves[j].route.push_back(current_city);
        instance.thieves[j].backpack_weight.push_back(0);
    }

    while(items_remaining)
    {
        for(int j = 0; j < instance.thieves.size(); j++)
        {
            better_items[j].assign(instance.items.size()*size_rlc + 0.5, -1);
            for(int k = 0; k < instance.items.size(); k++)
            {
                float cur_val = instance.items[k].value;
                float aux_val = 0;
                float out_val = instance.items[better_items[j][0]].value;
                int out_ind = 0;
                if(aux_caught_items[k] == 0)
                {
                    if(instance.cities_distance[current_city][instance.items[k].city_idx] != 0)                      
                        cur_val = instance.items[k].value / instance.cities_distance[current_city][instance.items[k].city_idx];

                    for(int l = 0; l < better_items[j].size(); l++)
                    {
                        if(instance.cities_distance[current_city][instance.items[better_items[j][l]].city_idx] != 0)     
                            aux_val = instance.items[better_items[j][l]].value / instance.cities_distance[current_city][instance.items[better_items[j][l]].city_idx];
                        else
                            aux_val = instance.items[better_items[j][l]].value;

                        if(aux_val < out_val)
                        {
                            out_val = aux_val;
                            out_ind = l;
                        }
                    } // escolher qual valor pode ser substituido da lista de melhores
                    if(cur_val > out_val)
                    {
                        better_items[j][out_ind] = k;
                    }
                    items_remaining = true;
                }
                else
                {
                    items_remaining = false;
                } 
            }
            int item_to_take = rand() % better_items[j].size();

            while(better_items[j][item_to_take] == -1 && items_remaining)
            {
                item_to_take = rand() % better_items[j].size();
            }

            if(better_items[j][item_to_take] != -1)
            {

                aux_caught_items[better_items[j][item_to_take]] = 1;


                if(instance.items[better_items[j][item_to_take]].weight < instance.max_capacity - instance.used_capacity)
                {

                    current_city = instance.items[better_items[j][item_to_take]].city_idx;

                    // Pega o item
                    instance.caught_items[better_items[j][item_to_take]] = 1;
                    instance.used_capacity += instance.items[better_items[j][item_to_take]].weight;

                    // Adiciona o item a mochila do ladrao
                    instance.thieves[j].items.push_back(better_items[j][item_to_take]);


                    // Adiciona a cidade a rota
                    auto pos = std::find(instance.thieves[j].route.begin(), instance.thieves[j].route.end(), instance.items[better_items[j][item_to_take]].city_idx);
                    if(pos == instance.thieves[j].route.end())
                    {
                        instance.thieves[j].route.push_back(current_city);
                        instance.thieves[j].backpack_weight.push_back(instance.items[better_items[j][item_to_take]].weight);
                    }
                    else
                    {
                        int index = std::distance(instance.thieves[j].route.begin(), pos);
                        instance.thieves[j].backpack_weight[index] += instance.items[better_items[j][item_to_take]].weight;
                    }
                }   
            }   
        }
    }
}

double grasp(Instance& instance, int imax, bool verbose)
{
    Instance best_solution = instance;
    best_solution.greedySolution();
    double best_value = best_solution.objectiveFunction();

    if(verbose) std::cout << "Solucao inicial do GRASP: " << best_value << std::endl;

    int i = 0;
    while(i < imax)
    {
        Instance aux_instance = instance;
        random_greedy(aux_instance, 0.05);
        localSearch(aux_instance, "items");
        double current = aux_instance.objectiveFunction();

        if(verbose) std::cout << "Iteracao " << i << ": " << best_value << std::endl;

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