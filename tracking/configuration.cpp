#include <opencv2/opencv.hpp>
#include <fstream>

#include "configuration.hpp"
#include "../tools/abstract_structs.hpp"

using namespace cv;
using namespace std;

HSVColor lower_color;
HSVColor upper_color;
int total_frames;
int fps;
int width;
int height;
int actualWatchedFrame;
int currentLoadedFrame;
int watchedOpacity;
float watchedZoom;
Pos watchedPos;
bool isPromptActive;
bool autoState;
Pos reloadFromCamera;
bool roiSetup;
bool indication;
Rect roi;
bool shouldCalculate;

Scalar getScalarFromHSVColor(HSVColor color) {
	return Scalar(color.H, color.S, color.V);
}

int sq(int value) {
	return value * value;
}

double distance(Pos pos1, Pos pos2) {
	return sqrt(sq(pos1.x-pos2.x) + sq(pos1.y-pos2.y));
}
