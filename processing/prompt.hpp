#ifndef PROMPT_HPP
#define PROMPT_HPP

#include <opencv2/opencv.hpp>
#include <fstream>
#include "configuration.hpp"

using namespace std;
using namespace cv;

void initialisePrompts();
bool showWindow();
void createTrackbar(string name, int max_value, int& variable);
void mouseCallback(int event, int x, int y, int flags, void* userdata);
void setNewWatchedPos(int x, int y);
void printCircle(Mat& mat, Pos center);
void printRectangle(Mat& mat, Pos top_left_corner, Size size);
void printRectangle(Mat& mat, Rect region);

#endif