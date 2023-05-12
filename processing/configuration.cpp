#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "configuration.hpp"

using namespace cv;
using namespace std;

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
int current_frame_number;
atomic<bool> shouldLoadFrames;