#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../simulations/friction.h"
#include "../simulations/magnus.h"
#include "../simulations/gravity.h"
#include "../simulations/RK4.h"
#include "loader.hpp"
#include "../tools/transfer.hpp"
#include "../tracking/tracking.hpp"

using namespace std;

vector<string> paragraphs;

void openFile()
{
    ifstream textFile(textPath);
    if (!textFile) cerr << errorMsg << endl;
    string line;
    string currentParagraph("");
    int space = 0;
    while (getline(textFile, line))
    {
        if (space == 2)
        {
            space = 0;
            paragraphs.push_back(currentParagraph);
            currentParagraph = "";
        }
        else if (line == "")
        {
            space++;
            if (space == 2)
            {
                continue;
            }
        }
        currentParagraph += "\n" + line;
    }
    paragraphs.push_back(currentParagraph);
    textFile.close();
}

string replace(string str, string from, string to)
{
    size_t start_pos = str.find(from);
    if (start_pos == string::npos)
        cerr << errorMsg << endl;
    str.replace(start_pos, from.length(), to);
    return str;
}

void run()
{
    openFile();
    cout << paragraphs[0] << endl;
    string simulationType;
    getline(cin, simulationType);
    cout << replace(paragraphs[1], "{type}", simulationType) << endl;

    if (simulationType == "0") {
        initTracking();
        setupTracking();
        return;
    }

    string iniP, iniS;

    cout << paragraphs[2] << endl;
    getline(cin, iniP);

    cout << paragraphs[3] << endl;
    getline(cin, iniS);

    cout << endl << paragraphs[4] << endl;

    Vect3D initialPosition = iniP;
    Vect3D initialSpeed = iniS;

    if (simulationType == "1")
    {

        auto vectors = runGravitySimulation(initialPosition, initialSpeed);

    }
    else if (simulationType == "2")
    {

        auto vectors = runFrictionSimulation(initialPosition, initialSpeed);

    }
    else if (simulationType == "3")
    {

        auto vectors = runMagnusSimulation(initialPosition, initialSpeed);
        allVectorsToTxt(get<0>(vectors), get<1>(vectors), get<2>(vectors));

    }
    else if (simulationType == "4") {

        auto vectors = runRK4simulation(initialPosition, initialSpeed);
        allVectorsToTxt(get<0>(vectors), get<1>(vectors), get<2>(vectors));

    }
    else
    {

        cerr << errorMsg << endl;
        
    }

}