#include<bits/stdc++.h>

class Item {
    public:
        float weight, value;

    // Construtor padrao
    Item(){
        weight = 0.0;
        value = 0.0;
    }

    // Construtor
    Item(float weight, float value){
        this->weight = weight;
        this->value = value;
    }
};

class City {
    public:
        float x, y;
        std::vector<Item> items;

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
            int aux = this->items[position];
            this->items.erase (this->items.begin(),this->items.begin()+position);
            return aux;
        }

        void returnItem(int index)
        {
            this->items.pushback(index);
        }
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
            thieves.pushback(thief);
        }

        void initialRoutes()
        {
            used_capacity = 0;
            float f_v = (this->max_speed - this->min_speed)/this->max_capacity;
            while(used_capacity < this->max_capacity)
            {
                for(int i = 0; i < cities.size(); i++)
                {
                    while(cities[i].items.size() > 0){
                        for(int j = 0; j < thieves.size(); j++)
                        {

                            //pegar alguma coisa 
                            int this_one = cities[i].items.takeItem(abs(rand() % cities[i].items.size()));
                            Item item_taken = this->items[this_one];

                            if(item_taken.weight < this->max_capacity - used_capacity)
                            {
                                thieves[j].items.pushback(item_taken);
                                used_capacity += item_taken.weight;
                                
                                //adicionar cidade na rota
                                thieves[j].route.pushback(i);
                                //adicionar tempo que ele saiu da cidade
                                //tempo que ele sai da cidade = o ultimo tempo + (distancia da ultima cidade com a penultima)/velocidade
                                try{
                                    float new_time = thieves[j].timing.back();                                    
                                    new_time = this->cities_distance[thieves[j].route[len(thieves[j].route) - 1]][thieves[j].route[len(thieves[j].route) - 2]]/this->cur_speed;
                                }
                                catch(int e)
                                {
                                    float new_time = 0;
                                }
                                thieves[j].timing.pushback(new_time);
                                //diminuir velocidade
                                this->cur_speed = cur_speed * f_v;
                                if(cities[i].items.size() == 0) break;
                            }
                        }
                    }    
                if(used_capacity == this->max_capacity) break;                
                }
            }
        }

};

class Thief {
    public:
        std::vector<int> route;
        std::vector<Item> items;
        std::vector<float> timing;

    //criar rota
    //setar mochila
    //avaliar rota
    
}