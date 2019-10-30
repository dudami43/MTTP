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
        std::cout << "instancia, numero de ladroes, funcao objetivo, tamanho rlc \n";
        while (getline(file, filename))
        {
            Instance instance;
            try
            {
                instance = get_instance(filename);
                for(int i = 1; i <= 5 ; i++){
                
                    //teste grasp
                    for(double k = 0.1; k < 1.0; k+=0.1)
                    {
                        for(int j = 0; j < 10; j++)
                        {
                            instance.thieves.clear();
                            instance.addThief(i);
                            std::cout << filename << "," << i << "," << grasp(instance, 20, k, false) << "," << k <<  std::endl;
                        }
                    }                
                }
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