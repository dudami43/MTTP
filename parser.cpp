#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"

Instance get_instance(std::string filename)
{
    // Abre o arquivo com os dados da instancia
    std::ifstream file;

    std::string name = "data/" + filename;
    if(*name.rbegin() == '\n')
        file.open(name.substr(0,name.length() - 1));
    else
        file.open(name);

    // Cria uma classe instancia com os dados adquiridos
    Instance instance; 

    if(file.is_open())
    {
        std::string line, aux;

        // Dados do problema
        int num_cities, num_items, max_capacity;
        double min_speed, max_speed, renting_ratio;
        std::string weight_type;
        
        // Le o cabecalho
        for(int i=0; i < 9; i++)
        {
            getline(file, line);
            if(i == 2) // Adquire o numero de vertices
            {
                std::stringstream ss(line);
                ss >> aux >> num_cities;
            }
            else if(i == 3) // Adquire o numero de itens
            {
                std::stringstream ss(line);
                ss >> aux >> aux >> aux >> num_items;
            }
            else if(i == 4) // Adquire a capacidade da mochila
            {
                std::stringstream ss(line);
                ss >> aux >> aux >> aux >> max_capacity;
            }
            else if(i == 5) // Adquire a velocidade minima
            {
                std::stringstream ss(line);
                ss >> aux >> aux >> min_speed;
            }
            else if(i == 6) // Adquire a velocidade maxima
            {
                std::stringstream ss(line);
                ss >> aux >> aux >> max_speed;
            }
            else if(i == 7) // Adquire a taxa de pagamento da mochila
            {
                std::stringstream ss(line);
                ss >> aux >> aux >> renting_ratio;
            }
            else if(i == 8) // Adquire o modelo de peso
            {
                std::stringstream ss(line);
                ss >> aux >> weight_type;
            }
        }

        // Seta os dados adquiridos
        instance.setValues(num_cities, num_items, max_capacity, min_speed, max_speed, renting_ratio);
        
        /* std::cout << std::endl;
        std::cout << "Dados da instancia: " << std::endl;
        std::cout << "-----Tipo de peso: " << weight_type << std::endl;
        std::cout << "-----Numero de cidades: " << instance.num_cities << std::endl;
        std::cout << "-----Numero de itens: " << instance.num_items << std::endl; 
        std::cout << "-----Capacidade maxima: " << instance.max_capacity << std::endl;
        std::cout << "-----Velocidade minima: " << instance.min_speed << std::endl; 
        std::cout << "-----Velocidade maxima: " << instance.max_speed << std::endl;
        std::cout << "-----Taxa da mochila: " << instance.renting_ratio << std::endl;
        std::cout << std::endl; */

        // Le o cabecalho dos vertices
        getline(file, line);
        // Le as vertices
        for(int i = 0; i < num_cities; i++){
            int index;
            double x, y;
            getline(file, line);
            
            // Adquire a posicao da cidade i
            std::stringstream ss(line);
            ss >> index >> x >> y;

            // Adiciona a cidade a lista de cidades
            City city_i(x, y);
            instance.cities.push_back(city_i);
        
            // Calcula a distancia da cidade i para todas que ja foram add
            for(int j = i-1; j >= 0; j--){
                instance.cities_distance[i][j] = sqrt( pow(instance.cities[i].x - instance.cities[j].x, 2) + pow(instance.cities[i].y - instance.cities[j].y, 2) );
                instance.cities_distance[j][i] = instance.cities_distance[i][j];
            }
        }

        /* std::cout << "Matriz de distancias: " << std::endl;
        for(auto x: instance.cities_distance){
            std::cout << "<<< ";
            for(auto y: x){
                std::cout << y << ", ";
            }std::cout << ">>>" << std::endl;
        }std::cout << std::endl; */

        // Le o cabecalho dos itens
        getline(file, line);
        // Le os itens
        for(int i = 0; i < num_items; i++){
            int index, profit, weight, city_index;
            getline(file, line);
            
            // Adquire os atributos do item
            std::stringstream ss(line);
            ss >> index >> profit >> weight >> city_index;

            // Cria o item
            Item item_i(weight, profit, city_index);
            
            // Adiciona o item a cidade em que esta localizado
            instance.cities[city_index-1].items_idx.push_back(i);

            // Adiciona o item no vetor de itens da instancia
            instance.items.push_back(item_i);
        }

        /*(for(auto x: instance.cities){
            std::cout << std::endl;
            std::cout << "Coordenada: (" << x.x << ", " << x.y << ")" << std::endl;
            std::cout << "Itens: \n";
            for(auto y: x.items_idx){
                std::cout << "Item " << y << " => ";
                std::cout << instance.items[y].value << " ";
                std::cout << instance.items[y].weight << std::endl;
            }
        }std::cout << std::endl;*/

    }else
    {
        throw "Unable to open the file";
    }
   
    return instance;
}