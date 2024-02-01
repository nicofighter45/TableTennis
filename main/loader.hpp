#ifndef LOADER_HPP
#define LOADER_HPP
#include <string>
#include <vector>


std::string const errorMsg("An error occured, we're deeply sorry :(");
std::string const textPath("D:/TableTennis/data/en.txt");
extern std::vector<std::string> paragraphs;
void openFile();
std::string replace(std::string str, std::string from, std::string to);
void run();

#endif