#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
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
	VideoCapture capture(testFilePath);

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

	// we read every frame and store it inside a vector (list), that's why we need a small video
	vector<Mat> readed_frames;
	Mat readed_frame;
	while (capture.read(readed_frame)) {
		// don't forget to .clone() in order to have the content and not the element
		readed_frames.push_back(readed_frame.clone());
	}

	// prepare the window to show frames
	initialisePrompts();

	// do while loop for loaded image, work 'till showWindow() return True
	do {
		// get the image from the list
		Mat originalFrame = readed_frames[actualWatchedFrame].clone();
		// resize only in the region we are interested in
		Mat imagetoProcess = originalFrame(testFileROI);

		// analyse the frame
		
		/*
		Analyser analyser(ref(imagetoProcess));

		// if center is positive ie it exist, we print a circle in the resultImage and in the original one too
		if (analyser.getCenterX() >= 0 and analyser.getCenterY() >= 0) {
			printCircle(analyser.getResultMatrice(), analyser.getCenterPixels());
			printCircle(ref(originalFrame), analyser.getCenterPixels());
		}

		// getting a black full size matrice
		Mat blackMatrice(height, width, CV_8UC3, Scalar(0, 0, 0));
		// coppy on the top left our image, in order to rescal it full size (because we only tracked in a portion of the image)
		Mat regionOfInterestMatrice = blackMatrice(testFileROI);
		analyser.getResultMatrice().copyTo(regionOfInterestMatrice);
		// fusionning result and orignal matrice (depending on opacity) so we can see both by transparency
		matForIniti.deallocate();
		const float conversion = static_cast<float>(watchedOpacity) / 100;
		addWeighted(originalFrame, conversion, blackMatrice, 1 - conversion, 0.0, matForIniti);
		printRectangle(ref(matForIniti), testFileROI);
		*/
	} while (showWindow());
}


void multithreading() {
	//glob(path, filenames, false);
	filenames.push_back(testFilePath);
	for (int k = 0; k < filenames.size(); k ++) {
		cout << "Processing video: " << filenames[k] << endl;
		processVideo(ref(filenames[k]), k);
		cout << "End of processing: " << filenames[k] << endl;
		for (int i = 0; i < total_frames; i ++) {
			orderedPositions.push_back(positionsResults[i % number_of_threads][i / number_of_threads]);
		}
		for (const Pos& position : orderedPositions) {
			cout << position.x << ";" << position.y << ";" << endl;
		}
		ofstream file;
		file.open("processing/output/" + to_string(k) + ".txt");
		if (file.is_open()) {
			for (const Pos& position : orderedPositions) {
				file << position.x << ";" << position.y << ";" << endl;
			}
			file.close();
		}
		frames.clear();
		positionsResults.clear();
		orderedPositions.clear();
		currentLoadedFrame = { 0 };
	}
}

void loadFrames(VideoCapture& capture, const int video_number) {
	int next_frame_number = currentLoadedFrame + number_frames_to_read_ahead;
	if (next_frame_number >= total_frames) {
		next_frame_number = total_frames;
	}

	for (int i = currentLoadedFrame; i < next_frame_number; ++i) {
		Mat frame;
		if (!capture.read(frame)) {
			cout << "------------------Failed to read frame------------------" << i << endl;
			continue;
		}
		cout << "adding frame: " << i << endl;
		Frame value = { i, frame(regions_of_interest[video_number]) };
		frames[i % number_of_threads].push_back(value);
	}

	currentLoadedFrame = next_frame_number;
	shouldLoadFrames = false;
}

void processVideo(const string& filename, const int video_number) {
	VideoCapture capture(filename);
	if (!capture.isOpened()) {
		cerr << "Failed to open this video: " << filename << endl;
	}

	fps = capture.get(CAP_PROP_FPS);
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);
	total_frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));

	cout << fps << " " << width << " " << height << " " << total_frames << " " << number_of_threads << endl;

	for (int i = 0; i < number_of_threads; i++) {
		vector<Pos> ini1;
		positionsResults.push_back(ini1);
		vector<Frame> ini2;
		frames.push_back(ini2);
	}

	loadFrames(ref(capture), video_number);

	vector<thread> threads;

	for (int i = 0; i < number_of_threads; i++) {
		const int thread_number = i;
		const int limite = total_frames% number_of_threads;
		threads.emplace_back([&, thread_number]() {
			int quotient(total_frames / number_of_threads);
			if (thread_number < limite) {
				quotient += 1;
			}
			cout << "Thread " << thread_number << " started processing" << endl;
			for (int j = 0; j < quotient; ++j) {
				processFrame(thread_number, j);
			}
			return;
			});
	}
	while (currentLoadedFrame < total_frames) {
		unique_lock<mutex> lock(mtx);
		cvariable.wait(lock, [] { return shouldLoadFrames == true; });
		loadFrames(ref(capture), video_number);
		cvariable.notify_all();
	}
	for (int i = 0; i < number_of_threads; i++) {
		cout << "thread joining: " << i << endl;
		threads[i].join();
	}
}

void processFrame(const int i, const int j) {
	Frame value = getFrame(i, j);
	const int frame_number = value.number;
	const Mat& frame = value.matrice;
	cout << "Processing frame: " << frame_number << endl;

	Mat hsv_frame;
	cvtColor(frame, hsv_frame, COLOR_BGR2HSV);

	Mat mask;
	inRange(hsv_frame, getScalarFromHSVColor(lower_color), getScalarFromHSVColor(upper_color), mask);

	int num_non_black_pixels = countNonZero(mask);
	if (num_non_black_pixels == 0) {
		Pos null = { -1, -1 };
		positionsResults[i].push_back(null);
		return;
	}

	Moments m = moments(mask, true);
	Point center(m.m10 / m.m00, m.m01 / m.m00);

	Pos pos = {  center.x, center.y };
	positionsResults[i].push_back(pos);

	Mat empty_mat;
	frames[i][j].matrice.deallocate();
	frames[i][j].matrice = empty_mat;

}

Frame getFrame(const int i, const int j) {
	if (j < frames[i].size()) {
		return frames[i][j];
	}

	cout << "calling mutex: " << j*number_of_threads + i << endl;
	unique_lock<mutex> lock(mtx);
	shouldLoadFrames = true;
	cvariable.notify_one();
	while (j >= frames[i].size()) {
		cvariable.wait(lock);
	}
	return frames[i][j];
}