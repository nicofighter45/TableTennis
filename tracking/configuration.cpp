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
bool shouldBreak;
bool autoState;
Pos reloadFromCamera;
bool roiSetup;
Rect roi;

Scalar getScalarFromHSVColor(HSVColor color) {
	return Scalar(color.H, color.S, color.V);
}