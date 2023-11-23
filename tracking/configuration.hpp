#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <opencv2/opencv.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>

using namespace cv;
using namespace std;

typedef struct Pos {
	double x;
	double y;
} Pos;
const Pos NULL_POS = { -1, -1 };
inline Pos inverse(const Pos& pos) {
	return Pos{ pos.y, pos.x};
}
inline bool operator==(const Pos& pos1, const Pos& pos2) {
	return pos1.x == pos2.x && pos1.y == pos2.y;
}
inline bool operator!=(const Pos& pos1, const Pos& pos2) {
	return !(pos1 == pos2);
}
inline ostream& operator<<(ostream& os, const Pos& pos) {
	os << "x: " << pos.x << "  y: " << pos.y;
	return os;
}
inline Pos operator+(const Pos& pos1, const Pos& pos2) {
	return Pos{ pos1.x + pos2.x, pos1.y + pos2.y };
}

inline Pos operator+=(const Pos& pos1, const int add) {
	return Pos{ pos1.x + add, pos1.y + add };
}
inline Pos operator-(const Pos& pos1, const Pos& pos2) {
	return Pos{ pos1.x - pos2.x, pos1.y - pos2.y };
}

inline Pos operator-=(const Pos& pos1, const int add) {
	return Pos{ pos1.x - add, pos1.y - add };
}
inline bool operator<=(const Pos& pos1, const Pos& pos2) {
	return (pos1.x <= pos2.x) && (pos1.y <= pos2.y);
}
inline bool operator>=(const Pos& pos1, const Pos& pos2) {
	return (pos1.x >= pos2.x) && (pos1.y >= pos2.y);
}


typedef struct Frame {
	int number;
	Mat matrice;
};
typedef struct HSVColor {
	int H;
	int S;
	int V;
};
inline ostream& operator<<(ostream& os, const HSVColor& color) {
	os << "h: " << color.H << "  s: " << color.S << "  v: " << color.V;
	return os;
}

const int number_frames_to_read_ahead = 100;
const string path = "C:\\Users\\fagot\\ShadowDrive\\tipe\\*.MP4";
const int number_of_threads = thread::hardware_concurrency();
const String windowName = "Window";
const String configurationWindowName = "Configuration";
const String testFilePath = "C:\\Users\\fagot\\ShadowDrive\\tipe\\test1.MP4";
const Rect testFileROI(0, 0, 1920, 1080 / 2);
const float windowScalar = 0.5;
const float imageScalar = 1;
const int searchPixelSpacing = 15;
const int searchPixelMaxSpacing = 200;
const int maxSearchAreaSize = 100;

extern HSVColor lower_color;
extern HSVColor upper_color;
extern int total_frames;
extern int fps;
extern int width;
extern int height;
extern Pos initPos;
extern int actualWatchedFrame;
extern int watchedOpacity;
extern float watchedZoom;
extern Pos watchedPos;
extern bool shouldBreak;
extern bool autoState;
extern Pos reloadFromCamera;
extern bool roiSetup;
extern Rect roi;

extern vector<vector<Frame>> frames;
extern vector<vector<Pos>> positionsResults;
extern vector<Pos> orderedPositions;
extern vector<string> filenames;
extern mutex mtx;
extern condition_variable cvariable;
extern int currentLoadedFrame;
extern atomic<bool> shouldLoadFrames;

Scalar getScalarFromHSVColor(HSVColor color);

#endif