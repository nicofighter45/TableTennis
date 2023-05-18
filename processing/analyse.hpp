#ifndef ANALYSE_HPP
#define ANALYSE_HPP

#include <opencv2/opencv.hpp>
#include "configuration.hpp"

using namespace cv;
using namespace std;

class Analyser {
private:
	Pos center;
	Mat& initialMatrice;
	Mat HSVMatrice;
	Mat maskMatrice;
	Mat resultMatrice;
public:

	Analyser(Mat& tInitialMatrice);

	Pos getCenterPixels() const;

	double getCenterX() const;
	
	double getCenterY() const;

	Mat& getInitialMatrice() const;

	Mat& getHSVMatrice();

	Mat& getMaskMatrice();

	Mat& getResultMatrice();

};

#endif