#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser_antiga.h"
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
    std::vector<int> melhoria(6, 0);
    int total = 0;
    if (file.is_open())
    {
        while (getline(file, filename))
        {
            std::cout << filename << std::endl;
            Instance instance;
            try
            {
                instance = get_instance(filename);
                //std::cout << filename << std::endl;
                for(int i = 1; i <= 5; i++)
                {
                    instance.thieves.clear();
                    instance.addThief(i);
                    std::cout << "ladrao " << i << ": ";
                    grasp(instance, 20, 2, 0.1, true, 1);
                    double greedy1 = instance.objectiveFunction();
                    grasp(instance, 20, 2, 0.1, true, 2);
                    double greedy2 = instance.objectiveFunction();
                    if(greedy2 > greedy1)
                        melhoria[i] ++;
                    std::cout << greedy2 - greedy1 << std::endl;
                    //std::cout << grasp(instance) << std::endl;
                    //instance.output(); 
                }
                total ++;
            }
            catch (char const *param)
            {
                std::cout << param << std::endl;
            }
        }
        file.close();
        
        for(int i = 1; i <= 5; i++)
        {   
            double a = melhoria[i]/total;
            std::cout << melhoria[i] << " " << total << " " << a << std::endl;
        }
    }
    else
    {
        throw "Unable to open the file";
    }
    return 0;
}