#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <Windows.h>
#include "configuration.hpp"
#include "tracking.hpp"
#include "prompt.hpp"
#include "analyse.hpp"

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
	lower_color = HSVColor{ 0, 65, 65 };
	upper_color = HSVColor{ 20, 255, 255 };
}

void setupTracking() {
	// this method is used to find the most optimized range

	// the capture of the video (testFilePath is the path to a video that is small enough for the setup)
	cout << "OpenCV version: " << getVersionString() << endl;
	VideoCapture capture(getFile());

	// error if video can't be open
	if (!capture.isOpened()) {
		cerr << "Failed to open test video" << endl;
		return;
	}

	// getting all video settings into proper variables
	fps = capture.get(CAP_PROP_FPS);
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);
	total_frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));

	// prepare the window to show frames
	initialisePrompts();

	const float conversion = static_cast<float>(watchedOpacity) / 100;

	Mat readed_frame;
	capture.read(readed_frame);
	// TODO let the user choose ROI
	Analyser analyser(ref(readed_frame), regions_of_interest[0]);
	cout << "Starting tracking" << endl;
	do {
		Pos center = analyser.findBall(readed_frame);
		cout << "the center is " << center.x << ";" << center.y << endl;
		matForIniti = analyser.getMixedMatrice(conversion);
		imshow("bou", matForIniti);
	} while (capture.read(readed_frame) /* && showWindow() */ );

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
		std::wstring wideFileName(fileName);
		return std::string(wideFileName.begin(), wideFileName.end());
	}
	else {
		// The user canceled the dialog or an error occurred.
		// You can handle this case accordingly (e.g., return an empty string).
		return "";
	}
}