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
        std::vector<int> caught_items;

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
            this->caught_items.assign(num_items, 0);
        }

        // Construtor de cópia
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
            this->caught_items = inst.caught_items;

            this->cities = inst.cities;
            this->thieves = inst.thieves; 
            this->items = inst.items;

        } 
        
        // Sobrecarga do operador de atribuicao
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
            this->caught_items = inst.caught_items;

            this->cities = inst.cities;
            this->thieves = inst.thieves; 
            this->items = inst.items;

            return *this;
        }

        // Seta os valores basicos da instancia
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
            this->caught_items.assign(num_items, 0);
        }

        // Adiciona n ladroes a instancia
        void addThief(int n = 1)
        {
            Thief thief;
            for(int i = 0; i < n; i++)
                thieves.push_back(std::make_pair(this->max_speed, thief));
        }

        // Zera a solucao da instancia(caso haja alguma)
        void cleanSolution()
        {
            // Inicializa todos os items como "nao pego"
            this->caught_items.clear();
            this->caught_items.assign(num_items, 0);

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
        void swap_cities(int choosed_thief, int city_1, int city_2, bool verbose = false)
        {
            /* // Escolhe um ladrao aleatorio
            int choosed_thief = rand() % this->thieves.size();

            // Escolhe aleatoriamente duas cidades distintas(e que nao sejam a inicial) do ladrao
            int city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            while(city_1 == 0) city_1 = rand() % this->thieves[choosed_thief].second.route.size();
            int city_2 = rand() % this->thieves[choosed_thief].second.route.size();
            while(city_2 == city_1 || city_2 == 0) city_2 = rand() % this->thieves[choosed_thief].second.route.size(); */

            // Printa as cidades escolhidas
            if(verbose)
                std::cout << "Trocou a cidade " << this->thieves[choosed_thief].second.route[city_1] << " com a cidade " << this->thieves[choosed_thief].second.route[city_2] << std::endl;

            // Troca as cidades de lugar
            //std::cout << "troca as cidades de lugar na rota" << std::endl;
            int aux = this->thieves[choosed_thief].second.route[city_1];
            this->thieves[choosed_thief].second.route[city_1] = this->thieves[choosed_thief].second.route[city_2];
            this->thieves[choosed_thief].second.route[city_2] = aux;

            // Troca os pesos referentes as cidades de lugar
            //std::cout << "troca os pesos relativos as cidades de lugar" << std::endl;
            aux = this->thieves[choosed_thief].second.backpack_weight[city_1];
            this->thieves[choosed_thief].second.backpack_weight[city_1] = this->thieves[choosed_thief].second.backpack_weight[city_2];
            this->thieves[choosed_thief].second.backpack_weight[city_2] = aux;
        }

        // Escolhe um ladrao aleatorio e troca a ordem em que uma cidade aleatoria eh visitada
        void move_cities(int choosed_thief, int choosed_city, int new_pos, bool verbose = false)
        {
            /* // Escolhe um ladrao aleatorio
            int choosed_thief = rand() % this->thieves.size();

            // Escolhe aleatoriamente uma cidade(diferente da inicial) e uma nova posicao(diferente da inicial) para move-la
            int choosed_city = rand() % this->thieves[choosed_thief].second.route.size();
            while(choosed_city == 0) choosed_city = rand() % this->thieves[choosed_thief].second.route.size();
            int new_pos = rand() % this->thieves[choosed_thief].second.route.size();
            while(new_pos == 0 || choosed_city == new_pos) new_pos = rand() % this->thieves[choosed_thief].second.route.size(); */

            // Printa as cidades escolhidas
            if(verbose)
                std::cout << "Moveu a cidade " << this->thieves[choosed_thief].second.route[choosed_city] << " para a posicao " << new_pos << " da rota." << std::endl;

            int aux = this->thieves[choosed_thief].second.route[choosed_city];
            int weight_aux = this->thieves[choosed_thief].second.backpack_weight[choosed_city];

            if(new_pos < choosed_city)
            {
                if(verbose)std::cout << "remove cidade e depois insere "  << this->thieves[choosed_thief].second.route[choosed_city] << std::endl;
                // Caso a nova posicao seja antes da posicao atual, entao remova a cidade da solucao antes de inseri-la novamente
                this->thieves[choosed_thief].second.route.erase(this->thieves[choosed_thief].second.route.begin() + choosed_city);
                this->thieves[choosed_thief].second.route.insert(this->thieves[choosed_thief].second.route.begin() + new_pos, aux);

                if(verbose)std::cout << "remove peso e depois insere"  << std::endl;
                // O mesmo para o peso
                this->thieves[choosed_thief].second.backpack_weight.erase(this->thieves[choosed_thief].second.backpack_weight.begin() + choosed_city);
                this->thieves[choosed_thief].second.backpack_weight.insert(this->thieves[choosed_thief].second.backpack_weight.begin() + new_pos, weight_aux);
            }
            else
            {
                if(verbose)std::cout << "insere cidade e depois remove"  << std::endl;
                // Caso a nova posicao seja depois da posicao atual, entao insira a cidade da solucao antes de remove-la
                this->thieves[choosed_thief].second.route.insert(this->thieves[choosed_thief].second.route.begin() + new_pos + 1, aux);
                this->thieves[choosed_thief].second.route.erase(this->thieves[choosed_thief].second.route.begin() + choosed_city);

                if(verbose)std::cout << "insere peso e depois remove"  << std::endl;
                // O mesmo para o peso
                this->thieves[choosed_thief].second.backpack_weight.insert(this->thieves[choosed_thief].second.backpack_weight.begin() + new_pos + 1, weight_aux);
                this->thieves[choosed_thief].second.backpack_weight.erase(this->thieves[choosed_thief].second.backpack_weight.begin() + choosed_city);
            }
        }

        void exchange_items(int choosed_thief, int remove_item, int insert_item, bool verbose = false)
        {

            if(this->thieves[choosed_thief].second.items.size() > remove_item)
            {
                if(this->caught_items[insert_item] == 0)
                {
                    double p = ((double) rand() / (RAND_MAX));
                    if(p >= 0.5 && ((this->items[insert_item].weight + this->used_capacity - this->items[remove_item].weight) <= this->max_capacity))
                    {
                        bool remove_city = true;
                        for(int j = 0; j < this->thieves[choosed_thief].second.items.size(); j++)
                        {
                            int thief_item = this->thieves[choosed_thief].second.items[j];
                            if(this->items[remove_item].city_idx == this->items[thief_item].city_idx && thief_item != remove_item)
                            {
                                remove_city = false;
                                break;
                            }
                        }
                        
                        this->thieves[choosed_thief].second.items.erase(this->thieves[choosed_thief].second.items.begin() + remove_item); 
                        caught_items[remove_item] = 0;

                        auto pos = std::find(this->thieves[choosed_thief].second.route.begin(), this->thieves[choosed_thief].second.route.end(), this->items[remove_item].city_idx);
                        int index_city = std::distance(thieves[choosed_thief].second.route.begin(), pos);
                        this->thieves[choosed_thief].second.backpack_weight[index_city] -= this->items[remove_item].weight;
                        this->used_capacity -= this->items[remove_item].weight;

                        if(verbose) std::cout << "Removi item " << remove_item << std::endl;

                        if(remove_city && pos != this->thieves[choosed_thief].second.route.end())
                        {
                            this->thieves[choosed_thief].second.route.erase(pos);
                            if(verbose) std::cout << "Removi cidade " << this->items[remove_item].city_idx << std::endl;
                        }

                        this->thieves[choosed_thief].second.items.push_back(insert_item);
                        caught_items[insert_item] = 1;
                        if(verbose) std::cout << "Inseri item " << insert_item << std::endl;

                        if(pos == this->thieves[choosed_thief].second.route.end())
                        {
                            int position = rand() % this->thieves[choosed_thief].second.route.size();
                            this->thieves[choosed_thief].second.route.insert(this->thieves[choosed_thief].second.route.begin() + position, 1, this->items[insert_item].city_idx);
                            this->thieves[choosed_thief].second.backpack_weight.insert(this->thieves[choosed_thief].second.backpack_weight.begin() + position, 1, this->items[insert_item].weight);
                            //this->thieves[choosed_thief].second.route.push_back(this->items[insert_item].city_idx);
                            //this->thieves[choosed_thief].second.backpack_weight.push_back(this->items[insert_item].weight);
                        }
                        else
                        {
                            int index = std::distance(thieves[choosed_thief].second.route.begin(), pos);
                            this->thieves[choosed_thief].second.backpack_weight[index] += this->items[insert_item].weight;
                        }

                        //se couber o novo item na mochila
                        //remover remove_item e cidade se precisar
                        //setar caught_items
                        //inserir insert_item
                        //colocar cidade na rota
                        //incrementar peso da mochila
                    }
                    else
                    {
                        bool remove_city = true;
                        for(int j = 0; j < this->thieves[choosed_thief].second.items.size(); j++)
                        {
                            int thief_item = this->thieves[choosed_thief].second.items[j];
                            if(this->items[remove_item].city_idx == this->items[thief_item].city_idx && thief_item != remove_item)
                            {
                                remove_city = false;
                                break;
                            }
                        }
                        auto pos = std::find(this->thieves[choosed_thief].second.route.begin(), this->thieves[choosed_thief].second.route.end(), this->items[remove_item].city_idx);
                        if(remove_city && pos != this->thieves[choosed_thief].second.route.end())
                        {
                            this->thieves[choosed_thief].second.route.erase(pos);
                            this->thieves[choosed_thief].second.items.erase(this->thieves[choosed_thief].second.items.begin() + remove_item);
                            caught_items[remove_item] = 0;
                            int index_city = std::distance(thieves[choosed_thief].second.route.begin(), pos);
                            this->thieves[choosed_thief].second.backpack_weight[index_city] -= this->items[remove_item].weight;
                            this->used_capacity -= this->items[remove_item].weight;

                            if(verbose) std::cout << "Removi item sem colocar outro " << remove_item << std::endl;

                        }
                    }
                }
            }

            
        }

        void exchange_random_items(bool verbose = false)
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                if(verbose) std::cout << "Ladrão " << i << std::endl;

                int remove_item = rand() % this->thieves[i].second.items.size();
                int insert_item = rand() % this->items.size();

                int tries = 0;

                while(this->caught_items[insert_item] == 1 && tries <= this->items.size()/3)
                {
                    insert_item = rand() % this->items.size();
                }

                if(verbose) std::cout << "insert " << insert_item << " remove " << remove_item << std::endl;

                if(this->caught_items[insert_item] == 0)
                {
                    double p = ((double) rand() / (RAND_MAX));
                    if(p >= 0.5 && ((this->items[insert_item].weight + this->used_capacity - this->items[remove_item].weight) <= this->max_capacity))
                    {
                        bool remove_city = true;
                        for(int j = 0; j < this->thieves[i].second.items.size(); j++)
                        {
                            int thief_item = this->thieves[i].second.items[j];
                            if(this->items[remove_item].city_idx == this->items[thief_item].city_idx && thief_item != remove_item)
                            {
                                remove_city = false;
                                break;
                            }
                        }
                        
                        this->thieves[i].second.items.erase(this->thieves[i].second.items.begin() + remove_item); 
                        caught_items[remove_item] = 0;

                        if(verbose) std::cout << "Removi item " << remove_item << std::endl;

                        auto pos = std::find(this->thieves[i].second.route.begin(), this->thieves[i].second.route.end(), this->items[remove_item].city_idx);
                        if(remove_city && pos != this->thieves[i].second.route.end())
                        {
                            this->thieves[i].second.route.erase(pos);
                            if(verbose) std::cout << "Removi cidade " << this->items[remove_item].city_idx << std::endl;
                        }
                        

                        this->thieves[i].second.items.push_back(insert_item);
                        caught_items[insert_item] = 1;
                        if(verbose) std::cout << "Inseri item " << insert_item << std::endl;

                        if(pos == this->thieves[i].second.route.end())
                        {
                            this->thieves[i].second.route.push_back(this->items[insert_item].city_idx);
                            this->thieves[i].second.backpack_weight.push_back(this->items[insert_item].weight);
                        }
                        else
                        {
                            int index = std::distance(thieves[i].second.route.begin(), pos);
                            this->thieves[i].second.backpack_weight[index] += this->items[insert_item].weight;
                        }

                        //se couber o novo item na mochila
                        //remover remove_item e cidade se precisar
                        //setar caught_items
                        //inserir insert_item
                        //colocar cidade na rota
                        //incrementar peso da mochila
                    }
                    else
                    {
                        bool remove_city = true;
                        for(int j = 0; j < this->thieves[i].second.items.size(); j++)
                        {
                            int thief_item = this->thieves[i].second.items[j];
                            if(this->items[remove_item].city_idx == this->items[thief_item].city_idx && thief_item != remove_item)
                            {
                                remove_city = false;
                                break;
                            }
                        }
                        auto pos = std::find(this->thieves[i].second.route.begin(), this->thieves[i].second.route.end(), this->items[remove_item].city_idx);
                        if(remove_city && pos != this->thieves[i].second.route.end())
                        {
                            this->thieves[i].second.route.erase(pos);
                            this->thieves[i].second.items.erase(this->thieves[i].second.items.begin() + remove_item);
                            caught_items[remove_item] = 0;

                            if(verbose) std::cout << "Removi item sem colocar outro " << remove_item << std::endl;

                        }
                        
                        //se há algum item com a cidade igual o dele nao remove
                        //senao remove o item e a cidade da rota
                    }
                }
            }
        }

        /**
         * Solucoes iniciais
         **/

        //Guloso: valor/distancia   
        void greedySolution()
        {
            this->cleanSolution();

            std::vector<int> better_item(this->thieves.size());
            std::vector<int> aux_caught_items(this->items.size(), 0);
            bool items_remaining = true;

            int current_city = 0;

            for(int i = 0; i < this->thieves.size(); i++)
            {
                this->thieves[i].second.route.push_back(current_city);
                this->thieves[i].second.backpack_weight.push_back(0);
            }

            while(items_remaining)
            {
                for(int i = 0; i < this->thieves.size(); i++)
                {
                    float best_val = 0;
                    int best_index = 0;
                    float cur_val = 0;
                    for(int j = 0; j < this->items.size(); j++)
                    {
                        if(aux_caught_items[j] == 0)
                        {
                            if(this->cities_distance[current_city][this->items[j].city_idx] != 0) 
                                cur_val = this->items[j].value / this->cities_distance[current_city][this->items[j].city_idx];
                            else
                                cur_val = this->items[j].value;
                            if(cur_val > best_val)
                            {
                                best_val = cur_val;
                                best_index = j;
                            }
                            items_remaining = true;
                        }
                        else
                        {
                            items_remaining = false;
                        }
                    }
                    better_item[i] = best_index;
                    aux_caught_items[best_index] = 1;

                    if(this->items[best_index].weight < this->max_capacity - this->used_capacity)
                    {
                        current_city = this->items[best_index].city_idx;

                        // Pega o item
                        this->caught_items[best_index] = 1;
                        this->used_capacity += this->items[best_index].weight;

                        // Adiciona o item a mochila do ladrao
                        this->thieves[i].second.items.push_back(best_index);
                        
                        // Adiciona a cidade a rota
                        auto pos = std::find(this->thieves[i].second.route.begin(), this->thieves[i].second.route.end(), this->items[best_index].city_idx);
                        if(pos == this->thieves[i].second.route.end())
                        {
                            this->thieves[i].second.route.push_back(current_city);
                            this->thieves[i].second.backpack_weight.push_back(this->items[best_index].weight);
                        }
                        else
                        {
                            int index = std::distance(thieves[i].second.route.begin(), pos);
                            this->thieves[i].second.backpack_weight[index] += this->items[best_index].weight;
                        }
                    }
                }    
            }
        }

        /**
         *  Funcoes auxiliares
         **/
        void disturbe(int n_disturbe)
        {
            for(int i = 0; i < n_disturbe; i++)
            {
                // Escolhe uma vizinhanca aleatoriamente
                int neighborhood = rand() % 3;

                if(neighborhood == 0) // swap_cities
                {
                    // Escolhe um ladrao aleatorio
                    int choosed_thief = rand() % this->thieves.size();

                    // Escolhe aleatoriamente duas cidades distintas(e que nao sejam a inicial) do ladrao
                    int city_1 = rand() % this->thieves[choosed_thief].second.route.size();
                    while(city_1 == 0) city_1 = rand() % this->thieves[choosed_thief].second.route.size();
                    int city_2 = rand() % this->thieves[choosed_thief].second.route.size();
                    while(city_2 == city_1 || city_2 == 0) city_2 = rand() % this->thieves[choosed_thief].second.route.size();
                    
                    // Aplica a perturbacao
                    this->swap_cities(choosed_thief, city_1, city_2);
                    
                }
                else if(neighborhood == 1) // move_cities
                {
                    // Escolhe um ladrao aleatorio
                    int choosed_thief = rand() % this->thieves.size();

                    // Escolhe aleatoriamente uma cidade(diferente da inicial) e uma nova posicao(diferente da inicial) para move-la
                    int choosed_city = rand() % this->thieves[choosed_thief].second.route.size();
                    while(choosed_city == 0) choosed_city = rand() % this->thieves[choosed_thief].second.route.size();
                    int new_pos = rand() % this->thieves[choosed_thief].second.route.size();
                    while(new_pos == 0 || choosed_city == new_pos) new_pos = rand() % this->thieves[choosed_thief].second.route.size();
                    
                    // Aplica a perturbacao
                    this->move_cities(choosed_thief, choosed_city, new_pos);
                }
                /* else if(neighborhood == 2) // swap_items_btw_thieves
                {
                    // Escolhe dois ladroes aleatoriamente
                    int thief_1 = rand() % this->thieves.size();
                    int thief_2 = rand() % this->thieves.size();
                    while(thief_2 == thief_1) thief_2 = rand() % this->thieves.size();

                    // Escolhe aleatoriamente um item de cada ladrao
                    int item_1 = rand() % this->thieves[thief_1].second.items.size();
                    int item_2 = rand() % this->thieves[thief_2].second.items.size();
                    
                    // Aplica a perturbacao
                    this->swap_items_btw_thieves(thief_1, thief_2, item_1, item_2);
                } */
                /* else // exchange_items
                {
                    // Aplica a perturbacao
                    this->exchange_random_items();
                } */
            }
        }

        /**
         * Funcoes de impressao
         **/
        
        // Printa a rota de cada ladrao
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

        // Printa os itens de cada ladrao
        void printItems()
        {
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "ladrao " << i << std::endl;std::cout << "items: \n";
                for(int j = 0; j < thieves[i].second.items.size(); j++)
                {   
                    std::cout << "item " << thieves[i].second.items[j] << " com valor " << this->items[thieves[i].second.items[j]].value << " e peso " << this->items[thieves[i].second.items[j]].weight <<  std::endl;
                }
            }
        }

        // Printa o peso adquirido em cada cidade, de cada ladrao
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

        // Gera a saida aceita pela aplicacao web de visualizacao de solucao
        void output()
        {
            for(auto thief: this->thieves){
                
                if(thief.second.route.size() <= 1) continue;

                // Printa a rota
                std::cout << "[";
                for(int i = 1; i < thief.second.route.size(); i++){
                    if(i > 1) std::cout << ',';
                    std::cout << thief.second.route[i] + 1;
                }std::cout << "]" << std::endl;

                // Printa os itens
                std::cout << "[";
                for(int i = 0; i < thief.second.items.size(); i++){
                    if(i > 0) std::cout << ',';
                    std::cout << thief.second.items[i] + 1;
                }std::cout << "]" << std::endl;
            }
        }

        /**
         * Funcao de avaliacao
         **/

        double objectiveFunction(bool verbose = false)
        {
            // Funcao objetivo
            double maxZ;

            // Constante V
            double v = (this->max_speed - this->min_speed)/this->max_capacity;

            // Variaveis
            long long int total_value = 0;

            // Soma todo os valores dos itens
            for(int i = 0; i < this->caught_items.size(); i++)
            {
                total_value += this->items[i].value*this->caught_items[i];
            }
            if(verbose) std::cout << "Profit: " << total_value << std::endl;

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

                time += (this->cities_distance[current_city][0])/(this->max_speed - (v*Wx_n));
            }

            // Soma todo os valores de peso
            double total_weight = 0;
            for(int i = 0; i < this->caught_items.size(); i++)
            {
                total_weight += this->items[i].weight*this->caught_items[i];
            }
            if(verbose) std::cout << "Weight: " << total_weight << std::endl;

            // Calcula funcao maxZ
            //std::cout << "Profit " << total_value << " Rent " << time << std::endl;
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
