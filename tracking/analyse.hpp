#ifndef ANALYSE_HPP
#define ANALYSE_HPP

#include <opencv2/opencv.hpp>
#include "configuration.hpp"

using namespace cv;
using namespace std;

class Analyser {
private:
	Pos center;
	Mat& actualMatrice;
	Mat maskMatrice;
	bool isInitialSearch;
	int actualIndex;
	int actualMinusIndex;
	Pos previous;
	Pos getPreSearchPos(Pos prePos);
	Pos getSearchPos(Pos prePos);
	Pos calculateCenter(Pos position);
	Pos initialCalculation();
public:
	void setIsInitialSearch(bool state);
	Pos findBall();
	Mat& getMixedMatrice(float conversion);
	Mat& getMaskMatrice();
	Mat& getMatriceWithCenter();
	Analyser(Mat& tActualMatrice);
};

bool pixelIsInHSVRange(Mat& matrice, Pos position);
Vec3b reducePixelStrength(const Vec3b& originalPixel, float conversion);
HSVColor RGBtoHSV(Vec3b& vector);
void addCubeToImage(Mat& matrice, Pos position, int size, Vec3b color);

#endif