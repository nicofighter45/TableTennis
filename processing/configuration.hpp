#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>

using namespace cv;
using namespace std;

typedef struct Pos Pos;
struct Pos {
	double x;
	double y;
};
typedef struct Frame Frame;
struct Frame {
	int number;
	Mat matrice;
};
typedef struct HSVColor HSVColor;
struct HSVColor {
	int H;
	int S;
	int V;
};

const Rect regions_of_interest[] = {
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2)
};
const int number_frames_to_read_ahead = 100;
const string path = "C:\\Users\\fagot\\Videos\\tipe\\*.MP4";
const int number_of_threads = thread::hardware_concurrency();
const String windowName = "Window";
const String configurationWindowName = "Configuration";
const String testFilePath = "C:\\Users\\fagot\\Videos\\tipe\\test1.MP4";
const Rect testFileROI(0, 0, 1920, 1080 / 2);
const float windowScalar = 0.7;
const float imageScalar = 1;

extern HSVColor lower_color;
extern HSVColor upper_color;
extern int total_frames;
extern int fps;
extern int width;
extern int height;
extern int actualWatchedFrame;
extern int watchedOpacity;
extern float watchedZoom;
extern Pos watchedPos;
extern bool shouldBreak;

extern vector<vector<Frame>> frames;
extern vector<vector<Pos>> positionsResults;
extern vector<Pos> orderedPositions;
extern vector<string> filenames;
extern mutex mtx;
extern condition_variable cvariable;
extern int currentLoadedFrame;
extern atomic<bool> shouldLoadFrames;

extern Mat matForIniti;

Scalar getScalarFromHSVColor(HSVColor color);

#endif