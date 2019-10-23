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
        else cont++;
        if(cont >= 10) break;
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
    std::vector<float> population_eval(population.size());
    std::vector<float> probabilities(population.size());
    float total_eval = 0;

    for(int i = 0; i < population.size(); i++)
    {
        population_eval[i] = population[i].objectiveFunction();
        total_eval += population_eval[i];
    }
    for(int i = 0; i < population.size(); i++)
    {
        if(i != 0)
        {
            for(int j = 0; j < i; j++)
            {
                probabilities[i] += probabilities[j];
            }
        }
        probabilities[i] += population_eval[i]/total_eval;
    }

    for(int i = 0; i < pool_size; i++)
    {
        double p = ((double) rand() / (RAND_MAX));
        if(p < probabilities[0])
        {
           mating_pool.push_back(population[0]); 
        }
        for(int j = 1; j < probabilities.size() - 1; j++)
        {
            if(p >= probabilities[j -1] && p < probabilities[j])
            {
                mating_pool.push_back(population[j]);
                break;
            }
        }
        if(p >= probabilities[probabilities.size() - 1])
        {
            mating_pool.push_back(population[population.size() - 1]); 
        }
    }
}
void crossover(std::vector<Instance>& mating_pool, std::vector<Instance>& next_gen, double alpha, int pop_size, bool verbose)
{
    int num_pairs = alpha*pop_size;
    for(int i = 0; i < num_pairs; i+=2)
    {
        int rand_thief_1 = rand() % mating_pool[i].thieves.size();
        int rand_thief_2 = rand() % mating_pool[i].thieves.size();

        int parent_1 = rand() % mating_pool.size();
        int parent_2 = rand() % mating_pool.size();

        next_gen.push_back(mating_pool[parent_1]);
        next_gen.push_back(mating_pool[parent_2]);

        int last = next_gen.size() - 1;
        int seclast = next_gen.size() - 2;

        if(verbose) std::cout << rand_thief_1 << " " << rand_thief_2 << std::endl;

        next_gen[seclast].thieves[rand_thief_1] = mating_pool[last].thieves[rand_thief_2];
        next_gen[seclast].used_capacity = 0;
        for(int j = 0; j < next_gen[seclast].thieves[rand_thief_1].second.route.size(); j++)
        {
            double weight_city = 0;
            // Soma o peso de todos os itens pertencentes a cidade atual
            for(auto stolen_item: next_gen[seclast].thieves[rand_thief_1].second.items)
            {
                if(next_gen[seclast].items[stolen_item].city_idx == next_gen[seclast].thieves[rand_thief_1].second.route[j]){
                    weight_city += next_gen[seclast].items[stolen_item].weight;
                }
            }

            // Seta o peso de quando sai da cidade atual como o peso calculado
            next_gen[seclast].thieves[rand_thief_1].second.backpack_weight[j] = weight_city;
            next_gen[seclast].used_capacity += weight_city;
        }

        next_gen[last].thieves[rand_thief_2] = mating_pool[seclast].thieves[rand_thief_1];
        next_gen[last].used_capacity = 0;
        for(int j = 0; j < next_gen[last].thieves[rand_thief_2].second.route.size(); j++)
        {
            double weight_city = 0;
            // Soma o peso de todos os itens pertencentes a cidade atual
            for(auto stolen_item: next_gen[last].thieves[rand_thief_2].second.items)
            {
                if(next_gen[last].items[stolen_item].city_idx == next_gen[last].thieves[rand_thief_2].second.route[j]){
                    weight_city += next_gen[last].items[stolen_item].weight;
                }
            }

            // Seta o peso de quando sai da cidade atual como o peso calculado
            next_gen[last].thieves[rand_thief_2].second.backpack_weight[j] = weight_city;
            next_gen[last].used_capacity += weight_city;
        }
    }
    if(verbose)
    {
        for(int i = 0; i < next_gen.size(); i++)
        {
            std::cout << next_gen[i].objectiveFunction() << " ";
        }
        std::cout << std::endl;
    }
}

void mutation(std::vector<Instance>& generation, double beta, int pop_size, bool verbose)
{
    int num_members = beta*pop_size;
    for(int i = 0; i < num_members; i++)
    {
        int choosed_thief = rand() % generation[i].thieves.size();
        int choosed_city = rand() % generation[i].thieves[choosed_thief].second.route.size();
        int new_pos = rand() % generation[i].thieves[choosed_thief].second.route.size();
        int individual = rand() % generation.size();
        double p = ((double) rand() / (RAND_MAX)); 
        generation[individual].move_cities(choosed_thief, choosed_city, new_pos);

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
                    for(int l = 0; l < generation[i].thieves[k].second.items.size(); l++)
                    {
                        if(generation[i].thieves[k].second.items[l] == j)
                        {
                            occur.push_back(std::make_pair(k,l));
                            break;
                        }
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

    for(int i = 0; i < num_generations; i++)
    {
        selection(population, generation, alpha, pop_size, verbose);
        std::vector<Instance> next_generation;
        crossover(generation, next_generation, alpha, pop_size, verbose);   
        mutation(next_generation, beta, pop_size, verbose);
        validation(next_generation, verbose);
        for(auto individuo: generation)
        {
            double current = individuo.objectiveFunction();
            if(bestest <= current){ bestest = current; best_individual = individuo;}
        }
        generation = next_generation;   
    }
    
    best_individual.output();
    return bestest;   
}