#include<bits/stdc++.h>

class Item {
    public:
        float weight, value;
        int city_idx;

    // Construtor padrao
    Item(){
        weight = 0.0;
        value = 0.0;
    }

    // Construtor
    Item(float weight, float value, int city_idx){
        this->weight = weight;
        this->value = value;
        this->city_idx = city_idx;
    }
};

class City {
    public:
        float x, y;
        std::vector<int> items_idx;

        // Construtor padrao
        City(){
            x = 0.0;
            y = 0.0;
        }

        // Construtor
        City(float x, float y){
            this->x = x;
            this->y = y;
        }

        int takeItem(int position)
        {
            int aux = this->items_idx[position];
            if(this->items_idx.size() > 1)
            {
                this->items_idx.erase(this->items_idx.begin()+position);
            }
            else
            {
                this->items_idx.clear();
            }
            return aux;
        }

        void returnItem(int index)
        {
            this->items_idx.push_back(index);
        }
};

class Thief {
    public:
        std::vector<int> route;
        std::vector<int> items;
        std::vector<int> backpack_weight;   
};

class Instance {
    public:
        // Atributos
        int num_cities, num_items, max_capacity, used_capacity;
        double min_speed, max_speed, renting_ratio;
        std::vector<std::vector<double> > cities_distance;
        std::vector<City> cities;
        std::vector<std::pair<double,Thief>> thieves; //<velocidade, ladrão>
        std::vector<Item> items;
        std::vector<int> taked_items;

        // Construtor padrao
        Instance()
        {
            num_cities = 0;
            num_items = 0;
            max_capacity = 0;
            used_capacity = 0;
            min_speed = 0.0;
            max_speed = 0.0;
            renting_ratio = 0.0;
        }

        // Construtor
        Instance(int num_cities, int num_items, int max_capacity, double min_speed, double max_speed, double renting_ratio)
        { 
            // Seta os dados base da instancia
            this->num_cities = num_cities;
            this->num_items = num_items;
            this->max_capacity = max_capacity;
            this->used_capacity = 0;
            this->min_speed = min_speed;
            this->max_speed = max_speed;
            this->renting_ratio = renting_ratio;

            // Gera uma matriz com num_cities linhas e colunas preenchidas com as distancias zeradas
            std::vector<double> empty_distance;
            empty_distance.assign(num_cities, 0.0);
            this->cities_distance.assign(num_cities, empty_distance);

            // Inicializa todos os items como "nao pego"
            this->taked_items.assign(num_items, 0);
        }

        void setValues(int num_cities, int num_items, int max_capacity, double min_speed, double max_speed, double renting_ratio)
        {
            
            // Seta os dados base da instancia
            this->num_cities = num_cities;
            this->num_items = num_items;
            this->max_capacity = max_capacity;
            this->min_speed = min_speed;
            this->max_speed = max_speed;
            this->renting_ratio = renting_ratio;

            // Gera uma matriz com num_cities linhas e colunas preenchidas com as distancias zeradas
            std::vector<double> empty_distance;
            empty_distance.assign(num_cities, 0.0);
            this->cities_distance.assign(num_cities, empty_distance);

            // Inicializa todos os items como "nao pego"
            this->taked_items.assign(num_items, 0);
        }

        void addThief(int n = 1)
        {
            Thief thief;
            for(int i = 0; i < n; i++)
                thieves.push_back(std::make_pair(this->max_speed, thief));
        }

        void initialRoutes()
        {
            float f_v = (this->max_speed - this->min_speed)/this->max_capacity;
            bool item_in_city = true;

            for(int j = 0; j < thieves.size(); j++) 
            {
                thieves[j].second.route.push_back(0);
                thieves[j].second.backpack_weight.push_back(0);
            }
            for(int i = 0; i < cities.size(); i++)
            {
                item_in_city = true;
                while(cities[i].items_idx.size() > 0 && item_in_city)
                {
                    for(int j = 0; j < thieves.size(); j++)
                    {
                        //pegar alguma coisa 
                        int x = rand() % cities[i].items_idx.size();
                        int this_one = this->cities[i].takeItem(x);
                        Item item_taken = this->items[this_one];

                        if(item_taken.weight < this->max_capacity - this->used_capacity)
                        {
                            if(std::find(thieves[j].second.items.begin(), thieves[j].second.items.end(), this_one) == thieves[j].second.items.end())
                            {
                                thieves[j].second.items.push_back(this_one);
                                this->taked_items.at(this_one) = 1;
                            }
                            this->used_capacity += item_taken.weight;

                            //adicionar cidade na rota
                            std::vector<int>::iterator it = std::find(thieves[j].second.route.begin(), thieves[j].second.route.end(), i);
                            if(it == thieves[j].second.route.end())
                             {
                                thieves[j].second.route.push_back(i);
                                thieves[j].second.backpack_weight.push_back(item_taken.weight);
                            }
                            else
                            {
                                int index = std::distance(thieves[j].second.route.begin(), it);
                                thieves[j].second.backpack_weight[index] += item_taken.weight;
                            }
                            
                            //diminuir velocidade
                            if(thieves[j].first - f_v > this->min_speed)
                            {
                                thieves[j].first = thieves[j].first;
                            }
                            else
                            {
                                thieves[j].first = this->min_speed;
                            }
                            
                            if(cities[i].items_idx.size() == 0)
                            {
                                item_in_city = false;
                                break;
                            }  
                        }
                        else
                        {
                            item_in_city = false;
                            break;
                        }
                    }
                }
                if(this->used_capacity == this->max_capacity) break;                
            }
            for(int j = 0; j < thieves.size(); j++)
            {
                thieves[j].second.route.push_back(0);
                thieves[j].second.backpack_weight.push_back(0);
            } 
            
        }

