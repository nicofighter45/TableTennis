#include "area.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Area::Area() : areaIndex(0), lineIndex(1), startIndex(-1),
positionIndex(0), centerPosition(NULL_POS), difference(0) {
}

Area::Area(int tAreaIndex, Pos tCenterPosition) : areaIndex(tAreaIndex), lineIndex(1), startIndex(-1),
positionIndex(0), centerPosition(tCenterPosition), difference(0) {
}

UnpairArea::UnpairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
	startIndex = 0;
}

PairArea::PairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
}

Pos PairArea::getNextPosition() {
	if (positionIndex - difference > lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	positionIndex++;
	Pos position = getPreNextPosition();
	if (position.x > width) {
		if (areaIndex == 3) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 1) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y > height) {
		if (areaIndex == 2) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 0) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	return position;
}
Pos UnpairArea::getNextPosition() {
	if (positionIndex - difference > lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	Pos position = getPreNextPosition();
	if (position.x > width) {
		if (areaIndex == 1) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 3) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y > height) {
		if (areaIndex == 0) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 2) {
			difference++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	return position;
}

Pos PairArea::getPreNextPosition() {
	switch (areaIndex) {
	case 0:
		return Pos{ centerPosition.x + lineIndex, centerPosition.y + positionIndex };
	case 1:
		return Pos{ centerPosition.x - positionIndex, centerPosition.y + lineIndex };;
	case 2:
		return Pos{ centerPosition.x - lineIndex, centerPosition.y - positionIndex };;
	case 3:
		return Pos{ centerPosition.x + positionIndex, centerPosition.y - lineIndex };;
	}
	cerr << "PairArea index problem" << endl;
	return NULL_POS;
}

Pos UnpairArea::getPreNextPosition() {
	switch (areaIndex) {
	case 0:
		return Pos{ centerPosition.x + lineIndex, centerPosition.y - positionIndex };
	case 1:
		return Pos{ centerPosition.x + positionIndex, centerPosition.y + lineIndex };;
	case 2:
		return Pos{ centerPosition.x - lineIndex, centerPosition.y + positionIndex };;
	case 3:
		return Pos{ centerPosition.x - positionIndex, centerPosition.y - lineIndex };;
	}
	cerr << "UnpairArea index problem" << areaIndex << endl;
	return NULL_POS;
}

