#ifndef AREA_HPP
#define AREA_HPP

#include "configuration.hpp"

using namespace cv;
using namespace std;

class Area {
protected:
	const int areaIndex;
	int lineIndex;
	int startIndex;
	int positionIndex;
	const Pos centerPosition;
	int difference;
public:

	Area(int tAreaIndex, Pos tCenterPosition);

	virtual Pos getNextPosition();

	virtual Pos getPreNextPosition();

};

class PairArea : public Area{
public:
	Pos getNextPosition();
};
class UnpairArea : public Area{
public:
	Pos getNextPosition();
};


#endif