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
        double min_speed, max_speed, renting_ratio, cur_speed;
        std::vector<std::vector<double> > cities_distance;
        std::vector<City> cities;
        std::vector<std::pair<double,Thief>> thieves; //<velocidade, ladrão>
        std::vector<Item> items;
        std::vector<bool> taked_items;

        // Construtor padrao
        Instance(){
            num_cities = 0;
            num_items = 0;
            max_capacity = 0;
            used_capacity = 0;
            min_speed = 0.0;
            max_speed = 0.0;
            cur_speed = 0.0;
            renting_ratio = 0.0;
        }

        // Construtor
        Instance(int num_cities, int num_items, int max_capacity, double min_speed, double max_speed, double renting_ratio){
            
            // Seta os dados base da instancia
            this->num_cities = num_cities;
            this->num_items = num_items;
            this->max_capacity = max_capacity;
            this->used_capacity = 0;
            this->min_speed = min_speed;
            this->max_speed = max_speed;
            this->cur_speed = max_speed;
            this->renting_ratio = renting_ratio;

            // Gera uma matriz com num_cities linhas e colunas preenchidas com as distancias zeradas
            std::vector<double> empty_distance;
            empty_distance.assign(num_cities, 0.0);
            cities_distance.assign(num_cities, empty_distance);

            // Inicializa todos os items como "nao pego"
            taked_items.assign(num_cities, false);
        }

        void setValues(int num_cities, int num_items, int max_capacity, double min_speed, double max_speed, double renting_ratio){
            
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
            cities_distance.assign(num_cities, empty_distance);
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
                            
                            if(cities[i].items_idx.size() == 0)  break;
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

        float evaluateRoutes()
        {
            float total = 0;
            int weight_j, weight_n;
            float value;
            float rent;
            float f_v = (this->max_speed - this->min_speed)/this->max_capacity;

            for(int i = 0; i < this->thieves.size(); i++)
            {
                value = rent = 0;

                weight_n = 0;

                for(int k = 0; k < thieves[i].second.backpack_weight.size(); k++)
                {
                    weight_n += thieves[i].second.backpack_weight[k];
                }

                for(int j = 0; j < thieves[i].second.items.size(); j++)
                {
                    value += (this->items[thieves[i].second.items[j]].value);
                }
                
                for(int j = 0; j < thieves[i].second.route.size() - 1; j++)
                {
                    weight_j = 0;
                    for(int k = 0; k < j; k++)
                    {
                        weight_j += thieves[i].second.backpack_weight[k];
                    }

                    rent += (cities_distance[j][j+1]) / (this->max_speed - (f_v * weight_j));
                }
                
                rent += cities_distance[0][thieves[i].second.route.size() - 1] / (this->max_speed - (f_v * weight_n));

                std::cout << i << " roubou " << value << " enquanto pagava " << this->renting_ratio *rent << std::endl;

                value -= this->renting_ratio * rent;
                
                std::cout << "Valor do ladrao " << i << " : " << value << std::endl;

                total += value;
            }
            std::cout << "A capacidade da mochila utilizada foi " << this->used_capacity << " de " << this->max_capacity << std::endl;

            return total;
        }

        float maxZ(){
            // Funcao objetivo
            double maxZ;

            // Constante V
            double v = (this->max_speed - this->min_speed)/this->max_capacity;

            // Variaveis
            int total_value = 0;

            // Soma todo os valores dos itens
            for(int i = 0; i < this->taked_items.size(); i++)
            {
                if(this->taked_items[i] == 1)
                {
                    total_value += this->items[i].value;
                }
            }

            // Calcula custo do percurso para todos os ladroes
            double time = 0;
            for(auto thief: this->thieves)
            {
                // Soma o tempo gasto da primeira cidade ate a ultima
                double Wx_i = 0;
                for(int i = 0; i < (thief.second.route.size() - 1); i++)
                {   
                    Wx_i += thief.second.backpack_weight[i];
                    time += (this->cities_distance[i][i+1])/(this->max_speed - v*Wx_i);
                }

                // Soma o tempo gasto da ultima ate a inicial
                int last_city_idx = thief.second.route.size() - 2;
                double Wx_n = Wx_i + thief.second.backpack_weight[last_city_idx];
                time += (this->cities_distance[last_city_idx][last_city_idx + 1])/(this->max_speed - v*Wx_n);
            }

            // Calcula funcao maxZ
            maxZ = total_value - this->renting_ratio*time;

            return maxZ;
        }
};