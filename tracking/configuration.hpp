#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include "../tools/abstract_structs.hpp"

using namespace cv;
using namespace std;

const int number_of_threads = thread::hardware_concurrency();
const String windowName = "Window";
const String configurationWindowName = "Configuration";
const float windowScalar = 0.5;
const float imageScalar = 1;
const int searchPixelSpacing = 15;
const int searchPixelMaxSpacing = 200;
const int maxSearchAreaSize = 30;
const int minimumPixelNeeded = 100;
const int morePixelStart = 30;
const int spacingBetweenCentersToStop = 20;

const Vec3b white = Vec3b(255, 255, 255);
const Vec3b black = Vec3b(0, 0, 0);
const Vec3b red = Vec3b(0, 0, 255);
const Vec3b colors[] = { Vec3b(0, 120, 120) , Vec3b(255, 0, 0) , Vec3b(0, 255, 0) ,
                    Vec3b(0, 0, 255) , Vec3b(0, 255, 255), Vec3b(255, 255, 0), Vec3b(255, 0, 255), Vec3b(120, 120, 0) };


extern HSVColor lower_color;
extern HSVColor upper_color;
extern int total_frames;
extern int fps;
extern int width;
extern int height;
extern int actualWatchedFrame;
extern int currentLoadedFrame;
extern int watchedOpacity;
extern float watchedZoom;
extern Pos watchedPos;
extern bool isPromptActive;
extern bool autoState;
extern Pos reloadFromCamera;
extern bool roiSetup;
extern Rect roi;
extern bool shouldCalculate;

Scalar getScalarFromHSVColor(HSVColor color);

int sq(int value);

double distance(Pos pos1, Pos pos2);

#endif