#include <bits/stdc++.h>
#include "classes.cpp"
#include "parser.h"

// Para compilar com g++11: g++ -std=c++11 main.cpp parser.cpp functions.cpp

int main(int argc, char *argv[])
{
    Instance instance(4, 6, 3, 0.1, 1.0, 1.0);

    City city_1(0, 0);
    City city_2(0, 0);
    City city_3(0, 0);
    City city_4(0, 0);

    instance.cities.push_back(city_1);
    instance.cities.push_back(city_2);
    instance.cities.push_back(city_3);
    instance.cities.push_back(city_4);
    instance.cities_distance[0][1] = instance.cities_distance[1][0] = 5;
    instance.cities_distance[0][2] = instance.cities_distance[2][0] = 6;
    instance.cities_distance[0][3] = instance.cities_distance[3][0] = 6;
    instance.cities_distance[1][2] = instance.cities_distance[2][1] = 5;
    instance.cities_distance[1][3] = instance.cities_distance[3][1] = 6;
    instance.cities_distance[3][2] = instance.cities_distance[2][3] = 4;

    Item item_21(2, 20, 2);
    instance.cities[1].items_idx.push_back(0);
    instance.items.push_back(item_21);

    Item item_22(3, 30, 2);
    instance.cities[1].items_idx.push_back(1);
    instance.items.push_back(item_22);

    Item item_31(3, 100, 3);
    instance.cities[2].items_idx.push_back(2);
    instance.items.push_back(item_31);

    Item item_32(1, 40, 3);
    instance.cities[2].items_idx.push_back(3);
    instance.items.push_back(item_32);

    Item item_33(1, 40, 3);
    instance.cities[3].items_idx.push_back(4);
    instance.items.push_back(item_33);

    Item item_41(2, 20, 4);
    instance.cities[3].items_idx.push_back(5);
    instance.items.push_back(item_41);
    
    instance.addThief(1);

    instance.thieves[0].second.route = {0, 1, 3, 2, 0};
    instance.thieves[0].second.backpack_weight = {0, 0, 0, 2, 0};
    instance.thieves[0].second.items = {3, 4};
    instance.taked_items = {0, 0, 0, 1, 1, 0};
    instance.thieves[0].first = 0.4;

    // instance.printRoutes();
    
    float evaluate = instance.evaluateRoutes();
    double maxZ = instance.maxZ();
    std::cout << evaluate << " =? " << maxZ << std::endl;

    /*try
    {
        std::cout << filename << "\n";
        instance = get_instance(filename);
        instance.initialRoutes();
        //instance.printRoutes();
        float evaluate = instance.evaluateRoutes();
        double maxZ = instance.maxZ();
        std::cout << evaluate << " =? " << maxZ << std::endl;
    }
    catch (char const *param)
    {
        std::cout << param << std::endl;
        return 0;
    }*/

    return 0;
}