        void greedySolution()
        {
            /**
             * Passo 1:
             * Ordena os itens por custo beneficio(valor/peso)
             * 
             * Passo 2:
             * A cada iteracao um ladrao escolhe um item para pegar(em ordem decrescente de custo beneficio)
             * ate que nenhum consiga mais pegar nenhum item
             * 
             * Passo 3:
             * Para cada ladrao, ordena os itens do mesmo de maneira que ele pegue os itens das cidades mais
             * distantes primeiro e passe apenas uma vez em cada cidade
            **/

            // Cria um vector com os indices ordenados decrescentemente
            std::vector<int> sorted_items_idx;
            for(int i = 0; i < this->items.size(); i++)
            {
                sorted_items_idx.push_back(i);
            }

            // Adiciona a cidade inicial como ponto de partida de todos os ladroes
            for(int i = 0; i < this->thieves.size(); i++)
            {
                thieves[i].second.route.push_back(0);
                thieves[i].second.backpack_weight.push_back(0);
            }

            // Ordena o indice dos itens por custo beneficio (valor/peso)
            std::sort(sorted_items_idx.begin(), sorted_items_idx.end(), [=](int i, int j)
            {
                return this->items[sorted_items_idx[i]].value / this->items[sorted_items_idx[i]].weight < this->items[sorted_items_idx[j]].value / this->items[sorted_items_idx[j]].weight;
            });

            // Enquanto algum ladrao escolheu um item
            bool taked = true;
            while(taked)
            {
                taked = false;

                for(int j = 0; j < this->thieves.size(); j++)
                {
                    for(int i = sorted_items_idx.size() - 1; i >= 0; i--)
                    {
                        // Se o item ainda n foi pego, e cabe na mochila
                        if(!this->taked_items[sorted_items_idx[i]] && (this->items[sorted_items_idx[i]].weight < this->max_capacity - this->used_capacity))
                        {
                            // Pega o item
                            this->taked_items[sorted_items_idx[i]] = true;
                            this->used_capacity += this->items[sorted_items_idx[i]].weight;
                            taked = true;

                            // Adiciona o item a mochila do ladrao
                            this->thieves[j].second.items.push_back(sorted_items_idx[i]);
                            
                            // Adiciona a cidade a rota
                            auto pos = std::find(this->thieves[j].second.route.begin(), this->thieves[j].second.route.end(), this->items[sorted_items_idx[i]].city_idx);
                            if(pos == this->thieves[j].second.route.end())
                            {
                                this->thieves[j].second.route.push_back(this->items[sorted_items_idx[i]].city_idx);
                            }

                            // Deixe outro ladrao escolher
                            break;
                        }
                    }
                }
            }
            
            /*for(auto thief: this->thieves)
            {
                std::cout << "Itens: \n";
                std::cout << "Qtd: " << thief.second.items.size() << std::endl;
                for(auto x: thief.second.items)
                {
                    std::cout << x << " ";
                }std::cout<<std::endl;
                std::cout << "Fim Itens" << std::endl;

                std::cout << "Rota: \n";
                std::cout << "Qtd: " << thief.second.route.size() << std::endl;
                for(auto x: thief.second.route)
                {
                    std::cout << x << " ";
                }std::cout<<std::endl;
                std::cout << "Fim Rota" << std::endl;
            }*/

            for(int j = 0; j < this->thieves.size(); j++)
            {
                // Ordena os itens de maneira que ele va nas cidades mais distantes primeiro(pegando todos os itens de uma cidade de uma vez)
                std::sort(this->thieves[j].second.route.begin() + 1, this->thieves[j].second.route.end(), [&](int k1, int k2)
                {
                    return this->cities_distance[k1][0] < this->cities_distance[k2][0];
                });

                // Seta os pesos de quando ele sai de cada cidade
                for(auto visited_city: this->thieves[j].second.route)
                {
                    double weight_city = 0;
                    // Soma o peso de todos os itens pertencentes a cidade atual
                    for(auto stolen_item: this->thieves[j].second.items)
                    {
                        if(this->items[stolen_item].city_idx == visited_city){
                            weight_city += this->items[stolen_item].weight;
                        }
                    }

                    // Seta o peso de quando sai da cidade atual como o peso calculado
                    this->thieves[j].second.backpack_weight.push_back(weight_city);
                }
            }
        }

