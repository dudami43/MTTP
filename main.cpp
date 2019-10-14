#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"
#include "functions.h"

// Para compilar com g++11: g++ -std=c++11 main.cpp parser.cpp functions.cpp

int main(int argc, char *argv[])
{
    // Inicializa a funcao randomica
    srand (time(NULL));

    char *file_name = new char[500];
    strcpy(file_name, "files/");
    if(argc == 1)
        strcat(file_name, "pvanet_files");
    else
        strcat(file_name, argv[1]);

    // Abre o arquivo indicado
    std::ifstream file;
    std::string filename;
    file.open(file_name);

    // Delete referencia para o nome do arquivo
    delete file_name;

    if (file.is_open())
    {
        while (getline(file, filename))
        {
            Instance instance;
            try
            {
                // std::cout << filename << "\n";
                instance = get_instance(filename);
                instance.addThief(5);
                instance.greedySolution();
                instance.output();
                // instance.printRoutes();
                // instance.printItems();
                // instance.printWeights();
                
                //instance.swap_items_btw_thieves(true);
                //instance.printItems();
                //std::cout << instance.used_capacity << " " << instance.max_capacity << std::endl;
                double objectiveFunction = instance.objectiveFunction();
                std::cout << objectiveFunction << std::endl;

                break;
            }
            catch (char const *param)
            {
                std::cout << param << std::endl;
                return 0;
            }
        }
        file.close();
        
    }
    else
    {
        throw "Unable to open the file";
    }
    return 0;
}