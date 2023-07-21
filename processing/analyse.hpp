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
	void newMatrice(Mat& tActualMatrice);
	void calculateCenter(Pos position);
	void initialCalculation();
	void findBall();
public:
	Analyser(Mat& tActualMatrice, 
		Rect tROI);
};

bool pixelIsInHSVRange(Mat& matrice, int i, int j);

HSVColor& RGBtoHSV(Vec3b& vector);

#endif