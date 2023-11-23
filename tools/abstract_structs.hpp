#ifndef ABSTRACT_STRUCTS_HPP
#define ABSTRACT_STRUCTS_HPP

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

#endif