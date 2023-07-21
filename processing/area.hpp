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
	virtual Pos getPreNextPosition();
	virtual Pos getNextPosition();
public:
	Area(int tAreaIndex, Pos tCenterPosition);
};

class PairArea : public Area{
private:
	Pos getPreNextPosition();
public:
	Pos getNextPosition();
};
class UnpairArea : public Area{
private:
	Pos getPreNextPosition();
public:
	Pos getNextPosition();
	UnpairArea(int tAreaIndex, Pos tCenterPosition);
};


#endif