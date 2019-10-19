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
}
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

void initial_pop(Instance& inst, std::vector<Instance>& population, bool verbose)
{
    for(int i = 0; i <= 20; i++)
    {
        Instance instance = inst;
        std::vector<int> aux_caught_items(instance.items.size(), 0);
        for(int i = 0; i < instance.thieves.size(); i++)
        {
            instance.thieves[i].second.route.push_back(0);
            instance.thieves[i].second.backpack_weight.push_back(0);
        }
        bool items_available = true;
        while(instance.used_capacity < instance.max_capacity and items_available)
        {
            int tries = 0;
            items_available = true;
            int rand_thief = rand() % instance.thieves.size();
            int rand_item = rand() % instance.items.size();
            while(tries < (instance.items.size()/3))
            {
                rand_item = rand() % instance.items.size();
                tries++;
                if(aux_caught_items[rand_item] == 1)
                {
                    items_available = false;
                }
                else
                {
                    aux_caught_items[rand_item] = 1;
                    items_available = true;
                    break;
                }
            }
            if(verbose) std::cout << "ladrao " << rand_thief << " item " << rand_item << std::endl;
            if(items_available)
            {
                if(instance.items[rand_item].weight < instance.max_capacity - instance.used_capacity)
                {
                    int current_city = instance.items[rand_item].city_idx;

                    // Pega o item
                    instance.caught_items[rand_item] = 1;
                    instance.used_capacity += instance.items[rand_item].weight;

                    if(verbose) std::cout << "Peguei o item" << std::endl;

                    // Adiciona o item a mochila do ladrao
                    instance.thieves[rand_thief].second.items.push_back(rand_item);
                    
                    // Adiciona a cidade a rota
                    auto pos = std::find(instance.thieves[rand_thief].second.route.begin(), instance.thieves[rand_thief].second.route.end(), instance.items[rand_item].city_idx);
                    if(pos == instance.thieves[rand_thief].second.route.end())
                    {
                        instance.thieves[rand_thief].second.route.push_back(current_city);
                        instance.thieves[rand_thief].second.backpack_weight.push_back(instance.items[rand_item].weight);
                    }
                    else
                    {
                        int index = std::distance(instance.thieves[rand_thief].second.route.begin(), pos);
                        instance.thieves[rand_thief].second.backpack_weight[index] += instance.items[rand_item].weight;
                    }
                }
            }        
        }
        population.push_back(instance);
    }       
}

void selection(std::vector<Instance>& population, std::vector<Instance>& mating_pool, bool verbose)
{
    int c = 0;
    int smallest = population[0].objectiveFunction();
    int smallest_index = 0;
    for(int i = 1; i < population.size(); i++)
    {
        int eval = population[i].objectiveFunction();
        if(c < 10)
        {
            mating_pool.push_back(population[i]);
            if(eval < smallest)
            {
                smallest = eval;
                smallest_index = c;
            }
            c++;
        }
        else
        {
            if(eval > smallest)
            {
                mating_pool[smallest_index] = population[i];
            }
            smallest_index = smallest = 0;
            for(int j = 0; j < mating_pool.size(); j++)
            {
                if(eval < smallest)
                {
                    smallest = eval;
                    smallest_index = j;
                }
            }
        }
    }
    if (verbose)
    {
        for(int i = 0; i < mating_pool.size(); i++)
        {
            std::cout << mating_pool[i].objectiveFunction() << " ";
        }
        std::cout << std::endl;
    }
}

