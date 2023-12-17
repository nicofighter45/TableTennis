#include "prompt.hpp"
#include "configuration.hpp"
#include "analyse.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

void initialisePrompts() {
	isPromptActive = true;
	namedWindow(windowName, WINDOW_NORMAL);
	namedWindow(configurationWindowName, WINDOW_NORMAL);
	resizeWindow(windowName, Size(width * windowScalar, height * windowScalar));
	resizeWindow(configurationWindowName, Size(400, 300));
	createTrackbar("Opacity", 100, watchedOpacity);
	createTrackbar("Frame", total_frames - 1, actualWatchedFrame);
	createTrackbar("H-low", 360, lower_color.H);
	createTrackbar("H-high", 360, upper_color.H);
	createTrackbar("S-low", 255, lower_color.S);
	createTrackbar("S-high", 255, upper_color.S);
	createTrackbar("V-low", 255, lower_color.V);
	createTrackbar("V-high", 255, upper_color.V);
}

void shutDownPrompt() {
	destroyWindow(windowName);
	destroyWindow(configurationWindowName);
}

void createTrackbar(string name, int max_value, int& variable) {
	createTrackbar(name, configurationWindowName, NULL, max_value, [](int value, void* userdata) {
		int& variable = *static_cast<int*>(userdata);
		variable = value;
		isPromptActive = false;
	}, &variable);
	setTrackbarPos(name, configurationWindowName, variable);
}

void chooseROI(Mat readed_frame) {
	Size imageSize(width * imageScalar * windowScalar, height * imageScalar * windowScalar);
	Size windowSize(width * windowScalar, height * windowScalar);

	int mouseData[2] = { 0, 0 };
	isPromptActive = true;

	setMouseCallback(windowName, roiMouseCallback, mouseData);

	while (isPromptActive) {

		Mat temp_frame = readed_frame.clone();

		printRectangle(temp_frame, roi);

		resize(temp_frame, temp_frame, imageSize);

		Mat window_image(windowSize, CV_8UC3, Scalar(0, 0, 0));
		Rect roi({ 0, 0 }, imageSize);
		Mat roi_image = window_image(roi);
		resize(temp_frame, roi_image, imageSize);

		imshow(windowName, window_image);

		int key = waitKeyEx(10);
		if (key == 32) { //espace
			actualWatchedFrame += 1;
			autoState = true;
			roiSetup = false;
			setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
			break;
		}
	}
}

void roiMouseCallback(int event, int x, int y, int flags, void* userdata) {
	int window_width = getWindowImageRect(windowName).width; //not working
	int window_height = getWindowImageRect(windowName).height;
	if (event == EVENT_LBUTTONUP) {		
		*(int*)userdata = 0;
		*((int*)userdata + 1) = 0;
	}
	else if (flags == EVENT_FLAG_LBUTTON) {
		if (*(int*)userdata == 0) {
			*(int*)userdata = x;
		}
		if (*((int*)userdata + 1) == 0) {
			*((int*)userdata + 1) = y;
		}
		if (x < 0) {
			x = 0;
		}
		if (y < 0) {
			y = 0;
		}

		int roi_width = (x - *(int*)userdata) * width / window_width;
		int roi_height = (y - *((int*)userdata + 1)) * height / window_height;
		int x = *(int*)userdata * width / window_width;
		int y = *((int*)userdata + 1) * height / window_height;
		if (roi_width < 0) {
			roi_width *= -1;
			x -= roi_width;
		}
		if (roi_height < 0) {
			roi_height *= -1;
			y -= roi_height;
		}
	
		roi = Rect(x, y, roi_width, roi_height);

	}
	else if (event == EVENT_MOUSEWHEEL) {
		int delta = static_cast<int>(getMouseWheelDelta(flags) / 120);
		actualWatchedFrame += delta;
		if (actualWatchedFrame < 0) {
			actualWatchedFrame = 0;
		}
		else if (actualWatchedFrame >= total_frames) {
			actualWatchedFrame = total_frames - 1;
		}
		setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
		isPromptActive = false;
		shouldCalculate = false;
	}
}

