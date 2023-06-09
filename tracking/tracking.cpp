#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include "../processing/configuration.hpp"

using namespace std;
using namespace cv;

void processVideo(const string& filename, const int video_number);
void processFrame(const int i, const int j);
Frame getFrame(const int i, const int j);
void printCenter(Mat& mat, const int x, const int y);

int current_frame_number;

void multithreading() {
	for (int k = 0; k < filenames.size(); k++) {
		cout << "Processing video: " << filenames[k] << endl;
		processVideo(ref(filenames[k]), k);
		cout << "End of processing: " << filenames[k] << endl;
		for (int i = 0; i < total_frames; i++) {
			orderedPositions.push_back(positionsResults[i % number_of_threads][i / number_of_threads]);
		}
		for (const Pos& position : orderedPositions) {
			cout << position.x << ";" << position.y << ";" << endl;
		}
		ofstream file;
		file.open("C:/Users/fagot/OneDrive/Documents/MPSI/TableTennis/processing/output/" + to_string(k) + "-tracked.txt");
		if (file.is_open()) {
			for (const Pos& position : orderedPositions) {
				file << position.x << ";" << position.y << ";" << endl;
			}
			file.close();
		}
		frames.clear();
		positionsResults.clear();
		orderedPositions.clear();
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
		const int limite = total_frames % number_of_threads;
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
	inRange(hsv_frame, getScalarFromHSVColor(lower_color), 
		getScalarFromHSVColor(upper_color), mask);

	int num_non_black_pixels = countNonZero(mask);
	if (num_non_black_pixels == 0) {
		Pos null = { -1, -1 };
		positionsResults[i].push_back(null);
		return;
	}

	Moments m = moments(mask, true);
	Point center(m.m10 / m.m00, m.m01 / m.m00);

	Pos pos = { center.x, center.y };
	positionsResults[i].push_back(pos);

	Mat empty_mat;
	frames[i][j].matrice.deallocate();
	frames[i][j].matrice = empty_mat;

}

Frame getFrame(const int i, const int j) {
	if (j < frames[i].size()) {
		return frames[i][j];
	}

	cout << "calling mutex: " << j * number_of_threads + i << endl;
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
	VideoCapture input_cap("C:/Users/fagot/ShadowDrive/tipe/MVI_0013.MP4");
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
	int start_frame = 2 * fps + 0;
	int end_frame = 15 * fps + 0;
	int output_width = width;
	int output_height = height;
	std::string output_filename = "C:/Users/fagot/ShadowDrive/tipe/output/rebond_sol.MP4";

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
	/*
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4 codec
	double fps = capture.get(CAP_PROP_FPS);
	Size frame_size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	string output_name = name;
	output_name.erase(output_name.size() - 4);
	output_name += "-tracked.MP4";
	*/
	//VideoWriter writer(output_name, fourcc, fps, frame_size);

	//Rect region_of_interest(1200, 500, 700, 200);
	Rect region_of_interest(100, 750, 1810, 300);

	vector<Pos> positions;

	Mat total_frame;
	int i(1);
	while (capture.read(total_frame)) {
		Mat frame = total_frame(region_of_interest);
		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);
		Mat mask;
		inRange(hsv, getScalarFromHSVColor(lower_color), getScalarFromHSVColor(upper_color), mask);
		Mat result;
		bitwise_and(frame, frame, result, mask);
		Moments m = moments(mask, true);
		Point center(m.m10 / m.m00, m.m01 / m.m00);
		if (center.x >= 0 and center.y >= 0) {
			printCenter(ref(result), center.x, center.y);
			Pos pos = { center.x, center.y };
			positions.push_back(pos);
		}
		Mat blanck(20, 1810, CV_8UC3, Scalar(255, 255, 255));
		Mat mat;
		vconcat(frame, blanck, mat);
		vconcat(mat, result, mat);
		putText(mat, "Video original", {70, 50}, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(255, 255, 255), 4, LINE_AA);
		putText(mat, "Video detecte", {70, 600}, FONT_HERSHEY_SIMPLEX, 1.5, Scalar(255, 255, 255), 4, LINE_AA);
		imshow("Mask", mat);
		cout << "Image " << i << " proccessed" << endl;
		imwrite("C:/Users/fagot/OneDrive/Documents/MPSI/TableTennis/processing/output/third/image-" + to_string(i) + ".png", mat);
		waitKey(1);
		i++;
	}

	ofstream file;
	file.open("C:/Users/fagot/OneDrive/Documents/MPSI/TableTennis/processing/output/" + to_string(i) + "-tracked.txt");
	if (file.is_open()) {
		for (const Pos& position : positions) {
			file << position.x << ";" << position.y << ";" << endl;
		}
		file.close();
	}
}


void printCenter(Mat& mat, const int x, const int y) {
	mat.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y + 1, x) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y - 1, x) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y, x + 1) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y, x - 1) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y + 1, x + 1) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y - 1, x - 1) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y - 1, x + 1) = Vec3b(0, 0, 255);
	mat.at<Vec3b>(y + 1, x - 1) = Vec3b(0, 0, 255);
}


void singlethreading() {
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
	lower_color = HSVColor{ 15, 130, 130};
	upper_color = HSVColor{ 28, 255, 255 };
	//glob("C:/Users/fagot/ShadowDrive/tipe/output/*.MP4", filenames, false);
	filenames.push_back("C:/Users/fagot/ShadowDrive/tipe/output/rebond_sol.MP4");
	//filenames.push_back("C:/Users/fagot/ShadowDrive/tipe/test1.MP4");
	singlethreading();
	return 0;
}