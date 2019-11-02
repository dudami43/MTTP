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
    strcpy(file_name, "files/one");

    // Abre o arquivo indicado
    std::ifstream file;
    std::string filename;
    file.open(file_name);

    int max_disturbance=5; 
    double n_disturbe=10;

    if(argc == 3)
    {
        max_disturbance = atoi(argv[1]);
        n_disturbe = atof(argv[2]);
    }
    else
    {
        throw "Numero errado de parametros(2 necessarios)";
    }

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
                instance.thieves.clear();
                instance.addThief(2);
                std::cout << VNS(instance, max_disturbance, n_disturbe) <<  std::endl;
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