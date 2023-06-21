#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include "vector.hpp"
#include "constants.hpp"
#include <cstdlib>

using namespace std;;

void print2DGraph(vector<tuple<double, double, double>> values, string name)
{

    ofstream file;
    file.open("C:/Users/fagot/OneDrive/Documents/MPSI/TableTennis/visualisation/" + name + ".txt");
    if(file.is_open()){
        for(int time = 0; time < values.size(); time++){
            file << time/spacing_per_second << ";" << get<0>(values[time]) << ";" << get<1>(values[time]) << ";" <<
                get<2>(values[time]) << endl;
        }
        file.close();
    }

    // system("py ../visualisation/2dgraph.py");

}