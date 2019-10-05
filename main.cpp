#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"

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
                std::cout << filename << "\n";
                instance = get_instance(filename);
                instance.addThief(1);
                
                std::cout << std::endl;
                std::cout << "rand: \n";
                instance.initialRoutes();
                float evaluate = instance.evaluateRoutes();
                double objectiveFunction = instance.objectiveFunction();
                std::cout << evaluate << " =? " << objectiveFunction << std::endl;
                std::cout << std::endl;

                std::cout << std::endl;
                std::cout << "greedy: \n";
                std::cout << "Capacidade maxima da mochila: " << instance.max_capacity << std::endl;
                instance.greedySolution();
                evaluate = instance.evaluateRoutes();
                objectiveFunction = instance.objectiveFunction();
                std::cout << evaluate << " =? " << objectiveFunction << std::endl;
                std::cout << std::endl;

                //instance.swap_cities(true);
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