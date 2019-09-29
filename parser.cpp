#include <bits/stdc++.h>
#include "parser.h"

std::vector<int> get_number_from_string(std::string s, char type)
{
    std::string aux = "";
    std::vector<int> numbers;

    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '\n')
        {
            if (type == 'G')
            {
                numbers.push_back(std::stoi(aux));
            }
            continue;
        }
        else if (s[i] == ' ')
        {
            numbers.push_back(std::stoi(aux));
            aux = "";
        }
        else
        {
            aux += s[i];
        }
    }

    if (aux != "\n" && type == 'N')
    {
        numbers.push_back(std::stoi(aux));
    }

    return numbers;
}

std::vector<std::vector<int> > get_adj_matrix(std::string filename)
{
    std::ifstream file;
    std::string line;
    file.open(filename);

    std::vector<std::vector<int>> adj_matrix;
    char type;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (line[0] == 'G')
            {
                type = line[0];
                continue;
            }
            else if (line[0] == 'N')
            {
                type = 'N';
                continue;
            }
            if (type == 'G')
            {
                adj_matrix.push_back(get_number_from_string(line, type));
            }
            else
            {
                auto numbers = get_number_from_string(line, type);

                if (numbers.size() == 3)
                {
                    adj_matrix.assign(numbers[1], std::vector<int>(numbers[1], 0));
                }
                else
                {
                    adj_matrix[numbers[0] - 1][numbers[1] - 1] = 1;
                    adj_matrix[numbers[1] - 1][numbers[0] - 1] = 1;
                }
            }
        }
        file.close();
    }
    else
    {
        throw "Unable to open the file";
    }

    return adj_matrix;
}

std::vector<std::vector<int> > get_adj_list(std::vector<std::vector<int> > adj_matrix){
    std::vector<std::vector<int> > adj_list;
    adj_list.reserve(adj_matrix.size());

    std::vector<int> aux;
    aux.reserve(adj_matrix.size());
    for(int i=0; i<adj_matrix.size(); i++){
        for(int j=0; j<adj_matrix[i].size(); j++){
            if(adj_matrix[i][j] == 1 && i != j){
                aux.push_back(j);
            }
        }
        adj_list.push_back(aux);
        aux.clear();
    }
    
    return adj_list;
}
