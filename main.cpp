#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string const error("An error occured, we're deeply sorry :(");

int main()
{
    ifstream textFile("data/en.txt");
    if (!textFile)
        cerr << error << endl;
    string line;
    while (getline(textFile, line))
    {
        cout << line << endl;
    }
    textFile.close();
    return 0;
}