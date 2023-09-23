#include "area.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Area::Area() : areaIndex(0), lineIndex(0), startIndex(0), stopIndex(0),
positionIndex(0), centerPosition(NULL_POS) {
}

Area::Area(int tAreaIndex, Pos tCenterPosition) : areaIndex(tAreaIndex), lineIndex(0), startIndex(0),
positionIndex(0), centerPosition(tCenterPosition), stopIndex(-1) {
}

UnpairArea::UnpairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
	startIndex = 1;
}

PairArea::PairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
}

Pos PairArea::getNextPosition() {
	if ((stopIndex != 0 && stopIndex >= lineIndex) || startIndex > lineIndex) {
		return NULL_POS;
	}
	if ((stopIndex != 0 && positionIndex >= stopIndex) || positionIndex > lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	else {
		positionIndex++;
	}
	Pos position = getPreNextPosition();
	if (position.x > width) {
		if (areaIndex == 2) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 0) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y > height) {
		if (areaIndex == 3) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 1) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	return position;
}
Pos UnpairArea::getNextPosition() {
	if ((stopIndex != 0 && stopIndex >= lineIndex + 1) || startIndex > lineIndex + 1) {
		return NULL_POS;
	}
	if ((stopIndex != 0 && positionIndex >= stopIndex) || positionIndex > lineIndex + 1) {
		lineIndex++;
		positionIndex = startIndex;
	}
	else {
		positionIndex++;
	}
	Pos position = getPreNextPosition();
	if (position.x > width) {
		if (areaIndex == 3) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 1) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y > height) {
		if (areaIndex == 0) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 2) {
			stopIndex++;
			return getNextPosition();
		}
		return NULL_POS;
	}
	return position;
}

Pos PairArea::getPreNextPosition() {
	switch (areaIndex) {
	case 0:
		return Pos{ centerPosition.x - positionIndex, centerPosition.y + lineIndex };;
	case 1:
		return Pos{ centerPosition.x - lineIndex, centerPosition.y - positionIndex };;
	case 2:
		return Pos{ centerPosition.x + positionIndex, centerPosition.y - lineIndex };;
	case 3:
		return Pos{ centerPosition.x + lineIndex, centerPosition.y + positionIndex };
	}
	cerr << "PairArea index problem: " << *this << endl;
	return NULL_POS;
}

Pos UnpairArea::getPreNextPosition() {
	switch (areaIndex) {
	case 0:
		return Pos{ centerPosition.x - lineIndex, centerPosition.y + positionIndex };;
	case 1:
		return Pos{ centerPosition.x - positionIndex, centerPosition.y - lineIndex };;
	case 2:
		return Pos{ centerPosition.x + lineIndex, centerPosition.y - positionIndex };
	case 3:
		return Pos{ centerPosition.x + positionIndex, centerPosition.y + lineIndex };;
	}
	cerr << "UnpairArea index problem: " << *this << endl;
	return NULL_POS;
}

void Area::nextRaw() {
	if (positionIndex == startIndex) {
		startIndex++;
	}
	else {
		stopIndex = positionIndex;
	}
}

ostream& Area::print(ostream& os) const {
	os << "Index: " << areaIndex << "  LineIndex: " << lineIndex <<
		"  StartIndex: " << startIndex << "  StopIndex: " << stopIndex <<
		"  PositionIndex: " << positionIndex <<
		"  Center: " << centerPosition;
	return os;
}

ostream& operator<<(std::ostream& os, const Area& obj) {
	return obj.print(os);
}