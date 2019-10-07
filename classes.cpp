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

    Item(const Item& item){
        this->weight = item.weight;
        this->value = item.value;
        this->city_idx = item.city_idx;
    }

    Item& operator=( const Item& item )
    {
        this->weight = item.weight;
        this->value = item.value;
        this->city_idx = item.city_idx;
        return *this;
    }
    
};

class City {
    public:
        float x, y;
        std::vector<int> items_idx;

        // Construtor padrao
        City()
        {
            x = 0.0;
            y = 0.0;
        }

        // Construtor
        City(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        City(const City& city)
        {
            this->x = city.x;
            this->y = city.y;
            this->items_idx = city.items_idx;
        }

        City& operator=( const City& city )
        {
            this->x = city.x;
            this->y = city.y;
            this->items_idx = city.items_idx;
            return *this;
        }
};

class Thief {
    public:
        std::vector<int> route;
        std::vector<int> items;
        std::vector<int> backpack_weight;   

    Thief()
    {
        route.reserve(50);
        items.reserve(50);
        backpack_weight.reserve(50);
    }

    Thief(const Thief& thief)
    {
        this->route = thief.route;
        this->items = thief.items;
        this->backpack_weight = thief.backpack_weight;
    }

    Thief& operator=( const Thief& thief )
    {
        this->route = thief.route;
        this->items = thief.items;
        this->backpack_weight = thief.backpack_weight;
        return *this;
    }


};

class Instance {
    public:
        /**
         * Atributos
         **/
        
        int num_cities, num_items, max_capacity, used_capacity;
        double min_speed, max_speed, renting_ratio;
        std::vector<std::vector<double> > cities_distance;
        std::vector<City> cities;
        std::vector<std::pair<double,Thief>> thieves; //<velocidade, ladrão>
        std::vector<Item> items;
        std::vector<int> taked_items;

        /**
         * Funcoes de inicializacao
         **/

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

        //Construtor de cópia
        Instance(const Instance& inst)
        {

            this->num_cities = inst.num_cities;
            this->num_items = inst.num_items;
            this->max_capacity = inst.max_capacity;
            this->used_capacity = inst.used_capacity;
            this->min_speed = inst.min_speed;
            this->max_speed = inst.max_speed;
            this->renting_ratio = inst.renting_ratio;
            this->cities_distance = inst.cities_distance;
            this->taked_items = inst.taked_items;

            this->cities = inst.cities;
            this->thieves = inst.thieves; 
            this->items = inst.items;

        } 
        
