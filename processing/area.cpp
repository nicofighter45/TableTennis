#include "area.hpp"
#include "configuration.hpp"

using namespace cv;
using namespace std;

Area::Area() : areaIndex(0), lineIndex(0), startIndex(0), stopIndex(-1),
positionIndex(0), centerPosition(NULL_POS), morePixel(20) {
}

Area::Area(int tAreaIndex, Pos tCenterPosition) : areaIndex(tAreaIndex), lineIndex(0), startIndex(0),
positionIndex(0), centerPosition(tCenterPosition), stopIndex(-1), morePixel(20) {
}

UnpairArea::UnpairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
	startIndex = 1;
}

PairArea::PairArea(int tAreadIndex, Pos tCenterPosition) : Area(tAreadIndex, tCenterPosition) {
}

Pos PairArea::getNextPosition() {
	if (lineIndex >= maxSearchAreaSize) {
		return NULL_POS;
	}
	if ((stopIndex != -1 && (stopIndex >= lineIndex || startIndex == stopIndex)) || startIndex > lineIndex) {
		if (morePixel == 0) {
			return NULL_POS;
		}
		morePixel -= 1;
	}
	if ((stopIndex != -1 && positionIndex >= stopIndex) || positionIndex >= lineIndex -1) {
		lineIndex++;
		positionIndex = startIndex;
	}
	else {
		positionIndex++;
	}
	Pos position = getPreNextPosition();
	if (position.x >= height) {
		if (areaIndex == 2) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 0) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y >= width) {
		if (areaIndex == 3) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 1) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	return position;
}
Pos UnpairArea::getNextPosition() {
	if (lineIndex >= maxSearchAreaSize) {
		return NULL_POS;
	}
	if ((stopIndex != -1 && (stopIndex >= lineIndex + 1 || startIndex == stopIndex)) || startIndex > lineIndex + 1) {
		if (morePixel == 0) {
			return NULL_POS;
		}
		morePixel -= 1;
	}
	if ((stopIndex != -1 && positionIndex >= stopIndex) || positionIndex >= lineIndex) {
		lineIndex++;
		positionIndex = startIndex;
	}
	else {
		positionIndex++;
	}
	Pos position = getPreNextPosition();
	if (position.x >= height) {
		if (areaIndex == 3) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.x < 0) {
		if (areaIndex == 1) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y >= width) {
		if (areaIndex == 0) {
			stopIndex = positionIndex;
			return getNextPosition();
		}
		return NULL_POS;
	}
	else if (position.y < 0) {
		if (areaIndex == 2) {
			stopIndex = positionIndex;
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