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
                for(int i = 1; i <= 5 ; i++){
                
                    instance.thieves.clear();
                    instance.addThief(i);

                    instance.greedySolution();

                    std::cout << i << " ladrao(oes) - ";

                    std::cout << "Solucao inicial: " << instance.objectiveFunction() << std::endl;
                    std::cout << geneticAlgorithm(instance);
                    std::cout << " - Funcao objetivo: " << instance.objectiveFunction() << std::endl;
                    //instance.output();

                    /**
                     * TODO:
                     * Fazer uma funcao que "recupera" solucoes com valor negativo
                     * (vai tirando itens por ordem de menor custo beneficio ate que 
                     * o valor da mesma se torne >= 0)
                     * 
                     * Refazer a vizinhanca de troca de itens entre ladroes
                     * (fazer com um numero aleatorio de ladroes ao inves de 2)
                     * 
                     **/
                    
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