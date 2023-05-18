#include "prompt.hpp"
#include "configuration.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;

void initialisePrompts() {
	namedWindow(windowName, WINDOW_NORMAL);
	namedWindow(configurationWindowName, WINDOW_NORMAL);
	resizeWindow(windowName, Size(width * windowScalar, height * windowScalar));
	createTrackbar("Opacity", 100, watchedOpacity);
	createTrackbar("Frame", total_frames - 1, actualWatchedFrame);
	createTrackbar("H-low", 360, lower_color.H);
	createTrackbar("H-high", 360, upper_color.H);
	createTrackbar("S-low", 255, lower_color.S);
	createTrackbar("S-high", 255, upper_color.S);
	createTrackbar("V-low", 255, lower_color.V);
	createTrackbar("V-high", 255, upper_color.V);
}

void createTrackbar(string name, int max_value, int& variable) {
	createTrackbar(name, configurationWindowName, NULL, max_value, [](int value, void* userdata) {
		int& variable = *static_cast<int*>(userdata);
		variable = value;
		shouldBreak = true;
		}, &variable);
	setTrackbarPos(name, configurationWindowName, variable);
}

bool showWindow() {
	Size imageSize(width * imageScalar * windowScalar, height * imageScalar * windowScalar);
	Size windowSize(width * windowScalar, height * windowScalar);
	Point imagePosition(0, 0);

	int mouseData[2] = { 0, 0 };
	setMouseCallback(windowName, mouseCallback, mouseData);

	thread keyboardThread;

	while (not shouldBreak) {

		Mat matrice = matForIniti(Rect(watchedPos.x, watchedPos.y, width / watchedZoom, height / watchedZoom));

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
			if (actualWatchedFrame < total_frames - 1) {
				actualWatchedFrame += 1;
				setTrackbarPos("Frame", configurationWindowName, actualWatchedFrame);
				break;
			}
		}
		else if (key == 2490368) {  //up_arrow_key
			if (watchedOpacity < 75) {
				watchedOpacity += 25;
			}
			else {
				watchedOpacity = 100;
			}
			setTrackbarPos("Opacity", configurationWindowName, watchedOpacity);
			break;
		}
		else if (key == 2621440) {  //down_arrow_key
			if (watchedOpacity > 25) {
				watchedOpacity -= 25;

			}
			else {
				watchedOpacity = 0;
			}
			setTrackbarPos("Opacity", configurationWindowName, watchedOpacity);
			break;
		}
		else if (key == 27) { //espace
			shouldBreak = false;
			return false;
		}
		else if (key == 'i') {
			cout << "Give your values, here are actual ones" << endl
				<< lower_color.H << " " << lower_color.S << " " << lower_color.V << " "
				<< upper_color.H << " " << upper_color.S << " " << upper_color.V << endl;
			string input;
			getline(cin, input);
			vector<string> inputs;
			string current("");
			for (unsigned int i = 0; i < input.length(); i++)
			{
				char c = input[i];
				if (c == ' ')
				{
					inputs.push_back(current);
					current = "";
				}
				else
				{
					current += c;
				}
			}
			inputs.push_back(current);
			lower_color = HSVColor{ stoi(inputs[0]), stoi(inputs[1]), stoi(inputs[2]) };
			upper_color = HSVColor{ stoi(inputs[3]), stoi(inputs[4]), stoi(inputs[5]) };
			cout << "Give your values, here are actual ones" << endl
				<< lower_color.H << " " << lower_color.S << " " << lower_color.V << " "
				<< upper_color.H << " " << upper_color.S << " " << upper_color.V << endl;
			break;
		}

	}
	cout << "processing ..." << endl;
	shouldBreak = false;
	return true;
}



void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONUP) {
		*(int*)userdata = 0;
		*((int*)userdata + 1) = 0;
	}
	else if (event == EVENT_LBUTTONDBLCLK) {
		if (watchedZoom < 16) {
			watchedZoom *= 2;
			setNewWatchedPos(x / 2, y / 2);
		}
	}
	else if (event == EVENT_RBUTTONDOWN) {
		if (watchedZoom > 1) {
			watchedZoom = 1;
			watchedPos = Pos{ 0, 0 };
		}
	}
	else if (flags == EVENT_FLAG_LBUTTON && event == EVENT_MOUSEMOVE) {
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
		setNewWatchedPos(watchedPos.x + dx, watchedPos.y + dy);
	}
}


void setNewWatchedPos(int x, int y) {
	if (x > 0 and x + width / watchedZoom < width) {
		watchedPos.x = x;
	}
	else {
		watchedPos.x = width - width / watchedZoom;
	}
	if (y > 0 and y + height / watchedZoom < height) {
		watchedPos.y = y;
	}
	else {
		watchedPos.y = height - height / watchedZoom;
	}
}

void printCircle(Mat& mat, Pos center) {
	circle(mat, Point(static_cast<int>(center.x), static_cast<int>(center.y)), 50, Scalar(0, 0, 255), 4, LINE_AA, false);
}

void printRectangle(Mat& mat, Pos top_left_corner, Size size) {
	printRectangle(mat, Rect(static_cast<int>(top_left_corner.x), static_cast<int>(top_left_corner.y), size.width, size.height));
}

void printRectangle(Mat& mat, Rect region) {
	rectangle(mat, region, Scalar(0, 0, 0), 4, LINE_AA);
}