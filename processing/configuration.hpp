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
	int x;
	int y;
};
typedef struct Frame Frame;
struct Frame {
	int number;
	Mat matrice;
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
const Scalar lower_color(0, 100, 100);
const Scalar upper_color(20, 255, 255);
const int number_frames_to_read_ahead = 100;
const string path = "C:\\Users\\fagot\\Videos\\tipe\\*.MP4";
const int number_of_threads = thread::hardware_concurrency();

extern int total_frames;
extern int fps;
extern int width;
extern int height;

extern vector<vector<Frame>> frames;
extern vector<vector<Pos>> positionsResults;
extern vector<Pos> orderedPositions;
extern vector<string> filenames;
extern mutex mtx;
extern condition_variable cvariable;
extern int current_frame_number;
extern atomic<bool> shouldLoadFrames;

#endif