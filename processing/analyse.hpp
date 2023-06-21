#ifndef ANALYSE_HPP
#define ANALYSE_HPP

#include <opencv2/opencv.hpp>
#include "configuration.hpp"

using namespace cv;
using namespace std;

class Analyser {
private:
	Rect ROI;
	Rect normalSearchRegion;
	Rect initialSearchRegion;
	Pos center;
	Mat& actualMatrice;
	Mat maskMatrice;
	Mat resultMatrice;
	bool isInitialSearch;
	int actualIndex;
	Pos previous;
	Pos getSearchPos(int straightDecal, int diagonalDecal);
	Pos getPreSearchPos(int straightDecal, int diagonalDecal);

public:
	Analyser(Mat& tActualMatrice, 
		Rect tNormalSearchRegion, 
		Rect tinitialSearchRegion);
	Pos getPreciseSearchPos();
	Pos getRoughSearchPos();
	void newMatrice(Mat& tActualMatrice);
	void calculateCenter();
	Pos getCenterPixels() const;
	double getCenterX() const;
	double getCenterY() const;
	Mat& getActualMatrice() const;
	Mat& getMaskMatrice();
	Mat& getResultMatrice();
};

bool pixelIsInHSVRange(Mat& matrice, int i, int j);

HSVColor& RGBtoHSV(Vec3b& vector);

#endif