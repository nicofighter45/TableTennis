#include <opencv2/opencv.hpp>
#include "analyse.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Analyser::Analyser(Mat& tInitialMatrice) : initialMatrice(tInitialMatrice) {
	// convert to hsv image format
	cvtColor(initialMatrice, HSVMatrice, COLOR_BGR2HSV);
	// this matrice is a mask that is blank if pixel are in the range, black otherwise
	inRange(HSVMatrice, getScalarFromHSVColor(lower_color), getScalarFromHSVColor(upper_color), maskMatrice);
	// we fusion original image and mask to make only pixel in range appear
	bitwise_and(initialMatrice, initialMatrice, resultMatrice, maskMatrice);
	// calculate the center of the detected pixels
	Moments m = moments(maskMatrice, true);
	center =  Pos{ m.m10 / m.m00, m.m01 / m.m00 };
}

Pos Analyser::getCenterPixels() const {
	return center;
}

double Analyser::getCenterX() const {
	return center.x;
}

double Analyser::getCenterY() const {
	return center.y;
}

Mat& Analyser::getInitialMatrice() const {
	return initialMatrice;
}

Mat& Analyser::getHSVMatrice() {
	return ref(HSVMatrice);
}

Mat& Analyser::getMaskMatrice() {
	return ref(maskMatrice);
}

Mat& Analyser::getResultMatrice() {
	return ref(resultMatrice);
}
