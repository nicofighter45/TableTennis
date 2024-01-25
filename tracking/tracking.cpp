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
	watchedPos = { 0, 0 };
	autoState = true;
	reloadFromCamera = NULL_POS;
	roiSetup = true;
	shouldCalculate = true;

	// default orange range, the best one we figured out for now
	lower_color = HSVColor{ 25, 120, 150};
	upper_color = HSVColor{ 40, 255, 255 };
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


void setupTracking() {

	cout << "Library: OpenCV version: " << getVersionString() << endl;

	// the capture of the video
	string filePath = getFile();
	if (filePath == "") {
		return;
	}
	cout << "FilePath: " << filePath << endl;

	streambuf* stream_buffer_cout = cout.rdbuf();
	ofstream out("output.txt");
	cout.rdbuf(out.rdbuf());
	VideoCapture capture(filePath);
	cout.rdbuf(stream_buffer_cout);
	
	// error if video can't be open
	if (!capture.isOpened()) {
		cerr << "Failed to open video" << endl;
	}

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

	launchTracking(capture);

}


void launchTracking(VideoCapture capture) {

	Mat readed_frame;
	capture.read(readed_frame);

	streambuf* stream_buffer_cout = cout.rdbuf();
	ofstream out("output.txt");
	cout.rdbuf(out.rdbuf());
	chooseROI(readed_frame);
	cout.rdbuf(stream_buffer_cout);

	cout << "ROI: " << roi << endl;

	bool reload = true;
	float conversion = static_cast<float>(watchedOpacity) / 100;
	Analyser analyser(ref(readed_frame));
	Pos center = NULL_POS;
	map < int, Pos > positionsResults;


	while (true) {
		auto loop_time_start = chrono::high_resolution_clock::now();
		int ms = 0;
		if (shouldCalculate && reload) {
			reloadFromCamera = analyser.findBall();
			if (reloadFromCamera != NULL_POS) {
				center = analyser.findBall();
				if (distance(center, reloadFromCamera) >= spacingBetweenCentersToStop) {
					autoState = false;
				}
				reloadFromCamera = NULL_POS;
			}
			else {
				center = NULL_POS;
				cout << "Frame " << capture.get(CAP_PROP_POS_FRAMES) - 1 << " no center" << endl;
			}
			positionsResults[currentLoadedFrame] = center;

		}

		auto loop_time_second = chrono::high_resolution_clock::now();

		if (not roiSetup) {
			cout.rdbuf(out.rdbuf());
			if (center == NULL_POS) {
				showWindow(NULL_POS, readed_frame, ms);
			}
			else {
				showWindow(inverse(center), analyser.getMixedMatrice(conversion), ms);
			}
			cout.rdbuf(stream_buffer_cout);
		}

		auto loop_time_third = chrono::high_resolution_clock::now();

		if (roiSetup) {
			cout.rdbuf(out.rdbuf());
			chooseROI(analyser.getMixedMatrice(conversion));
			cout.rdbuf(stream_buffer_cout);
			if (!roiSetup) {
				center = NULL_POS;
				shouldCalculate = true;
				capture.set(CAP_PROP_POS_FRAMES, actualWatchedFrame);
				cout << "ROI: " << roi << endl;
			}
			else {
				capture.set(CAP_PROP_POS_FRAMES, actualWatchedFrame);
				currentLoadedFrame = actualWatchedFrame;
			}
		}
		else {
			if (currentLoadedFrame == actualWatchedFrame) {
				if (watchedOpacity != conversion * 100) {
					conversion = static_cast<float>(watchedOpacity) / 100;
					reload = false;

				}
				else {
					reload = true;
				}
				continue;
			}
			reload = true;
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
		}
		auto loop_time_last = chrono::high_resolution_clock::now();
		cout << "Loop calc" <<
			chrono::duration_cast<chrono::milliseconds>
			(loop_time_second - loop_time_start).count() << "ms" << endl
			<< "Loop visu" <<
			chrono::duration_cast<chrono::milliseconds>
			(loop_time_third - loop_time_second).count() << "ms" << endl
			<< "Loop new" <<
			chrono::duration_cast<chrono::milliseconds>
			(loop_time_last - loop_time_third).count() << "ms" << endl
			<< "Loop total" <<
			chrono::duration_cast<chrono::milliseconds>
			(loop_time_last - loop_time_start).count() << "ms" << endl
			<< endl;
		if (!capture.read(readed_frame)) {
			cout << "End Video calculation" << endl;
			shutDownPrompt();
			break;
		}
	}

	saveTracking(positionsResults);

}


void saveTracking(map < int, Pos > positionsResults) {
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
	file.open(directoryPath + "tracked-0.txt");
	if (!file.is_open()) {
		cerr << "Cannot save tracking data" << endl;
	}
	int file_k = 0;
	int nb_of_frame_not_null = 0;
	int first_to_remove = 0;
	vector<string> lines;
	for (int i = 0; i <= total_frames; i++) {
		if (positionsResults[i] == NULL_POS || positionsResults[i] == Pos{ 0, 0 }) {
			if (nb_of_frame_not_null == 0) {
				continue;
			}
			cout << first_to_remove << endl;
			for (string line : lines) {
				if (first_to_remove > 0) {
					first_to_remove--;
				}
				else {
					file << line << endl;
				}
			}
			file.close();
			if (nb_of_frame_not_null <= 5) {
				streambuf* stream_buffer_cout = cout.rdbuf();
				ofstream out("output.txt");
				cout.rdbuf(out.rdbuf());
				filesystem::remove(directoryPath + "tracked-" + to_string(file_k) + ".txt");
				cout.rdbuf(stream_buffer_cout);
			}
			else {
				file_k++;
			}
			file.open(directoryPath + "tracked-" + to_string(file_k) + ".txt");
			if (!file.is_open()) {
				cerr << "Cannot save tracking data" << endl;
			}
			nb_of_frame_not_null = 0;
			first_to_remove = 0;
			continue;
		}
		nb_of_frame_not_null++;
		if (nb_of_frame_not_null == 6) {
			if (distance(positionsResults[i], positionsResults[i-5]) < 10.0) {
				cout << first_to_remove << endl;
				first_to_remove++;
				nb_of_frame_not_null--;
			}
		}
		lines.push_back(to_string(positionsResults[i].x) + ";"
			+ to_string(positionsResults[i].y) + ";");
	}
	cout << first_to_remove << endl;
	for (string line : lines) {
		if (first_to_remove > 0) {
			first_to_remove--;
		}
		else {
			file << line << endl;
		}
	}
	file.close();
	if (nb_of_frame_not_null <= 5) {
		streambuf* stream_buffer_cout = cout.rdbuf();
		ofstream out("output.txt");
		cout.rdbuf(out.rdbuf());
		filesystem::remove(directoryPath + "tracked-" + to_string(file_k) + ".txt");
		cout.rdbuf(stream_buffer_cout);
	}
}

