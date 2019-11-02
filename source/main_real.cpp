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
                instance = get_instance(filename);
                std::cout << filename << std::endl;
                instance.thieves.clear();
                instance.addThief(2);
                std::cout << VNS(instance, 5, 10) <<  std::endl;
                std::cout << instance.objectiveFunction(true) << std::endl;
                instance.output(); 
            }
            catch (char const *param)
            {
                std::cout << param << std::endl;
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