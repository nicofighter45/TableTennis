#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tools/vector.cpp"
#include "tools/constants.cpp"
#include "simulations/friction.cpp"
#include "simulations/magnus.cpp"

using namespace std;

string const error("An error occured, we're deeply sorry :(");
string const textPath("data/en.txt");
vector<string> paragraphs;

void openFile()
{
    ifstream textFile(textPath);
    if (!textFile)
        cerr << error << endl;
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
        cerr << error << endl;
    str.replace(start_pos, from.length(), to);
    return str;
}

int main()
{
    openFile();
    cout << paragraphs[0] << endl;
    string simulationType;
    getline(cin, simulationType);
    cout << replace(paragraphs[1], "{type}", simulationType) << endl;
    if(simulationType == "1"){

    }else if(simulationType == "2"){
        runFrictionSimulation();
    }else if(simulationType == "3"){
        runMagnusSimulation();
    }else{
        cerr << error << endl;
    }
    return 0;
}