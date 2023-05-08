#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <mutex>

using namespace std;
using namespace cv;


Rect regions_of_interest[] = {
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 1080 / 2, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2),
	Rect(0, 0, 1920, 1080 / 2)
};
const Scalar lower_color(0, 100, 100);
const Scalar upper_color(20, 255, 255);
const int number_frames_to_read_ahead = 100;

typedef struct Pos Pos;
struct Pos {
	int x;
	int y;
};
typedef struct Frame Frame;
struct Frame {
	int number;
	Mat matrice;
};

void processVideo(const string& filename, const int video_number);
void processFrame(const int i, const int j);
Frame getFrame(const int i, const int j);

vector<vector<Frame>> frames;
vector<vector<Pos>> positionsResults;
vector<Pos> orderedPositions;
mutex mtx;
condition_variable cvariable;
int current_frame_number(0);
atomic<bool> shouldLoadFrames(false);

int total_frames;
int fps;
int width;
int height;
int number_of_threads;


void multithreading() {

	vector<string> filenames;
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
		current_frame_number = { 0 };
	}

}

void loadFrames(VideoCapture& capture, const int video_number) {
	int next_frame_number = current_frame_number + number_frames_to_read_ahead;
	if (next_frame_number >= total_frames) {
		next_frame_number = total_frames;
	}

	for (int i = current_frame_number; i < next_frame_number; ++i) {
		Mat frame;
		if (!capture.read(frame)) {
			cout << "------------------Failed to read frame------------------" << i << endl;
			continue;
		}
		cout << "adding frame: " << i << endl;
		Frame value = { i, frame(regions_of_interest[video_number]) };
		frames[i % number_of_threads].push_back(value);
	}

	current_frame_number = next_frame_number;
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

	number_of_threads = thread::hardware_concurrency();

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
	while (current_frame_number < total_frames) {
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
	// Define the output video file properties
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4 codec
	double fps = capture.get(CAP_PROP_FPS);
	Size frame_size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	string output_name = name;
	output_name.erase(output_name.size() - 4);
	output_name += "-tracked.MP4";
	//VideoWriter writer(output_name, fourcc, fps, frame_size);

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
			result.at<Vec3b>(center.y, center.x) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y + 1, center.x) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y - 1, center.x) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y, center.x + 1) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y, center.x - 1) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y + 1, center.x + 1) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y - 1, center.x - 1) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y - 1, center.x + 1) = Vec3b(0, 0, 255);
			result.at<Vec3b>(center.y + 1, center.x - 1) = Vec3b(0, 0, 255);
		}
		
		imshow("Mask", result);
		cout << "Image " << i << " proccessed" << endl;
		waitKey(1);
		i++;
	}
}



void singlethreading() {
	vector<string> filenames;
	//glob("C:\\Users\\fagot\\Videos\\tipe\\*.MP4", filenames, false);
	filenames.push_back("C:\\Users\\fagot\\Videos\\tipe\\test2.MP4");
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

int main() {
	multithreading();
	return 0;
}