        Instance& operator=( const Instance& inst )
        {
            this->num_cities = inst.num_cities;
            this->num_items = inst.num_items;
            this->max_capacity = inst.max_capacity;
            this->used_capacity = inst.used_capacity;
            this->min_speed = inst.min_speed;
            this->max_speed = inst.max_speed;
            this->renting_ratio = inst.renting_ratio;
            this->cities_distance = inst.cities_distance;
            this->taked_items = inst.taked_items;

            this->cities = inst.cities;
            this->thieves = inst.thieves; 
            this->items = inst.items;

            return *this;
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

        void cleanSolution()
        {
            // Inicializa todos os items como "nao pego"
            this->taked_items.clear();
            this->taked_items.assign(num_items, 0);

            // Reinicializa a capacidade usada
            this->used_capacity = 0;

            // Reseta os atributos dos ladroes
            for(int i = 0; i < this->thieves.size(); i++){
                this->thieves[i].second.backpack_weight.clear();
                this->thieves[i].second.items.clear();
                this->thieves[i].second.route.clear();
            }
        }

        /**
         * Vizinhancas
         **/

        // Escolhe um ladrao aleatorio e troca a ordem de visitacao entre duas cidades do mesmo
        void swap_cities(bool verbose = false)
        {
            // Escolhe um ladrao aleatorio
            int choosed_thief = rand() % this->thieves.size();

            // Escolhe aleatoriamente duas cidades distintas(e que nao sejam a inicial) do ladrao
            int city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            while(city_1 == 0) city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            int city_2 = rand() % this->thieves[choosed_thief].second.route.size();
            while(city_2 == city_1 || city_2 == 0) city_2 = rand() % this->thieves[choosed_thief].second.route.size();

            // Printa as cidades escolhidas
            if(verbose)
                std::cout << "Trocou a cidade " << this->thieves[choosed_thief].second.route[city_1] << " com a cidade " << this->thieves[choosed_thief].second.route[city_2] << std::endl;

            // Troca as cidades de lugar
            int aux = this->thieves[choosed_thief].second.route[city_1];
            this->thieves[choosed_thief].second.route[city_1] = this->thieves[choosed_thief].second.route[city_2];
            this->thieves[choosed_thief].second.route[city_2] = aux;

            // Troca os pesos referentes as cidades de lugar
            aux = this->thieves[choosed_thief].second.backpack_weight[city_1];
            this->thieves[choosed_thief].second.backpack_weight[city_1] = this->thieves[choosed_thief].second.backpack_weight[city_2];
            this->thieves[choosed_thief].second.backpack_weight[city_2] = aux;
        }

        void move_cities(bool verbose = false)
        {
            // Escolhe um ladrao aleatorio
            int choosed_thief = rand() % this->thieves.size();

            // Escolhe aleatoriamente duas cidades distintas(e que nao sejam a inicial) do ladrao
            int city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            while(city_1 == 0) city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            int new_pos = rand() % this->thieves[choosed_thief].second.route.size();
            while(new_pos == 0) new_pos = rand() % this->thieves[choosed_thief].second.route.size();

            // Printa as cidades escolhidas
            if(verbose)
                //Trocou a cidade X para a segunda posicao
                ;
        }

        /**
         * Solucoes iniciais
         **/

        // Guloso: valor/peso
        void greedySolution1()
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

            // Reseta a solucao(para caso ja aja alguma)
            this->cleanSolution();

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

        //Guloso: valor/(peso*distancia)
        void greedySolution()
        {
            this->cleanSolution();

            std::vector<int> stolen_items(this->thieves.size());
            std::vector<int> reserved_items(this->items.size());

            for(int i = 0; i < reserved_items.size(); i++)
            {
                reserved_items[i] = 0;
            }

            for(int i = 0; i < this->thieves.size(); i++)
            {
                this->thieves[i].second.route.push_back(0);
                float better_cb = 0;
                int better_index = 0;
                Item better_item = this->items[0];

                for(int j = 0; j < this->items.size(); j++)
                {
                    if(reserved_items[j] == 0)
                    {
                        float cur_cb = this->items[j].value/(this->items[j].weight * this->cities_distance[0][this->items[j].city_idx]);
                        if(cur_cb > better_cb )
                        {
                            better_cb = cur_cb;
                            better_index = j;
                            better_item = this->items[j];
                        }
                        else if(cur_cb == better_cb && this->cities_distance[0][this->items[j].city_idx] < this->cities_distance[0][better_item.city_idx])
                        {
                            better_cb = cur_cb;
                            better_index = j;
                            better_item = this->items[j];
                        }
                    }
                }

                reserved_items[better_index] = 1;
                stolen_items[i] = better_index;

            }

            bool items_available = true;
            while(this->used_capacity < this->max_capacity && items_available)
            {
                for(int i = 0; i < this->thieves.size(); i++)
                {
                    if(this->used_capacity + this->items[stolen_items[i]].weight < this->max_capacity)
                    {
                        if(!this->taked_items[stolen_items[i]])
                        {
                            this->taked_items.push_back(stolen_items[i]); //marca o item como roubado
                            this->used_capacity += this->items[stolen_items[i]].weight;
                            this->thieves[i].second.items.push_back(stolen_items[i]);
                            this->taked_items[stolen_items[i]] = 1;

                            std::vector<int>::iterator it = std::find(thieves[i].second.route.begin(), thieves[i].second.route.end(), this->items[stolen_items[i]].city_idx);
                            if(it == thieves[i].second.route.end())
                            {
                                thieves[i].second.route.push_back(this->items[stolen_items[i]].city_idx);
                                thieves[i].second.backpack_weight.push_back(this->items[stolen_items[i]].weight);
                            }
                            else
                            {
                                int index = std::distance(thieves[i].second.route.begin(), it);
                                thieves[i].second.backpack_weight[index] += this->items[stolen_items[i]].weight;
                            }
                        }
                    }

                    float better_cb = -1;
                    int better_index = 0;
                    Item better_item = this->items[0];

                    for(int j = 0; j < this->items.size(); j++)
                    {
                        if(reserved_items[j] == 0)
                        {
                            float cur_cb = this->items[j].value/this->items[j].weight;
                            if(this->items[stolen_items[i]].city_idx != this->items[j].city_idx)
                            {
                                cur_cb = this->items[j].value/(this->items[j].weight * this->cities_distance[this->items[stolen_items[i]].city_idx][this->items[j].city_idx]);
                            }
                            if(cur_cb > better_cb )
                            {
                                better_cb = cur_cb;
                                better_index = j;
                                better_item = this->items[j];
                            }
                            else if(cur_cb == better_cb && this->cities_distance[0][this->items[j].city_idx] < this->cities_distance[this->items[stolen_items[i]].city_idx][better_item.city_idx])
                            {
                                better_cb = cur_cb;
                                better_index = j;
                                better_item = this->items[j];
                            }
                            items_available = true;
                        }
                        else
                        {
                            items_available = false;
                        }
                    }

                    reserved_items[better_index] = 1;
                    stolen_items[i] = better_index;

                }
            }            
        }

        /**
         * Funcoes de impressao
         **/

        void printRoutes()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrao " << i << std::endl << "rota: ";
                for(int j = 0; j < thieves[i].second.route.size(); j++)
                {
                    std::cout << "cidade " << thieves[i].second.route[j] << " em " << j << std::endl;
                }
            }
        }

