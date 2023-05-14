#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include "configuration.hpp"
#include "tracking.hpp"

using namespace std;
using namespace cv;

void initTracking() {
	// initializer, set default values for all variables that are in configuration.cpp
	currentLoadedFrame = 0;
	actualWatchedFrame = 0;
	watchedOpacity = 1;
	watchedZoom = 1;
	shouldLoadFrames = false;
	watchedPos = { 0, 0 };

	// default orange range, the best one we figured out for now
	lower_color = Scalar(0, 65, 65); 
	upper_color = Scalar(20, 255, 255);
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

	// do while loop for loaded image, work 'till showWindow() return True
	do {
		// process the frame

		// get the image from the list
		Mat originalFrame = readed_frames[actualWatchedFrame];
		// resize only in the region we are interested in
		Mat imagetoProcess = originalFrame(testFileROI);
		// convert to hsv image format
		Mat hsvImage;
		cvtColor(imagetoProcess, hsvImage, COLOR_BGR2HSV);
		// this matrice is a mask that is blank if pixel are in the range, black otherwise
		Mat maskImage;
		inRange(hsvImage, lower_color, upper_color, maskImage);
		// we fusion original image and mask to make only pixel in range appear
		Mat resultImage;
		bitwise_and(imagetoProcess, imagetoProcess, resultImage, maskImage);
		// calculate the center of the detected pixels
		Moments m = moments(maskImage, true);
		Point center(m.m10 / m.m00, m.m01 / m.m00);

		// if center is positive ie it exist, we print a circle in the resultImage and in the original one too
		if (center.x >= 0 and center.y >= 0) {
			printCircleCenter(ref(resultImage), center.x, center.y);
			printCircleCenter(ref(originalFrame), center.x, center.y);
		}

		// getting a black full size matrice
		Mat blackMat(height, width, CV_8UC3, Scalar(0, 0, 0));
		// coppy on the top left our image, in order to rescal it full size (because we only tracked in a portion of the image)
		Mat roi_image = blackMat(testFileROI);
		resultImage.copyTo(roi_image);
		// fusionning result and orignal matrice (depending on opacity) so we can see both by transparency
		Mat temp;
		addWeighted(originalFrame, watchedOpacity, blackMat, 1 - watchedOpacity, 0.0, temp);
		// set the variable to this matrice, so showWIndow will find it
		matForIniti = temp;
	} while (showWindow());
}

bool showWindow() {
	Size imageSize(width * imageScalar * windowScalar, height * imageScalar * windowScalar);
	Size windowSize(width * windowScalar, height * windowScalar);
	Point imagePosition(0, 0);
	namedWindow(windowName, WINDOW_NORMAL);
	resizeWindow(windowName, windowSize);

	int mouseData[2] = { 0, 0 };
	setMouseCallback(windowName, mouseCallback, mouseData);

	thread keyboardThread;

	while (true) {

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
				break;
			}
		}
		else if (key == 2555904) {  //right_arrow_key
			if (actualWatchedFrame < total_frames - 1) {
				actualWatchedFrame += 1;
				break;
			}
		}
		else if (key == 2490368) {  //up_arrow_key
			if (watchedOpacity < 1) {
				watchedOpacity += 0.25;
				break;
			}
		}
		else if (key == 2621440) {  //down_arrow_key
			if (watchedOpacity > 0) {
				watchedOpacity -= 0.25;
				break;
			}
		}
		else if (key == 27) { //espace
			return false;
		}
		else if (key == 'i') {
			cout << "Give your values, here are actual ones" << endl 
				<< lower_color[0] << " " << lower_color[1] << " " << lower_color[2] << " "
				<< upper_color[0] << " " << upper_color[1] << " " << upper_color[2] << endl;
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
			lower_color = Scalar(stof(inputs[0]), stof(inputs[1]), stof(inputs[2]));
			upper_color = Scalar(stof(inputs[3]), stof(inputs[4]), stof(inputs[5]));
			cout << "Values are now:" << endl
				<< lower_color[0] << " " << lower_color[1] << " " << lower_color[2] << " "
				<< upper_color[0] << " " << upper_color[1] << " " << upper_color[2] << endl;
			break;
		}

	}
	cout << "processing ..." << endl;
	return true;
}



