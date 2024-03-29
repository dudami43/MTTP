#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"
#include "functions.h"

using namespace std::chrono;

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
            std::cout << filename << std::endl;
            Instance instance;
            try
            {
                instance = get_instance(filename);
                for(int i = 1; i <= 5; i++)
                {
                    instance.thieves.clear();
                    instance.addThief(i);
                    std::cout << "ladrao " << i << ": ";
                    std::cout << "GRASP ( ";
                    high_resolution_clock::time_point t_i_grasp = high_resolution_clock::now();
                    std::cout << std::fixed << std::setprecision(2) << "Valor: " << grasp(instance, 20, 10, 0.7, false, 1);;
                    std::cout << " Tempo: " << duration_cast<duration<double>>(high_resolution_clock::now() - t_i_grasp).count() << " sec )";
                    std::cout << " -- ";
                    std::cout << "VNS ( ";
                    high_resolution_clock::time_point t_i_vns = high_resolution_clock::now();
                    std::cout << std::fixed << std::setprecision(2) << "Valor: " << VNS(instance, 7, 9);
                    std::cout << " Tempo: " << duration_cast<duration<double>>(high_resolution_clock::now() - t_i_vns).count() << " sec )" << std::endl;
                    
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