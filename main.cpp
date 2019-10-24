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
                std::cout << filename << "\n";
                instance = get_instance(filename);
                instance.thieves.clear();
                instance.addThief(2);

                //instance.greedySolution();
                //std::cout << "Guloso " << instance.objectiveFunction() << std::endl;

                //double result = VNS(instance, 10);
                //std::cout << "VNS + Greedy " << result << std::endl;

                //instance.cleanSolution();
                geneticAlgorithm(instance);
                //instance.greedySolution();
                //instance.output();
                //instance.printRoutes();
                //instance.printItems();
                //instance.printWeights();
                //instance.output();

                //double best = objectiveFunction;
                //result = VNS(instance, 10);
                //double result = localSearch(instance, "trade_btw_thieves");

                //std::cout << "Initial Value: " << instance.objectiveFunction();
                //std::cout << "VNS + GA " << result << std::endl;
            
            }
            catch (char const *param)
            {
                std::cout << param << std::endl;
                //return 0;
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