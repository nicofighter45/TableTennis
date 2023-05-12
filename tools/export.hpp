#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include "vector.hpp"
#include "constants.hpp"
#include <cstdlib>

using namespace std;;

void print2DGraph(vector<Vect3D> positions)
{

    ofstream file;
    file.open("../visualisation/transfer_value.txt");
    if(file.is_open()){
        for(int time = 0; time < positions.size(); time++){
            file << time/spacing_per_second << ";" << positions[time].getX() << ";" << positions[time].getY() << ";" <<
            positions[time].getZ() << endl;
        }
        file.close();
    }

    system("py ../visualisation/2dgraph.py");

}