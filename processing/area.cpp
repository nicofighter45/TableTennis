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

UnpairArea::UnpairArea(int tAreadIndex, Pos tCenterPosition) {
	Area::Area(tAreadIndex, tCenterPosition);
	startIndex = 1;
}

Pos PairArea::getNextPosition() {
	if (positionIndex - difference >= lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	Pos position = Area::getNextPosition();
	if (position.x == -1) {
		return Pos{ -1, -1 };
	}
	if (position.x > width) {
		if (areaIndex == 3) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.x < 0) {
		if (areaIndex == 1) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.y > height) {
		if (areaIndex == 2) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.y < 0) {
		if (areaIndex == 0) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
}
Pos UnpairArea::getNextPosition() {
	if (positionIndex - difference > lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	Pos position = Area::getNextPosition();
	if (position.x == -1) {
		return Pos{ -1, -1 };
	}
	if (position.x > width) {
		if (areaIndex == 1) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.x < 0) {
		if (areaIndex == 3) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.y > height) {
		if (areaIndex == 0) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
	else if (position.y < 0) {
		if (areaIndex == 2) {
			difference++;
			return getNextPosition();
		}
		// issue
	}
}

Pos Area::getNextPosition() {
	Pos position = getPreNextPosition();
	switch(areaIndex) {
	case 0:
		if (position.x > width) {
			return Pos{ -1, -1 };
		}
	case 1:
		if (position.y < 0) {
			return Pos{ -1, -1 };
		}
	case 2:
		if (position.x < 0) {
			return Pos{ -1, -1 };
		}
	case 3:
		if (position.y > height) {
			return Pos{ -1, -1 };
		}
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
	return ;
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
	return ;
}

