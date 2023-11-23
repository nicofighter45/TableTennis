#ifndef TACKING_HPP
#define TRACKING_HPP

#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include "configuration.hpp"

using namespace cv;
using namespace std;

String getFile();

void initTracking();
void setupTracking();
void launchTracking(VideoCapture capture);
void saveTracking(map < int, Pos > positionsResults);


#endif