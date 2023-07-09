#include "area.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Area::Area(int tAreaIndex, Pos tCenterPosition) :
	areaIndex(tAreaIndex), centerPosition(tCenterPosition) {
	lineIndex = 1;
	startIndex = 0;
	positionIndex = 0;
	difference = 0;
}

Pos PairArea::getNextPosition() {
	if (positionIndex + difference >= lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	Pos position = getPreNextPosition();
	if (position.x > width) {
		if (areaIndex == 0) {
			// stop
		}
		else if (areaIndex == 6) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.x < 0) {
		if (areaIndex == 2) {
			difference++;
			return getNextPosition();
		}
		else if (areaIndex == 4) {
			// stop
		}
		// issue
	}
	if (position.y > height) {
		if (areaIndex == 4) {
			// stop
		}
		else if (areaIndex == 6) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
}

Pos PairArea::getPreNextPosition() {
	switch (areaIndex) {
	case 0:
		return Pos{ centerPosition.x + lineIndex, centerPosition.y + positionIndex };
	case 2:
		return Pos{ centerPosition.x - positionIndex, centerPosition.y + lineIndex };;
	case 4:
		return Pos{ centerPosition.x - lineIndex, centerPosition.y - positionIndex };;
	case 6:
		return Pos{ centerPosition.x + positionIndex, centerPosition.y - lineIndex };;
	}
	return ;
}