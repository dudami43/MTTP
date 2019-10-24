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
            
            // Muda para o proximo vizinho
            inst.swap_items_btw_thieves(thief_1, thief_2, i, j);

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
    if(method.compare("swap") == 0)
    {
        return first_improvement_swap(inst);
    }
    else if(method.compare("move") == 0)
    {
        return first_improvement_move(inst);
    }
    else if(method.compare("trade_ungotted") == 0)
    {
        return first_improvement_trade_ungotted(inst);
    }
    else if(method.compare("trade_btw_thieves") == 0)
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
        else 
        {
            cont++;
        }
        if(cont >= 10)
        {
            break;
        } 
    }
    return best;
}

/**
 * Heuristicas
 **/

double VNS(Instance& inst, int max_disturbance, bool verbose)
{

    // Inicializa o vector de vizinhanças
    std::vector<std::string> neighborhoods;
    neighborhoods.push_back("swap");
    neighborhoods.push_back("move");
    //neighborhoods.push_back("trade_btw_thieves");
    //neighborhoods.push_back("trade_ungotted");

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
        } */
        current_value = localSearch(inst, neighborhoods[neighborhood]);

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
            if(neighborhood >= neighborhoods.size()) break;
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
            instance.thieves[j].second.route.push_back(current_city);
            instance.thieves[j].second.backpack_weight.push_back(0);
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
                        instance.thieves[j].second.items.push_back(better_items[j][item_to_take]);
                        

                        // Adiciona a cidade a rota
                        auto pos = std::find(instance.thieves[j].second.route.begin(), instance.thieves[j].second.route.end(), instance.items[better_items[j][item_to_take]].city_idx);
                        if(pos == instance.thieves[j].second.route.end())
                        {
                            instance.thieves[j].second.route.push_back(current_city);
                            instance.thieves[j].second.backpack_weight.push_back(instance.items[better_items[j][item_to_take]].weight);
                        }
                        else
                        {
                            int index = std::distance(instance.thieves[j].second.route.begin(), pos);
                            instance.thieves[j].second.backpack_weight[index] += instance.items[better_items[j][item_to_take]].weight;
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

        Thief aux = next_gen[last_from_next].thieves[thief1].second;
        next_gen[last_from_next].thieves[thief1].second = next_gen[last_from_next - 1].thieves[thief2].second;
        next_gen[last_from_next - 1].thieves[thief2].second = aux;
       
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
            for(int k = 0; k < next_gen[i].thieves[j].second.items.size(); k++)
            {
                if(next_gen[i].caught_items[next_gen[i].thieves[j].second.items[k]] == 0)
                {
                    next_gen[i].used_capacity += next_gen[i].items[next_gen[i].thieves[j].second.items[k]].weight;
                    next_gen[i].caught_items[next_gen[i].thieves[j].second.items[k]] = 1;
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
        int city_1 = rand() % generation[i].thieves[choosed_thief].second.route.size();
        int city_2 = rand() % generation[i].thieves[choosed_thief].second.route.size();
        int individual = rand() % generation.size();
        double p = ((double) rand() / (RAND_MAX)); 
        generation[individual].swap_cities(choosed_thief, city_1, city_2);
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
                    auto pos = std::find(generation[i].thieves[k].second.items.begin(), generation[i].thieves[k].second.items.end(), j);
                    if(pos != generation[i].thieves[k].second.items.end())
                    {
                        int index = std::distance(generation[i].thieves[k].second.items.begin(), pos);
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
                    generation[i].thieves[thief].second.items.erase(generation[i].thieves[thief].second.items.begin() + pos_item); 
                    auto pos = std::find(generation[i].thieves[thief].second.route.begin(), generation[i].thieves[thief].second.route.end(), generation[i].items[pos_item].city_idx);
                    if(pos != generation[i].thieves[thief].second.route.end())
                    {
                        int index_city = std::distance(generation[i].thieves[thief].second.route.begin(), pos);
                        generation[i].thieves[thief].second.backpack_weight[index_city] -= generation[i].items[pos_item].weight;
                    
                    }
                    generation[i].used_capacity -= generation[i].items[pos_item].weight;

                    if(verbose) std::cout << "Removi item " << pos_item << std::endl;

                    bool remove_city = true;
                    for(int l = 0; l < generation[i].thieves[thief].second.items.size(); l++)
                    {
                        int thief_item = generation[i].thieves[thief].second.items[l];
                        if(generation[i].items[pos_item].city_idx == generation[i].items[thief_item].city_idx && thief_item != pos_item)
                        {
                            remove_city = false;
                            break;
                        }
                    }
                    if(remove_city && pos != generation[i].thieves[thief].second.route.end())
                    {
                        generation[i].thieves[thief].second.route.erase(pos);
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

void random_greedy(Instance& instance, double size_rlc)
{
    instance.cleanSolution();
    std::vector<std::vector<int>> better_items(instance.thieves.size()); //Cada vetor representa os n melhores itens pra um ladrão i
    std::vector<int> aux_caught_items(instance.items.size(), 0);
    bool items_remaining = true;
    int current_city = 0;
    for(int j = 0; j < instance.thieves.size(); j++)
    {
        instance.thieves[j].second.route.push_back(current_city);
        instance.thieves[j].second.backpack_weight.push_back(0);
    }
    while(items_remaining)
    {
        for(int j = 0; j < instance.thieves.size(); j++)
        {
            better_items[j].assign(instance.items.size()*size_rlc, -1);
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
                    instance.thieves[j].second.items.push_back(better_items[j][item_to_take]);
                    

                    // Adiciona a cidade a rota
                    auto pos = std::find(instance.thieves[j].second.route.begin(), instance.thieves[j].second.route.end(), instance.items[better_items[j][item_to_take]].city_idx);
                    if(pos == instance.thieves[j].second.route.end())
                    {
                        instance.thieves[j].second.route.push_back(current_city);
                        instance.thieves[j].second.backpack_weight.push_back(instance.items[better_items[j][item_to_take]].weight);
                    }
                    else
                    {
                        int index = std::distance(instance.thieves[j].second.route.begin(), pos);
                        instance.thieves[j].second.backpack_weight[index] += instance.items[better_items[j][item_to_take]].weight;
                    }
                }   
            }   
        }
    }
}

double grasp(Instance& instance, int imax, bool verbose)
{
    int i = 0;
    
    Instance best_individual = instance;
    best_individual.greedySolution();
    int current;
    double best_value = best_individual.objectiveFunction();
    if(verbose) std::cout << "Greedy " << best_value << std::endl;

    while(i < imax)
    {
        random_greedy(instance, 0.05);
        call_localSearch(instance, "swap");
        current = instance.objectiveFunction();
        //if(verbose) std::cout << current << std::endl;
        if(best_value < current)
        {
            best_value = current;
            best_individual = instance;
        }
        i++;
    }
    if(verbose) std::cout << "Grasp " << best_value << std::endl;
    return best_value;
}