void showWindow(Pos center, Mat originalMatrice, int ms) {

	Size imageSize(width * imageScalar * windowScalar, height * imageScalar * windowScalar);
	Size windowSize(width * windowScalar, height * windowScalar);
	Point imagePosition(0, 0);

	int mouseData[2] = { 0, 0 };
	isPromptActive = true;

	setMouseCallback(windowName, mouseCallback, mouseData);

	if (center != NULL_POS) {
		if (autoState) {
			watchedZoom = 4;
			if (center.x + width / (2 * watchedZoom) >= width) {
				watchedPos.x = width - width / watchedZoom - 1;
			}
			else if (center.x >= width / (2 * watchedZoom)) {
				watchedPos.x = center.x - width / (2 * watchedZoom);
			}
			else {
				watchedPos.x = 0;
			}
			if (center.y + height / (2 * watchedZoom) >= height) {
				watchedPos.y = height - height / watchedZoom - 1;
			}
			else if (center.y >= height / (2 * watchedZoom)) {
				watchedPos.y = center.y - height / (2 * watchedZoom);
			}
			else {
				watchedPos.y = 0;
			}
		}

	}
	else {
		watchedPos.x = 0;
		watchedPos.y = 0;
		watchedZoom = 1;
	}

	while (isPromptActive) {

		Mat matrice = originalMatrice(Rect(watchedPos.x, watchedPos.y, width / watchedZoom, height / watchedZoom));

		resize(matrice, matrice, imageSize);

		Mat window_image(windowSize, CV_8UC3, Scalar(0, 0, 0));
		Rect roi(imagePosition, imageSize);
		Mat roi_image = window_image(roi);
		resize(matrice, roi_image, imageSize);

		imshow(windowName, window_image);

		int key = waitKeyEx(10);
		if (key == 2424832) {  //left_arrow_key
			if (actualWatchedFrame > 0) {
				actualWatchedFrame -= 1;
				setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
				break;
			}
		}
		else if (key == 2555904) {  //right_arrow_key
			actualWatchedFrame += 1;
			setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
			break;
		}
		else if (key == 2621440) {  //down_arrow_key
			if (watchedOpacity < 75) {
				watchedOpacity += 25;
			}
			else {
				watchedOpacity = 100;
			}
			setTrackbarPos("Opacity", configurationWindowName, watchedOpacity);
			break;
		}
		else if (key == 2490368) {  //up_arrow_key
			if (watchedOpacity > 25) {
				watchedOpacity -= 25;
			}
			else {
				watchedOpacity = 0;
			}
			setTrackbarPos("Opacity", configurationWindowName, watchedOpacity);
			break;
		}
		else if (key == 32) { //espace
			autoState = not autoState;
			shouldCalculate = true;
			break;
		}
		else if (key == 99){ // c
			reloadFromCamera = inverse(center);
			break;
		}
		else if (key == 114){ // r
			break;
		}
		else if (key == 115) { // s
			roiSetup = true;
			autoState = false;
			break;
		}
		if (autoState) {
			actualWatchedFrame += 1;
			setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
			break;
		}			
	}
	
}

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	int window_width = getWindowImageRect(windowName).width;
	int window_height = getWindowImageRect(windowName).height;
	if (event == EVENT_LBUTTONUP) {
		*(int*)userdata = 0;
		*((int*)userdata + 1) = 0;
	}
	else if (event == EVENT_LBUTTONDBLCLK) {
		if (watchedZoom < 16) {
			watchedZoom *= 2;
			setNewWatchedPos(x*(1-1/watchedZoom), y*(1 - 1 / watchedZoom), window_width, window_height);
		}
	}
	else if (event == EVENT_RBUTTONDOWN) {
		if (watchedZoom > 1) {
			watchedZoom /= 2;
			setNewWatchedPos(x*(1-1/watchedZoom), y*(1-1/watchedZoom), window_width, window_height);
		}
	}
	else if (flags == EVENT_FLAG_LBUTTON && event == EVENT_MOUSEMOVE) {
		watchedPos.x = static_cast<int>(watchedPos.x * window_width / width);
		watchedPos.y = static_cast<int>(watchedPos.y * window_height / height);
		if (*(int*)userdata == 0) {
			*(int*)userdata = x;
		}
		if (*((int*)userdata + 1) == 0) {
			*((int*)userdata + 1) = y;
		}
		int dx = x - *(int*)userdata;
		int dy = y - *((int*)userdata + 1);
		*(int*)userdata = x;
		*((int*)userdata + 1) = y;
		setNewWatchedPos(watchedPos.x + dx, watchedPos.y + dy, window_width, window_height);
	}
	else if (event == EVENT_MOUSEWHEEL) {
		int delta = static_cast<int>(getMouseWheelDelta(flags)/120);
		actualWatchedFrame += delta;
		if (actualWatchedFrame < 0) {
			actualWatchedFrame = 0;
		}
		else if (actualWatchedFrame >= total_frames) {
			actualWatchedFrame = total_frames - 1;
		}
		setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
		isPromptActive = false;
		autoState = false;
		shouldCalculate = false;
	}
}

void setNewWatchedPos(int x, int y, int window_width, int window_height) {
	if (x < 0) {
		watchedPos.x = 0;
	}
	else if (x + window_width / watchedZoom >= window_width) {
		watchedPos.x = window_width - window_width / watchedZoom;
	}
	else {
		watchedPos.x = x;
	}
	if (y < 0) {
		watchedPos.y = 0;
	}
	else if (y + window_height / watchedZoom >= window_height) {
		watchedPos.y = window_height - window_height / watchedZoom;
	}
	else {
		watchedPos.y = y;
	}
	watchedPos.x = static_cast<int>(watchedPos.x * width / window_width);
	watchedPos.y = static_cast<int>(watchedPos.y * height / window_height);
}

void printCircle(Mat& mat, Pos center) {
	circle(mat, Point(static_cast<int>(center.x), static_cast<int>(center.y)), 50, white, 4, LINE_AA, false);
}

void printRectangle(Mat& mat, Pos top_left_corner, Size size) {
	printRectangle(mat, Rect(static_cast<int>(top_left_corner.x), static_cast<int>(top_left_corner.y), size.width, size.height));
}

void printRectangle(Mat& mat, Rect region) {
	rectangle(mat, region, black, 4, LINE_AA);
}