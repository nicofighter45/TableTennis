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
	int stopIndex;
	int positionIndex;
	const Pos centerPosition;
public:
	ostream& print(ostream& os) const;
	virtual Pos getPreNextPosition() = 0;
	Area();
	Area(int tAreaIndex, Pos tCenterPosition);
	virtual Pos getNextPosition() = 0;
	void nextRaw();
};

class PairArea : public Area{
private:
public:
	Pos getPreNextPosition();
	Pos getNextPosition();
	PairArea(int tAreaIndex, Pos tCenterPosition);
};
class UnpairArea : public Area{
private:
public:
	Pos getPreNextPosition();
	Pos getNextPosition();
	UnpairArea(int tAreaIndex, Pos tCenterPosition);
};

ostream& operator<<(std::ostream& os, const Area& obj);

#endif