void mouseCallback(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONUP) {
		*(int*)userdata = 0;
		*((int*)userdata + 1) = 0;
	}else if (event == EVENT_LBUTTONDBLCLK) {
		if (watchedZoom < 10) {
			watchedZoom *= 2;
			setNewWatchedPos(x / 2, y / 2);
		}
	}else if (event == EVENT_RBUTTONDOWN) {
		if (watchedZoom > 1) {
			watchedZoom = 1;
			watchedPos = Pos{ 0, 0 };
		}
	}else if (flags == EVENT_FLAG_LBUTTON && event == EVENT_MOUSEMOVE) {
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

void keyboardCallback(int key, void* userdata) {

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

void printCircleCenter(Mat& mat, const int x, const int y) {
	circle(mat, Point(x, y), 50, Scalar(0, 0, 255), 4, LINE_AA, false);
}












void multithreading() {
	//glob("C:\\Users\\fagot\\Videos\\tipe\\*.MP4", filenames, false);
	filenames.push_back("C:\\Users\\fagot\\Videos\\tipe\\test2.MP4");
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
	inRange(hsv_frame, lower_color, upper_color, mask);

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


void cutter() {
	// Open the input video
	VideoCapture input_cap("C:\\Users\\fagot\\Videos\\tipe\\MVI_0009.MP4");
	if (!input_cap.isOpened()) {
		std::cerr << "Error: Could not open input video\n";
		return;
	}

	// Get input video properties
	double fps = input_cap.get(CAP_PROP_FPS);
	int width = input_cap.get(CAP_PROP_FRAME_WIDTH);
	int height = input_cap.get(CAP_PROP_FRAME_HEIGHT);
	int total_frames = input_cap.get(CAP_PROP_FRAME_COUNT);

	// Define output video properties
	int start_frame = 15*fps+11;
	int end_frame = 22*fps;
	int output_width = width;
	int output_height = height;
	std::string output_filename = "C:\\Users\\fagot\\Videos\\tipe\\test2.MP4";

	// Open the output video
	VideoWriter output_cap(output_filename, VideoWriter::fourcc('X', '2', '6', '4'), fps, Size(output_width, output_height));
	if (!output_cap.isOpened()) {
		std::cerr << "Error: Could not open output video\n";
		return;
	}

	// Set the input video position to the start frame
	input_cap.set(CAP_PROP_POS_FRAMES, start_frame);

	// Read and write the frames from start frame to end frame
	for (int i = start_frame; i <= end_frame; i++) {
		Mat frame;
		input_cap >> frame;
		output_cap.write(frame);
	}

	// Release the input and output videos
	input_cap.release();
	output_cap.release();

}


void processVideoSingleThreaded(VideoCapture capture, String name) {
	Rect region_of_interest(0, 0, 1920, 1080 / 2);
	Mat total_frame;
	int i(1);
	while (capture.read(total_frame)) {

		Mat frame = total_frame(region_of_interest);

		// Convert the frame from RGB to HSV
		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		// Threshold the image based on the color range
		Mat mask;
		Scalar lower_color(0, 100, 100);
		Scalar upper_color(20, 255, 255);
		inRange(hsv, lower_color, upper_color, mask);

		// Apply the mask to the original image to isolate the object
		Mat result;
		bitwise_and(frame, frame, result, mask);

		// Display the segmented image for tuning the color range values
		// writer.write(result);
		Moments m = moments(mask, true);
		Point center(m.m10 / m.m00, m.m01 / m.m00);

		if (center.x >= 0 and center.y >= 0) {
			printCircleCenter(ref(result), center.x, center.y);
		}

		//imshow("Mask", result);
		//imshow("Original", frame);

		Size size(1920, 1080/2);
		Mat blackMat(1080, 1920, CV_8UC3, cv::Scalar(0, 0, 0));
		Rect roi(Point(0, 0), size);
		Mat roi_image = blackMat(roi);
		resize(result, roi_image, size);

		cout << "Image " << i << " proccessed" << endl;
		waitKey(1);
		i++;
	}
	
	showWindow();

}


void singlethreading() {
	//glob(path, filenames, false);
	filenames.push_back("C:\\Users\\fagot\\Videos\\tipe\\test1.mp4");
	for (const auto& filename : filenames) {
		VideoCapture capture(filename);
		if (!capture.isOpened()) {
			cerr << "Failed to open this video: " << filename << endl;
			continue;
		}
		processVideoSingleThreaded(capture, filename);
		cout << filename << " was processed" << endl;
		break;
	}
}