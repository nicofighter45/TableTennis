#ifndef ANALYSE_HPP
#define ANALYSE_HPP

#include <opencv2/opencv.hpp>
#include "configuration.hpp"

using namespace cv;
using namespace std;

class Analyser {
private:
	Rect ROI;
	Pos center;
	Mat& actualMatrice;
	Mat maskMatrice;
	Mat resultMatrice;
	bool isInitialSearch;
	int actualIndex;
	Pos previous;
	Pos getSearchPos(int straightDecal, int diagonalDecal);
	Pos getPreSearchPos(int straightDecal, int diagonalDecal);
	Pos getSearchPos();
	Pos calculateCenter(Pos position);
	Pos initialCalculation();
public:
	Pos findBall(Mat& tActualMatrice);
	Mat& getMixedMatrice(float conversion);
	Mat& getMaskMatrice();
	Mat& getResultMatrice();
	Analyser(Mat& tActualMatrice, Rect tROI);
};

bool pixelIsInHSVRange(Mat& matrice, Pos position);
HSVColor RGBtoHSV(Vec3b& vector);

#endif