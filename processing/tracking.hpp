#ifndef TACKING_HPP
#define TRACKING_HPP

#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include "configuration.hpp"

using namespace cv;
using namespace std;

void multithreading();
void loadFrames(VideoCapture& capture, const int video_number);
void processVideo(const string& filename, const int video_number);
void processFrame(const int i, const int j);
Frame getFrame(const int i, const int j);
void cutter();
void processVideoSingleThreaded(VideoCapture capture, String name);
void printCircleCenter(Mat& mat, const int x, const int y);
void singlethreading();


void initTracking();
void setupTracking();
void trackSinglethreading();
void tackMultithreading();
void analyseResults();


#endif