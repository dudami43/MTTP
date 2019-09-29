#include <bits/stdc++.h>
#include "functions.h"
#include "parser.h"
//g++ -std=c++11 main.cpp parser.cpp functions.cpp
int main(int argc, char *argv[])
{
    srand (time(NULL));

    char *file_name = new char[500];
    strcpy(file_name, "files/");
    if(argc == 1)
        strcat(file_name, "pvanet_files");
    else
        strcat(file_name, argv[1]);

    std::ifstream file;
    std::string filename;
    file.open(file_name);

    delete file_name;

    if (file.is_open())
    {
        while (getline(file, filename))
        {
            std::vector<std::vector<int>> adj_matrix, adj_list;
            
            try
            {
                std::cout << filename << "\n";
                //adj_matrix = get_adj_matrix(filename);
                //adj_list = get_adj_list(adj_matrix);

               
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