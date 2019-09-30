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
        std::vector<float> timing;   
};

class Instance {
    public:
        // Atributos
        int num_cities, num_items, max_capacity;
        double min_speed, max_speed, renting_ratio, cur_speed;
        std::vector<std::vector<double> > cities_distance;
        std::vector<City> cities;
        std::vector<Thief> thieves;
        std::vector<std::pair<float,int>> backpack; //<tempo, peso>
        std::vector<Item> items;

        // Construtor padrao
        Instance(){
            num_cities = 0;
            num_items = 0;
            max_capacity = 0;
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
            this->min_speed = min_speed;
            this->max_speed = max_speed;
            this->cur_speed = max_speed;
            this->renting_ratio = renting_ratio;

            // Gera uma matriz com num_cities linhas e colunas preenchidas com as distancias zeradas
            std::vector<double> empty_distance;
            empty_distance.assign(num_cities, 0.0);
            cities_distance.assign(num_cities, empty_distance);
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

        void addThief()
        {
            Thief thief;
            thieves.push_back(thief);
        }

        void initialRoutes()
        {
            int used_capacity = 0;
            float f_v = (this->max_speed - this->min_speed)/this->max_capacity;
            bool item_in_city = true;

            for(int j = 0; j < thieves.size(); j++)
            {
                thieves[j].route.push_back(0);
                thieves[j].timing.push_back(0);
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

                        if(item_taken.weight < this->max_capacity - used_capacity)
                        {
                            if(std::find(thieves[j].items.begin(), thieves[j].items.end(), this_one) == thieves[j].items.end()) {
                                thieves[j].items.push_back(this_one);
                            }
                            used_capacity += item_taken.weight;

                            //adicionar cidade na rota
                            if(std::find(thieves[j].route.begin(), thieves[j].route.end(), i) == thieves[j].route.end()) {
                                thieves[j].route.push_back(i);
                                //adicionar tempo que ele saiu da cidade
                                float new_time = 0;
                                if(thieves[j].timing.size() > 1 && thieves[j].route.size() >= 2)
                                {
                                    new_time = thieves[j].timing.back();                                    
                                    new_time += this->cities_distance[thieves[j].route[thieves[j].route.size() - 1]][thieves[j].route[thieves[j].route.size() - 2]]/this->cur_speed;
                                }
                                thieves[j].timing.push_back(new_time);
                                this->backpack.push_back(std::make_pair(new_time, used_capacity));
                            }

                            //diminuir velocidade
                            if(cur_speed - f_v > this->min_speed)
                            {
                                this->cur_speed = cur_speed - f_v;
                            }
                            else
                            {
                                this->cur_speed = this->min_speed;
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
                if(used_capacity == this->max_capacity) break;                
            }
            for(int j = 0; j < thieves.size(); j++)
            {
                thieves[j].route.push_back(0);
                thieves[j].timing.push_back(cities_distance[thieves[j].route[thieves[j].route.size() - 1]][0]);
            } 
            
        }

        void printRoutes()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrão " << i << std::endl << "rota: ";
                for(int j = 0; j < thieves[i].route.size(); j++)
                {
                    std::cout << "cidade " << thieves[i].route[j] << " tempo " << thieves[i].timing[j] << std::endl;
                }
                std::cout << "\nitems: ";
                for(int j = 0; j < thieves[i].items.size(); j++)
                {
                    std::cout << thieves[i].items[j] << " " << std::endl;
                }
                std::cout << std::endl;
            }
        }

        float evaluateRoutes()
        {
            float total = 0;
            for(int i = 0; i < this->thieves.size(); i++)
            {
                float value = 0;
                float rent = 0;
                float f_v = (this->max_speed - this->min_speed)/this->max_capacity;
                int weight_n = 1;
                for(int k = 1; k < backpack.size(); k++)
                {
                    if(backpack[k].first == thieves[i].timing.back())
                    {
                        weight_n = backpack[k].second;
                        break;
                    }
                }

                for(int j = 2; j < thieves[i].items.size(); j++)
                {
                    value += (this->items[thieves[i].items[j]].weight * this->items[thieves[i].items[j]].value);
                }
                
                for(int j = 1; j < thieves[i].route.size() - 1; j++)
                {
                    int weight_i = 1;
                    for(int k = 1; k < backpack.size(); k++)
                    {
                        if(backpack[k].first == thieves[i].timing[j])
                        {
                            weight_i = backpack[k].second;
                            break;
                        }
                    }
                    
                    rent += this->renting_ratio * ( (cities_distance[j][j+1] / (f_v * weight_i))  + (cities_distance[j][j+1] / (f_v * weight_n)));
                }
                
                value -= rent;
                std::cout << "Valor do ladrão " << i << " : " << value << std::endl;
                
                total += value;
            }
            return total;
        }

};