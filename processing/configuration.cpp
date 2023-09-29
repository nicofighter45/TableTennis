#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "configuration.hpp"

using namespace cv;
using namespace std;

HSVColor lower_color;
HSVColor upper_color;
int total_frames;
int fps;
int width;
int height;
Pos initPos;
int actualWatchedFrame;
int watchedOpacity;
float watchedZoom;
Pos watchedPos;
bool shouldBreak;

vector<vector<Frame>> frames;
vector<vector<Pos>> positionsResults;
vector<Pos> orderedPositions;
vector<string> filenames;
mutex mtx;
condition_variable cvariable;
int currentLoadedFrame;
atomic<bool> shouldLoadFrames;

Mat matForIniti;

Scalar getScalarFromHSVColor(HSVColor color) {
	return Scalar(color.H, color.S, color.V);
}