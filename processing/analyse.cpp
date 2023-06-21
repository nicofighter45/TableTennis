#include <opencv2/opencv.hpp>
#include "analyse.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Analyser::Analyser(Mat& tActualMatrice, Rect tNormalSearchRegion, Rect tInitialSearchRegion)
    : actualMatrice(tActualMatrice), normalSearchRegion(tNormalSearchRegion), initialSearchRegion(tInitialSearchRegion) {
    isInitialSearch = true;
    actualIndex = 0;
    calculateCenter();
    previous = center;
    /*
    // convert to hsv image format
	cvtColor(initialMatrice, HSVMatrice, COLOR_BGR2HSV);
	// this matrice is a mask that is blank if pixel are in the range, black otherwise
	inRange(HSVMatrice, getScalarFromHSVColor(lower_color), getScalarFromHSVColor(upper_color), maskMatrice);
	// we fusion original image and mask to make only pixel in range appear
	bitwise_and(initialMatrice, initialMatrice, resultMatrice, maskMatrice);
	// calculate the center of the detected pixels
	Moments m = moments(maskMatrice, true);
	center =  Pos{ m.m10 / m.m00, m.m01 / m.m00 };
    */
}

Pos Analyser::getRoughSearchPos() {
    int straightDecal = static_cast<int>((1 + actualIndex / 8) * searchPixelSpacing);
    int diagonalDecal = static_cast<int>(straightDecal * 0.7);
    Pos position = getSearchPos(straightDecal, diagonalDecal);
    return position;
}

Pos Analyser::getPreciseSearchPos() {
    Pos position = getSearchPos(1, 1);
    return position;
}

Pos Analyser::getSearchPos(int straightDecal, int diagonalDecal) {
    Pos position = getPreSearchPos(straightDecal, diagonalDecal);
    if (position.x > width) {
        if (position.y > height) {
            if (previous.y == position.y) {
                return Pos{ -1, -1 };
            }
        }
        if (previous.x == position.x) {
            return Pos{ -1, -1 };
        }
    }
    else if (position.y > height) {
        if (previous.y == position.y) {
            return Pos{ -1, -1 };
        }
    }
    actualIndex++;
    return position;
}

Pos Analyser::getPreSearchPos(int straightDecal, int diagonalDecal) {
    switch (actualIndex % 8) {
    case 0:
        return Pos{ center.x + straightDecal, center.y };
    case 1:
        return Pos{ center.x + diagonalDecal, center.y + diagonalDecal };
    case 2:
        return Pos{ center.x, center.y + straightDecal };
    case 3:
        return Pos{ center.x - diagonalDecal, center.y + diagonalDecal };
    case 4:
        return Pos{ center.x - straightDecal, center.y };
    case 5:
        return Pos{ center.x - diagonalDecal, center.y - diagonalDecal };
    case 6:
        return Pos{ center.x, center.y - straightDecal };
    case 7:
        return Pos{ center.x + diagonalDecal, center.y - diagonalDecal };
    }
}

void Analyser::newMatrice(Mat& tActualMatrice) {
    actualMatrice = tActualMatrice;
    calculateCenter();
}

void Analyser::calculateCenter() {
    if (isInitialSearch) {
        ROI = initialSearchRegion;
    }
    else {
        ROI = normalSearchRegion;
    }
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

Mat& Analyser::getActualMatrice() const {
	return actualMatrice;
}

Mat& Analyser::getMaskMatrice() {
	return ref(maskMatrice);
}

Mat& Analyser::getResultMatrice() {
	return ref(resultMatrice);
}

bool pixelIsInHSVRange(Mat& matrice, int i, int j) {
    HSVColor& color = RGBtoHSV(ref(matrice.at<Vec3b>(i, j)));
    if (color.H > upper_color.H) {
        return false;
    }
    if (color.H < lower_color.H) {
        return false;
    }
    if (color.S > upper_color.S) {
        return false;
    }
    if (color.S > upper_color.S) {
        return false;
    }
    if (color.V > upper_color.V) {
        return false;
    }
    if (color.V > upper_color.V) {
        return false;
    }
    return true;
}

HSVColor& RGBtoHSV(Vec3b& vector) {
    HSVColor color = { 0, 0, 0 };
    double r_normalized = vector[0] / 255.0;
    double g_normalized = vector[1] / 255.0;
    double b_normalized = vector[2] / 255.0;

    double cmax = std::max({ r_normalized, g_normalized, b_normalized });
    double cmin = std::min({ r_normalized, g_normalized, b_normalized });
    double delta = cmax - cmin;

    color.V = static_cast<int>(cmax * 255);

    if (delta != 0) {
        if (cmax == r_normalized) {
            color.H = static_cast<int>(60 * ((g_normalized - b_normalized) / delta) + 360) % 360;
        }
        else if (cmax == g_normalized) {
            color.H = static_cast<int>(60 * ((b_normalized - r_normalized) / delta) + 120);
        }
        else if (cmax == b_normalized) {
            color.H = static_cast<int>(60 * ((r_normalized - g_normalized) / delta) + 240);
        }
    }

    if (cmax != 0) {
        color.S = static_cast<int>((delta / cmax) * 255);
    }

    return ref(color);
}