        void printRoutes()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrao " << i << std::endl << "rota: ";
                for(int j = 0; j < thieves[i].second.route.size(); j++)
                {
                    std::cout << "cidade " << thieves[i].second.route[j] << " em " << j << std::endl;
                }
                std::cout << "\nitems: \n";
                int totalValue = 0;
                for(int j = 0; j < thieves[i].second.items.size(); j++)
                {   
                    totalValue += this->items[thieves[i].second.items[j]].value;
                    std::cout << thieves[i].second.items[j] << std::endl;
                    //std::cout << "Valor: " << this->items[thieves[i].second.items[j]].value << "   Peso: " << this->items[thieves[i].second.items[j]].weight << std::endl;
                }
                std::cout << "Valor total adquirido: " << totalValue << std::endl;
            }
        }

        double evaluateRoutes()
        {
            double total = 0;
            int weight_j, weight_n;
            double value, total_value = 0.0;
            double rent, time = 0.0;
            double f_v = (this->max_speed - this->min_speed)/this->max_capacity;
            int current_city = 0, next_city = 0;

            for(int i = 0; i < this->thieves.size(); i++)
            {
                weight_n = value = rent = 0;

                for(int j = 0; j < thieves[i].second.items.size(); j++)
                {
                    value += (this->items[thieves[i].second.items[j]].value);
                }
                
                for(int k = 0; k < thieves[i].second.backpack_weight.size() -  1; k++)
                {
                    weight_n += thieves[i].second.backpack_weight[k];
                }

                weight_j = 0;
                for(int j = 0; j < (thieves[i].second.route.size() - 2); j++)
                {   
                    current_city = thieves[i].second.route[j];
                    next_city = thieves[i].second.route[j+1];
                    weight_j += thieves[i].second.backpack_weight[j];
                    rent += (this->cities_distance[current_city][next_city])/(this->max_speed - f_v*weight_j);
                }

                current_city =  thieves[i].second.route[thieves[i].second.route.size() - 2];

                rent += cities_distance[0][current_city] / (this->max_speed - (f_v * weight_n));
                total_value += value;
                time+= rent;

                //std::cout << i << " roubou " << value << " enquanto pagava " << this->renting_ratio *rent << std::endl;

                value -= this->renting_ratio * rent;
                
                //std::cout << "Valor do ladrao " << i << " : " << value << std::endl;

                total += value;
            }

            //std::cout << "Valor recolhido " << total_value << " com aluguel de  " << this->renting_ratio*time << std::endl;

            return total;
        }

        double maxZ()
        {
            // Funcao objetivo
            double maxZ;

            // Constante V
            double v = (this->max_speed - this->min_speed)/this->max_capacity;

            // Variaveis
            long long int total_value = 0;

            // Soma todo os valores dos itens
            for(int i = 0; i < this->taked_items.size(); i++)
            {
                total_value += this->items[i].value*this->taked_items[i];
            }

            // Calcula custo do percurso para todos os ladroes
            double time = 0;int current_city = 0;int next_city = 0;
            for(auto thief: this->thieves)
            {
                // Soma o tempo gasto da primeira cidade ate a ultima
                double Wx_i = 0;
                for(int i = 0; i < (thief.second.route.size() - 2); i++)
                {   
                    current_city = thief.second.route[i];
                    next_city = thief.second.route[i + 1];
                    Wx_i += thief.second.backpack_weight[i];

                    time += (this->cities_distance[current_city][next_city])/(this->max_speed - v*Wx_i);
                    
                    /*std::cout << std::endl;
                    std::cout << "iteration " << i << std::endl;
                    std::cout << "time " << time << std::endl;
                    std::cout << "cities_distance " << this->cities_distance[current_city][next_city] << std::endl;
                    std::cout << "Max Speed " << this->max_speed << std::endl;
                    std::cout << "Const Speed " << v << std::endl;
                    std::cout << "Wx_i " << Wx_i << std::endl;
                    std::cout << "time " << time << std::endl;
                    std::cout << std::endl;*/
                }

                // Soma o tempo gasto da ultima ate a inicial
                int last_city_idx = thief.second.route.size() - 2;
                current_city =  thief.second.route[last_city_idx];
                double Wx_n = Wx_i + thief.second.backpack_weight[last_city_idx];
         
                //std::cout << "Aluguel sem a ultima " << time << std::endl;
                time += (this->cities_distance[current_city][0])/(this->max_speed - (v*Wx_n));
                //std::cout << "Aluguel da ultima " << (this->cities_distance[current_city][0])/(this->max_speed - (v*Wx_n)) << std::endl; 
            }

            // Calcula funcao maxZ
            maxZ = total_value - this->renting_ratio*time;
            
            //std::cout << "Valor recolhido " << total_value << " com aluguel de  " << this->renting_ratio*time << std::endl;
            return maxZ;
        }
};