#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "configuration.hpp"

using namespace cv;
using namespace std;

Scalar lower_color;
Scalar upper_color;
int total_frames;
int fps;
int width;
int height;
int actualWatchedFrame;
float watchedOpacity;
float watchedZoom;
Pos watchedPos;

vector<vector<Frame>> frames;
vector<vector<Pos>> positionsResults;
vector<Pos> orderedPositions;
vector<string> filenames;
mutex mtx;
condition_variable cvariable;
int currentLoadedFrame;
atomic<bool> shouldLoadFrames;

Mat matForIniti;