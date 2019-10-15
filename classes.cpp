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
            this->taked_items = inst.taked_items;

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
            this->taked_items = inst.taked_items;

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
            this->taked_items.assign(num_items, 0);
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
            int aux = this->thieves[choosed_thief].second.route[city_1];
            this->thieves[choosed_thief].second.route[city_1] = this->thieves[choosed_thief].second.route[city_2];
            this->thieves[choosed_thief].second.route[city_2] = aux;

            // Troca os pesos referentes as cidades de lugar
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
                // Caso a nova posicao seja antes da posicao atual, entao remova a cidade da solucao antes de inseri-la novamente
                this->thieves[choosed_thief].second.route.erase(this->thieves[choosed_thief].second.route.begin() + choosed_city);
                this->thieves[choosed_thief].second.route.insert(this->thieves[choosed_thief].second.route.begin() + new_pos, aux);

                // O mesmo para o peso
                this->thieves[choosed_thief].second.backpack_weight.erase(this->thieves[choosed_thief].second.backpack_weight.begin() + choosed_city);
                this->thieves[choosed_thief].second.backpack_weight.insert(this->thieves[choosed_thief].second.backpack_weight.begin() + new_pos, weight_aux);
            }
            else
            {
                // Caso a nova posicao seja depois da posicao atual, entao insira a cidade da solucao antes de remove-la
                this->thieves[choosed_thief].second.route.insert(this->thieves[choosed_thief].second.route.begin() + new_pos + 1, aux);
                this->thieves[choosed_thief].second.route.erase(this->thieves[choosed_thief].second.route.begin() + choosed_city);

                // O mesmo para o peso
                this->thieves[choosed_thief].second.backpack_weight.insert(this->thieves[choosed_thief].second.backpack_weight.begin() + new_pos + 1, weight_aux);
                this->thieves[choosed_thief].second.backpack_weight.erase(this->thieves[choosed_thief].second.backpack_weight.begin() + choosed_city);
            }
        }

        // Escolhe dois ladroes aleatoriamente e troca dois itens(um de cada) entre eles
        void swap_items_btw_thieves(int thief_1, int thief_2, int item_1, int item_2, bool verbose = false)
        {
            /* // Escolhe dois ladroes aleatoriamente
            int thief_1 = rand() % this->thieves.size();
            int thief_2 = rand() % this->thieves.size();
            while(thief_2 == thief_1) thief_2 = rand() % this->thieves.size();

            // Escolhe aleatoriamente um item de cada ladrao
            int item_1 = rand() % this->thieves[thief_1].second.items.size();
            int item_2 = rand() % this->thieves[thief_2].second.items.size(); */

            // Verifica se a cidade que contem o novo item ja esta na rota do ladrao
            // Se sim, altera o peso adquirido na mesma
            // Se nao, adiciona a cidade na rota e seta o peso da mesma
            auto pos_1 = std::find(this->thieves[thief_1].second.route.begin(), 
                                    this->thieves[thief_1].second.route.end(), 
                                    this->items[this->thieves[thief_2].second.items[item_2]].city_idx);
            int int_pos_1 = std::distance(this->thieves[thief_1].second.route.begin(), pos_1);
            if(pos_1 != this->thieves[thief_1].second.route.end())
            {
                this->thieves[thief_1].second.backpack_weight[int_pos_1] += this->items[this->thieves[thief_2].second.items[item_2]].weight;
            }
            else
            {
                // Adquire a cidade mais proxima
                int nearest = this->thieves[thief_1].second.route[0];
                for(int i = this->thieves[thief_1].second.route[1]; i < this->thieves[thief_1].second.route.size(); i++)
                {
                    if(this->cities_distance[this->items[this->thieves[thief_2].second.items[item_2]].city_idx][this->thieves[thief_1].second.route[i]] < this->cities_distance[this->items[this->thieves[thief_2].second.items[item_2]].city_idx][nearest])
                        nearest = this->thieves[thief_1].second.route[i];
                }

                // Insere a nova cidade logo apos a cidade mais proxima a ela
                auto new_pos = std::find(this->thieves[thief_1].second.route.begin(), 
                                    this->thieves[thief_1].second.route.end(), 
                                    nearest);
                this->thieves[thief_1].second.route.insert(new_pos, this->items[this->thieves[thief_2].second.items[item_2]].city_idx);
            }

            auto pos_2 = std::find(this->thieves[thief_2].second.route.begin(), 
                                    this->thieves[thief_2].second.route.end(), 
                                    this->items[this->thieves[thief_1].second.items[item_1]].city_idx);
            int int_pos_2 = std::distance(this->thieves[thief_2].second.route.begin(), pos_2);
            if(pos_2 != this->thieves[thief_2].second.route.end())
            {
                this->thieves[thief_2].second.backpack_weight[int_pos_2] += this->items[this->thieves[thief_1].second.items[item_1]].weight;
            }
            else
            {
                // Adquire a cidade mais proxima
                int nearest = this->thieves[thief_2].second.route[0];
                for(int i = this->thieves[thief_2].second.route[1]; i < this->thieves[thief_2].second.route.size(); i++)
                {
                    if(this->cities_distance[this->items[this->thieves[thief_1].second.items[item_1]].city_idx][this->thieves[thief_2].second.route[i]] < this->cities_distance[this->items[this->thieves[thief_1].second.items[item_1]].city_idx][nearest])
                        nearest = this->thieves[thief_2].second.route[i];
                }

                // Insere a nova cidade logo apos a cidade mais proxima a ela
                auto new_pos = std::find(this->thieves[thief_2].second.route.begin(), 
                                    this->thieves[thief_2].second.route.end(), 
                                    nearest);
                this->thieves[thief_2].second.route.insert(new_pos, this->items[this->thieves[thief_1].second.items[item_1]].city_idx);
            }

            // Retira os pesos dos itens dos ladroes:
            // Caso o ladrao so pegue este item da cidade, entao remova a cidade e o peso relativo a ela da solucao
            // Caso pegue outros itens, apenas desconta o peso do item
            int pos_item_1 = std::distance(this->thieves[thief_1].second.route.begin(), 
                                        std::find(this->thieves[thief_1].second.route.begin(), this->thieves[thief_1].second.route.end(),
                                                    this->items[this->thieves[thief_1].second.items[item_1]].city_idx));
            if(this->thieves[thief_1].second.backpack_weight[pos_item_1] == this->items[this->thieves[thief_1].second.items[item_1]].weight)
            {
                this->thieves[thief_1].second.route.erase(this->thieves[thief_1].second.route.begin() + pos_item_1);
                this->thieves[thief_1].second.backpack_weight.erase(this->thieves[thief_1].second.backpack_weight.begin() + pos_item_1);
            }
            else
            {
                this->thieves[thief_1].second.backpack_weight[pos_item_1] -= this->items[this->thieves[thief_1].second.items[item_1]].weight;
            }

            int pos_item_2 = std::distance(this->thieves[thief_2].second.route.begin(), 
                                        std::find(this->thieves[thief_2].second.route.begin(), this->thieves[thief_2].second.route.end(),
                                                    this->items[this->thieves[thief_2].second.items[item_2]].city_idx));
            if(this->thieves[thief_2].second.backpack_weight[pos_item_2] == this->items[this->thieves[thief_2].second.items[item_2]].weight)
            {
                this->thieves[thief_2].second.route.erase(this->thieves[thief_2].second.route.begin() + pos_item_2);
                this->thieves[thief_2].second.backpack_weight.erase(this->thieves[thief_2].second.backpack_weight.begin() + pos_item_2);
            }
            else
            {
                this->thieves[thief_2].second.backpack_weight[pos_item_2] -= this->items[this->thieves[thief_2].second.items[item_2]].weight;
            }
        
            // Troca os itens entre os ladroes
            std::iter_swap(this->thieves[thief_1].second.items.begin() + item_1, this->thieves[thief_2].second.items.begin() + item_2);

            // Printa os itens que foram trocados
            if(verbose)
                std::cout << "Trocou o item " << item_1 << " do ladrao " << thief_1 << " com o item " << item_2 << " do ladrao " << thief_2 << std::endl;

        }

        // Escolhe aleatoriamente um item de cada ladrao e troca com os outros
        void swap_items_btw_all_thieves(bool verbose = false)
        {
            // TODO
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

        //Guloso: valor/distancia   
        void greedySolution()
        {
            this->cleanSolution();

            std::vector<int> better_item(this->thieves.size());
            std::vector<int> aux_taked_items(this->items.size(), 0);
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
                        if(aux_taked_items[j] == 0)
                        {
                            cur_val = this->items[j].value / this->cities_distance[current_city][this->items[j].city_idx];
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
                    aux_taked_items[best_index] = 1;

                    if(this->items[best_index].weight < this->max_capacity - this->used_capacity)
                    {
                        current_city = this->items[best_index].city_idx;

                        // Pega o item
                        this->taked_items[best_index] = 1;
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
            for(int i = 0; i < this->thieves.size(); i++)
            {
                std::cout << "[";
                for(int j = 1; j < thieves[i].second.route.size() - 1; j++)
                {
                    std::cout <<thieves[i].second.route[j]+1 << ",";
                }
                std::cout << thieves[i].second.route[thieves[i].second.route.size() - 1]+1 << "]\n[";
                for(int j = 0; j < thieves[i].second.items.size() - 1; j++)
                {   
                    std::cout << thieves[i].second.items[j]+1 << ",";
                }
                std::cout << thieves[i].second.items[thieves[i].second.items.size() - 1]+1 << "]\n";
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



