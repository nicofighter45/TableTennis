#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <mutex>

using namespace cv;
using namespace std;

extern const Rect regions_of_interest[] = {
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
extern const Scalar lower_color(0, 100, 100);
extern const Scalar upper_color(20, 255, 255);
extern const int number_frames_to_read_ahead = 100;
extern const string path = "C:\\Users\\fagot\\Videos\\tipe\\*.MP4";
const int number_of_threads = thread::hardware_concurrency();

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

int total_frames;
int fps;
int width;
int height;

vector<vector<Frame>> frames;
vector<vector<Pos>> positionsResults;
vector<Pos> orderedPositions;
vector<string> filenames;
mutex mtx;
condition_variable cvariable;
int current_frame_number(0);
atomic<bool> shouldLoadFrames(false);