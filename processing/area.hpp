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
	virtual Pos getPreNextPosition() = 0;
public:
	Area();
	Area(int tAreaIndex, Pos tCenterPosition);
	virtual Pos getNextPosition() = 0;
};

class PairArea : public Area{
private:
	Pos getPreNextPosition();
public:
	Pos getNextPosition();
	PairArea(int tAreaIndex, Pos tCenterPosition);
};
class UnpairArea : public Area{
private:
	Pos getPreNextPosition();
public:
	Pos getNextPosition();
	UnpairArea(int tAreaIndex, Pos tCenterPosition);
};


#endif