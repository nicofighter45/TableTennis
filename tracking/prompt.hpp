#ifndef PROMPT_HPP
#define PROMPT_HPP

#include <opencv2/opencv.hpp>
#include <fstream>
#include "configuration.hpp"

using namespace std;
using namespace cv;

void initialisePrompts();
void shutDownPrompt();
void showWindow(Pos center, Mat originalMatrice, int ms);
void createTrackbar(string name, int max_value, int& variable);
void mouseCallback(int event, int x, int y, int flags, void* userdata);
void setNewWatchedPos(int x, int y, int window_width, int window_height);
void printCircle(Mat& mat, Pos center);
void printRectangle(Mat& mat, Pos top_left_corner, Size size);
void printRectangle(Mat& mat, Rect region);
void chooseROI(Mat readed_frame);
void roiMouseCallback(int event, int x, int y, int flags, void* userdata);

#endif