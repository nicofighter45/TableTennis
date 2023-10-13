#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <Windows.h>
#include "configuration.hpp"
#include "tracking.hpp"
#include "prompt.hpp"
#include "analyse.hpp"
#include "area.hpp"
#include <chrono>

using namespace std;
using namespace cv;

void initTracking() {
	// initializer, set default values for all variables that are in configuration.cpp
	currentLoadedFrame = 0;
	actualWatchedFrame = 0;
	watchedOpacity = 25; // from 0 to 100
	watchedZoom = 1; // from 1 to 16
	shouldLoadFrames = false;
	shouldBreak = false;
	watchedPos = { 0, 0 };

	// default orange range, the best one we figured out for now
	lower_color = HSVColor{ 8, 70, 70};
	upper_color = HSVColor{ 25, 255, 255 };
}

void test2() {
	Vec3b colors[] = { Vec3b(255, 255, 255) , Vec3b(255, 0, 0) , Vec3b(0, 255, 0) ,
		Vec3b(0, 0, 255) , Vec3b(0, 255, 255), Vec3b(255, 255, 0), Vec3b(255, 0, 255), Vec3b(120, 120, 0) };
	for (Vec3b color : colors) {
		cout << "R: " << color[0] << "G: " << color[1] << "B: " << color[2] << " " << RGBtoHSV(color) << endl << endl;
	}
	cout << lower_color;
}

void test() {
	vector<Area*> areas;
	Pos center({ 10, 10 });
	for (int k = 0; k < 4; k++) {
		areas.push_back(new PairArea(k, center));
		areas.push_back(new UnpairArea(k, center));
	}
	Vec3b colors[] = { Vec3b(255, 255, 255) , Vec3b(255, 0, 0) , Vec3b(0, 255, 0) , 
		Vec3b(0, 0, 255) , Vec3b(0, 255, 255), Vec3b(255, 255, 0), Vec3b(255, 0, 255), Vec3b(120, 120, 0)};
	width = 20;
	height = 20;
	Mat mat(1100, 1100, CV_8UC3, Scalar(0, 0, 0));
	for (int x = center.x * 10; x < center.x * 10 + 10; x++) {
		for (int y = center.y * 10; y < center.y * 10 + 10; y++) {
			mat.at<Vec3b>(x, y) = colors[1];
		}
	}
	imshow("mat", mat);
	waitKey(-1);
	
	for (int i = 0; i < 400; i++) {
		int j = 0;
		for (Area*area : areas) {
			Pos pos = area -> getNextPosition();
			if (pos.x + pos.y > 20) {
				area->nextRaw();
			}
			else if (pos != NULL_POS) {
				for (int x = pos.x * 10; x < pos.x * 10 + 10; x++) {
					for (int y = pos.y * 10; y < pos.y * 10 + 10; y++) {
						mat.at<Vec3b>(x, y) = colors[j];
					}
				}
			}
			cout << j << ") " <<  pos << " --> " << *area << endl;	
			j++;
			imshow("mat", mat);
			waitKey(100);
		}
		waitKey(-1);
	}
	waitKey(-1);
}

void setupTracking() {

	// the capture of the video (testFilePath is the path to a video that is small enough for the setup)
	/*
	cout << "OpenCV version: " << getVersionString() << endl;
	string filePath = getFile();
	if (filePath == "") {
		return;
	}
	cout << "FilePath: " << filePath << endl;
	VideoCapture capture(filePath);*/

	VideoCapture capture("C:\\Users\\fagot\\ShadowDrive\\tipe\\test1.MP4");

	// error if video can't be open
	if (!capture.isOpened()) {
		cerr << "Failed to open video" << endl;
		return;
	}

	// getting all video settings into proper variables
	fps = capture.get(CAP_PROP_FPS);
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);
	total_frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));

	// prepare the window to show frames
	initialisePrompts();

	Mat readed_frame;
	capture.read(readed_frame);
	// TODO let the user choose ROI
	cout << "Starting tracking" << endl;
	Mat currentMixedMatrice;
	while (true) {
		float conversion = static_cast<float>(watchedOpacity) / 100;
		auto start = chrono::high_resolution_clock::now();
		Analyser analyser(ref(readed_frame), regions_of_interest[0]);
		Pos center = analyser.findBall(readed_frame);
		currentMixedMatrice = analyser.getMixedMatrice(conversion);
		auto finish = std::chrono::high_resolution_clock::now();
		cout << "finish analyse center: " << center << " in " << chrono::duration_cast<chrono::milliseconds>(finish - start).count() << "ms" << endl;
		if (!showWindow(ref(currentMixedMatrice))) {
			break;
		}
		if (currentLoadedFrame == actualWatchedFrame) {
			continue;
		}
		if (currentLoadedFrame == actualWatchedFrame + 1) {
			capture.set(CAP_PROP_POS_FRAMES, actualWatchedFrame);
			currentLoadedFrame--;
		}
		else if (currentLoadedFrame == actualWatchedFrame - 1) {
			currentLoadedFrame++;
		}
		else {
			capture.set(CAP_PROP_POS_FRAMES, actualWatchedFrame);
			currentLoadedFrame = actualWatchedFrame;
		}
		if (!capture.read(readed_frame)) {
			break;
		}
	}

}

String getFile() {

	// https://learn.microsoft.com/fr-fr/windows/win32/dlgbox/using-common-dialog-boxes?redirectedfrom=MSDN#open_file

	wchar_t fileName[MAX_PATH] = { 0 };
	OPENFILENAMEW ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL; // The parent window handle (or NULL if you don't have one).
	ofn.lpstrFilter = L"MP4 Files (*.mp4)\0*.mp4\0"; // Filter for MP4 files only.
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST; // Flags for dialog behavior.

	if (GetOpenFileNameW(&ofn)) {
		// The user selected an MP4 file. Convert the wide character string to UTF-8 and return as a string.
		wstring wideFileName(fileName);
		return string(wideFileName.begin(), wideFileName.end());
	}
	// The user canceled the dialog or an error occurred.
		// You can handle this case accordingly (e.g., return an empty string).
	return "";
}