void crossover(std::vector<Instance>& mating_pool, std::vector<Instance>& next_gen, bool verbose)
{
    next_gen = mating_pool;
    for(int i = 0; i < mating_pool.size() - 2; i+=2)
    {
        int rand_thief = rand() % mating_pool[i].thieves.size();

        if(verbose) std::cout << rand_thief << std::endl;

        next_gen[i].thieves[rand_thief] = mating_pool[i+1].thieves[rand_thief];
        next_gen[i].used_capacity = 0;
        for(int j = 0; j < next_gen[i].thieves[rand_thief].second.route.size(); j++)
        {
            double weight_city = 0;
            // Soma o peso de todos os itens pertencentes a cidade atual
            for(auto stolen_item: next_gen[i].thieves[rand_thief].second.items)
            {
                if(next_gen[i].items[stolen_item].city_idx == next_gen[i].thieves[rand_thief].second.route[j]){
                    weight_city += next_gen[i].items[stolen_item].weight;
                }
            }

            // Seta o peso de quando sai da cidade atual como o peso calculado
            next_gen[i].thieves[rand_thief].second.backpack_weight[j] = weight_city;
            next_gen[i].used_capacity += weight_city;
        }

        next_gen[i+1].thieves[rand_thief] = mating_pool[i].thieves[rand_thief];
        next_gen[i+1].used_capacity = 0;
        for(int j = 0; j < next_gen[i+1].thieves[rand_thief].second.route.size(); j++)
        {
            double weight_city = 0;
            // Soma o peso de todos os itens pertencentes a cidade atual
            for(auto stolen_item: next_gen[i+1].thieves[rand_thief].second.items)
            {
                if(next_gen[i].items[stolen_item].city_idx == next_gen[i+1].thieves[rand_thief].second.route[j]){
                    weight_city += next_gen[i].items[stolen_item].weight;
                }
            }

            // Seta o peso de quando sai da cidade atual como o peso calculado
            next_gen[i+1].thieves[rand_thief].second.backpack_weight[j] = weight_city;
            next_gen[i+1].used_capacity += weight_city;
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

void mutation(std::vector<Instance>& generation, bool verbose)
{
    for(int i = 0; i < generation.size(); i++)
    {
        int rand_thief = rand() % generation[i].thieves.size();
        int remove_item = rand() % generation[i].thieves[rand_thief].second.items.size();
        int insert_item = rand() % generation[i].items.size();
        int tries = 0;
        while(generation[i].caught_items[insert_item] == 1 && tries <= generation[i].items.size()/3)
        {
            insert_item = rand() % generation[i].items.size();
            tries++;
        }
        if(generation[i].caught_items[insert_item] == 0 && ((generation[i].items[insert_item].weight + generation[i].used_capacity) <= generation[i].used_capacity))
        {
            bool remove_city = true;
            for(int j = 0; j < generation[i].thieves[rand_thief].second.items.size(); j++)
            {
                int thief_item = generation[i].thieves[rand_thief].second.items[j];
                if(generation[i].items[remove_item].city_idx == generation[i].items[thief_item].city_idx && thief_item != remove_item)
                {
                    remove_city = false;
                    break;
                }
            }
                        
            generation[i].thieves[rand_thief].second.items.erase(generation[i].thieves[rand_thief].second.items.begin() + remove_item); 
            generation[i].caught_items[remove_item] = 0;
            auto pos = std::find(generation[i].thieves[rand_thief].second.route.begin(), generation[i].thieves[rand_thief].second.route.end(), generation[i].items[remove_item].city_idx);
            int index_city = std::distance(generation[i].thieves[rand_thief].second.route.begin(), pos);
            generation[i].thieves[rand_thief].second.backpack_weight[index_city] -= generation[i].items[remove_item].weight;
            generation[i].used_capacity -= generation[i].items[remove_item].weight;

            if(verbose) std::cout << "Removi item " << remove_item << std::endl;

            if(remove_city && pos != generation[i].thieves[rand_thief].second.route.end())
            {
                generation[i].thieves[rand_thief].second.route.erase(pos);
                if(verbose) std::cout << "Removi cidade " << generation[i].items[remove_item].city_idx << std::endl;
            }   

            generation[i].thieves[rand_thief].second.items.push_back(insert_item);
            generation[i].caught_items[insert_item] = 1;
            generation[i].used_capacity += generation[i].items[insert_item].weight;

            if(verbose) std::cout << "Inseri item " << insert_item << std::endl;

            if(pos == generation[i].thieves[rand_thief].second.route.end())
            {
                generation[i].thieves[rand_thief].second.route.push_back(generation[i].items[insert_item].city_idx);
                generation[i].thieves[rand_thief].second.backpack_weight.push_back(generation[i].items[insert_item].weight);
            }
            else
            {
                int index = std::distance(generation[i].thieves[rand_thief].second.route.begin(), pos);
                generation[i].thieves[rand_thief].second.backpack_weight[index] += generation[i].items[insert_item].weight;
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
double geneticAlgorithm(Instance& instance, int num_generations, bool verbose)
{
    std::vector<Instance> population;
    std::vector<Instance> mating_pool;
    std::vector<Instance> generation;
    
    if(verbose) std::cout << "Initial population" << std::endl;
    initial_pop(instance, population, verbose);
    if(verbose) std::cout << "Selection" << std::endl;
    selection(population, mating_pool, verbose);

    for(int i = 0; i < num_generations; i++)
    {
        if(verbose) std::cout << "Crossover" << std::endl;
        crossover(mating_pool, generation, verbose);   
        if(verbose) std::cout << "Mutation" << std::endl;
        mutation(generation, verbose);  
    }
    
    if(verbose) std::cout << "Validation" << std::endl;
    validation(generation, verbose);

    int best = generation[0].objectiveFunction();
    for(auto inst: generation)
    {
        int current = inst.objectiveFunction();
        if(best > current)
        {
            best = current;
        }
    }
    return best;   
}