        void printItems()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrao " << i << std::endl;std::cout << "items: \n";
                for(int j = 0; j < thieves[i].second.items.size(); j++)
                {   
                    std::cout << "item " << thieves[i].second.items[j] << " com valor " << this->items[thieves[i].second.items[j]].value << std::endl;
                }
            }
        }

        void printWeights()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrao " << i << std::endl << "pesos: \n";
                for(int j = 0; j < thieves[i].second.backpack_weight.size(); j++){
                    std::cout << "peso " << thieves[i].second.backpack_weight[j] << " apos sair de " << thieves[i].second.route[j] << std::endl;
                }
            }
        }

        /**
         * Funcao de avaliacao
         **/

        double objectiveFunction()
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
                for(int i = 0; i < (thief.second.route.size() - 1); i++)
                {   
                    current_city = thief.second.route[i];
                    next_city = thief.second.route[i + 1];
                    Wx_i += thief.second.backpack_weight[i];

                    time += (this->cities_distance[current_city][next_city])/(this->max_speed - v*Wx_i);
                }

                // Soma o tempo gasto da ultima ate a inicial
                int last_city_idx = thief.second.route.size() - 1;
                current_city =  thief.second.route[last_city_idx];
                double Wx_n = Wx_i + thief.second.backpack_weight[last_city_idx];
         
                //std::cout << "Aluguel sem a ultima " << time << std::endl;
                time += (this->cities_distance[current_city][0])/(this->max_speed - (v*Wx_n));
                //std::cout << "idx_cidade: " << current_city << " max_speed: " << this->max_speed <<  " v_const: " << v;
                //std::cout << " Wn: " << Wx_n << std::endl; 
                //std::cout << "Aluguel da ultima " << (this->cities_distance[current_city][0])/(this->max_speed - (v*Wx_n)) << std::endl; 
            }

            // Calcula funcao maxZ
            maxZ = total_value - this->renting_ratio*time;
            
            return maxZ;
        }

        /**
         * Verificação de restrições
         **/

        bool solutionValid()
        {
            //capacidade da mochila
            return this->used_capacity <= this->max_capacity;
        }
};



