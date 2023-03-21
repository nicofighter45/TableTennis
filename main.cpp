#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string const error("An error occured, we're deeply sorry :(");
string const textPath("C:/Users/fagot/OneDrive/Documents/MPSI/TIPE/data/en.txt");
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
            space == 0;
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
    int simulationType;
    cin >> simulationType;
    cout << replace(paragraphs[1], "{type}", to_string(simulationType)) << endl;
    return 0;
}