#include<bits/stdc++.h>

class item {
    public:
        float weight, value;

    // Construtor padrao
    item(){
        weight = 0.0;
        value = 0.0;
    }

    // Construtor
    item(float weight, float value){
        this->weight = weight;
        this->value = value;
    }
};

class city {
    public:
        float x, y;
        std::vector<item> itens;

        // Construtor padrao
        city(){
            x = 0.0;
            y = 0.0;
        }

        // Construtor
        city(float x, float y){
            this->x = x;
            this->y = y;
        }
};

class instance {
    public:
        // Atributos
        int num_cities, num_items, max_capacity;
        double min_speed, max_speed, renting_ratio;
        std::vector<std::vector<double> > cities_distance;
        std::vector<city> cities;

        // Construtor padrao
        instance(){
            num_cities = 0;
            num_items = 0;
            max_capacity = 0;
            min_speed = 0.0;
            max_speed = 0.0;
            renting_ratio = 0.0;
        }

        // Construtor
        instance(int num_cities, int num_items, int max_capacity, double min_speed, double max_speed, double renting_ratio){
            
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
};