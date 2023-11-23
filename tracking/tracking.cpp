#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <chrono>
#include <filesystem>

#include "configuration.hpp"
#include "tracking.hpp"
#include "prompt.hpp"
#include "analyse.hpp"
#include "area.hpp"

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
	autoState = true;
	reloadFromCamera = NULL_POS;
	roiSetup = true;

	// default orange range, the best one we figured out for now
	lower_color = HSVColor{ 5, 50, 50};
	upper_color = HSVColor{ 45, 250, 250 };
}


void setupTracking() {

	cout << "Library: OpenCV version: " << getVersionString() << endl;


	// the capture of the video
	string filePath = getFile();
	if (filePath == "") {
		return;
	}
	cout << "FilePath: " << filePath << endl;

	streambuf* stream_buffer_cout = std::cout.rdbuf();

	// Redirect cout to a file
	ofstream out("output.txt");
	cout.rdbuf(out.rdbuf());

	VideoCapture capture(filePath);

	// error if video can't be open
	if (!capture.isOpened()) {
		cout.rdbuf(stream_buffer_cout);
		cerr << "Failed to open video" << endl;
	}

	// Restore the original buffer for cout
	cout.rdbuf(stream_buffer_cout);

	// getting all video settings into proper variables
	fps = capture.get(CAP_PROP_FPS);
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);
	total_frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));
	roi = Rect{ 0, 0, width, height };

	cout << "FPS: " << fps << endl << "width: " << width << endl << 
		"height: " << height << endl << "total frames: " << total_frames << endl;

	// prepare the window to show frames
	cout.rdbuf(out.rdbuf());
	initialisePrompts();
	cout.rdbuf(stream_buffer_cout);

	Mat readed_frame;
	capture.read(readed_frame);
	chooseROI(readed_frame);

	cout << "ROI: " << roi << endl;

	bool shouldCalculate = true;
	float conversion = static_cast<float>(watchedOpacity) / 100;
	Analyser analyser(ref(readed_frame));
	Pos center = NULL_POS;
	map < int, Pos > positionsResults;

	while (true) {
		int ms = 0;
		if (shouldCalculate) {
			auto start = chrono::high_resolution_clock::now();
			reloadFromCamera = analyser.findBall();
			if (reloadFromCamera != NULL_POS) {
				center = analyser.findBall();
				reloadFromCamera = NULL_POS;
				auto finish = chrono::high_resolution_clock::now();
				ms = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
				cout << "Frame " << capture.get(CAP_PROP_POS_FRAMES) - 1 << " center " << center << " in "
					<< ms << "ms" << endl;
			}
			else {
				center = NULL_POS;
				auto finish = chrono::high_resolution_clock::now();
				ms = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
				cout << "Frame " << capture.get(CAP_PROP_POS_FRAMES) - 1 << " no center in " << ms << "ms" << endl;
			}
			positionsResults[currentLoadedFrame] = center;

		}

		if (center == NULL_POS) {
			showWindow(NULL_POS, readed_frame, ms);
		}
		else {
			showWindow(inverse(center), analyser.getMixedMatrice(conversion), ms);
		}
		if (roiSetup) {
			chooseROI(analyser.getMixedMatrice(conversion));
			cout << "ROI: " << roi << endl;
			center = NULL_POS;
		}
		if (currentLoadedFrame == actualWatchedFrame) {
			if (watchedOpacity != conversion * 100) {
				conversion = static_cast<float>(watchedOpacity) / 100;
				shouldCalculate = false;

			}
			else {
				shouldCalculate = true;
			}
			continue;
		}
		shouldCalculate = true;
		if (currentLoadedFrame == actualWatchedFrame + 1) {
			analyser.setIsInitialSearch(true);
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
			cout << "End Video calculation" << endl;
			shutDownPrompt();
			break;
		}
	}

	string name;

	cout << "name of file: ";
	cin >> name;
	cout << endl;

	string directoryPath = "C:/Users/fagot/Code/TableTennis/output/" + name + "/";

	if (!filesystem::exists(directoryPath)) {
		if (filesystem::create_directories(directoryPath)) {
			cerr << "Failed to create the directory" << endl;
		}
	}

	ofstream file;
	file.open("C:/Users/fagot/Code/TableTennis/output/" + name + "/tracked-0.txt");
	if (!file.is_open()) {
		cerr << "Cannot save tracking data" << endl;
	}
	int file_k = 0;
	bool first = true;
	for (int i = 0; i <= total_frames; i++) {
		if (positionsResults[i] == NULL_POS || positionsResults[i] == Pos{ 0, 0 }) {
			if (first) {
				continue;
			}
			file_k++;
			file.close();
			file.open("C:/Users/fagot/Code/TableTennis/output/" + name + "/tracked-" + to_string(file_k) + ".txt");
			if (!file.is_open()) {
				cerr << "Cannot save tracking data" << endl;
			}
			first = true;
			continue;
		}
		first = false;
		file << positionsResults[i].x << ";" << positionsResults[i].y << ";" << endl;
	}
	file.close();
	if (first) {
		filesystem::remove("C:/Users/fagot/Code/TableTennis/output/" + name + "/tracked-" + to_string(file_k) + ".txt");
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