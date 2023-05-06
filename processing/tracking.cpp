#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>

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
Scalar lower_color_rgb, upper_color_rgb;

Scalar convertHSVtoRGB(const Scalar HSVcolor) {
	// made using https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur
	const int h = HSVcolor[0];
	const int s = HSVcolor[1];
	const int v = HSVcolor[2];
	const int hi = (h/60) % 6;
	const int f = h / 60 - hi;
	const int p = v * (255 - s) / 255;
	const int q = v * (255 - f * s / 255) / 255;
	const int t = v * (255 - (255 - f) * s / 255) / 255;

	int r, g, b;

	switch (hi) {
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case 5: r = v; g = p; b = q; break;
	}

	return Scalar(r, g, b);
}

typedef struct Pos Pos;
struct Pos {
	int frame;
	double X;
	double Y;
};

void processVideo(const string& filename, const int video_number);
void processFrames(VideoCapture& capture, const int start, const int end, const int video_number);
void processFrame(const Mat& frame, const int frame_number);
Mat getFrame(VideoCapture& capture, const int frame_number);

vector<thread> threads;
vector<Pos> positionsResults;
int fps;
int width;
int height;

int main() {

	lower_color_rgb = convertHSVtoRGB(lower_color);
	upper_color_rgb = convertHSVtoRGB(upper_color);

	vector<string> filenames;
	//glob("C:\\Users\\fagot\\Videos\\tipe\\*.MP4", filenames, false);
	filenames.push_back("C:\\Users\\fagot\\Videos\\tipe\\mtest.MP4");
	for (int i = 0; i < filenames.size(); i ++) {
		cout << "Processing video: " << filenames[i] << endl;
		processVideo(ref(filenames[i]), i);
		for (int i = 0; i < threads.size(); i++) {
			threads[i].join();
		}
		ofstream file;
		file.open(i + "_tracking.txt");
		if (file.is_open()) {
			for (const auto& position : positionsResults) {
				file << position.frame << ";" << position.X << ";" << position.Y << ";" << endl;
			}
			file.close();
		}
	}

	return 0;
}

void processVideo(const string& filename, const int video_number) {
	VideoCapture capture(filename);
	if (!capture.isOpened()) {
		cerr << "Failed to open this video: " << filename << endl;
	}

	fps = capture.get(CAP_PROP_FPS);
	width = capture.get(CAP_PROP_FRAME_WIDTH);
	height = capture.get(CAP_PROP_FRAME_HEIGHT);

	const int total_frames = static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));

	// we create a thread for each core of the processor
	int number_of_threads = thread::hardware_concurrency();
	for (int i = 0; i < number_of_threads; i++) {
		thread thread(processFrames, ref(capture), i*total_frames/number_of_threads, (i+1) * total_frames / number_of_threads, video_number);
		threads.push_back(move(thread));
	}
}

void processFrames(VideoCapture& capture, const int start, const int end, const int video_number) {
	for (int i = start; i < end; i++) {
		processFrame(getFrame(capture, i)(regions_of_interest[video_number]), i);
	}
}

void processFrame(const Mat& frame, const int frame_number) {
	int num_non_black_pixels = 0;
	double sum_x = 0;
	double sum_y = 0;
	for (int row = 0; row < frame.rows; ++row) {
		for (int col = 0; col < frame.cols; ++col) {
			Vec3b pixel = frame.at<Vec3b>(row, col);
			if (pixel[0] >= lower_color_rgb[0] && pixel[0] <= upper_color_rgb[0] &&
				pixel[1] >= lower_color_rgb[1] && pixel[1] <= upper_color_rgb[1] &&
				pixel[2] >= lower_color_rgb[2] && pixel[2] <= upper_color_rgb[2]) {
				num_non_black_pixels++;
				sum_x += row;
				sum_y += col;
			}
		}
	}

	Pos mean_pos;
	if (num_non_black_pixels > 0) {
		mean_pos.frame = frame_number;
		mean_pos.X = sum_x / num_non_black_pixels;
		mean_pos.Y = sum_y / num_non_black_pixels;
		positionsResults.push_back(mean_pos);
	}
}

Mat getFrame(VideoCapture& capture, const int frame_number) {
	capture.set(CAP_PROP_POS_FRAMES, frame_number);
	Mat frame;
	if (capture.grab() && capture.retrieve(frame)) {
		return ref(frame);
	}
	else {
		cerr << "Fail to get the frame " << frame_number << endl;
	}
}





int cutvideo() {
	// Open the input video
	VideoCapture input_cap("C:\\Users\\fagot\\Videos\\tipe\\MVI_0009.MP4");
	if (!input_cap.isOpened()) {
		std::cerr << "Error: Could not open input video\n";
		return 1;
	}

	// Get input video properties
	double fps = input_cap.get(CAP_PROP_FPS);
	int width = input_cap.get(CAP_PROP_FRAME_WIDTH);
	int height = input_cap.get(CAP_PROP_FRAME_HEIGHT);
	int total_frames = input_cap.get(CAP_PROP_FRAME_COUNT);

	// Define output video properties
	int start_frame = 15*fps+11;
	int end_frame = 18*fps-17;
	int output_width = width;
	int output_height = height;
	std::string output_filename = "C:\\Users\\fagot\\Videos\\tipe\\test.MP4";

	// Open the output video
	VideoWriter output_cap(output_filename, VideoWriter::fourcc('X', '2', '6', '4'), fps, Size(output_width, output_height));
	if (!output_cap.isOpened()) {
		std::cerr << "Error: Could not open output video\n";
		return -1;
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

	return 0;
}


void processVideoSingleThreaded(VideoCapture capture, String name) {
	// Define the output video file properties
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4 codec
	double fps = capture.get(CAP_PROP_FPS);
	Size frame_size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	string output_name = name;
	output_name.erase(output_name.size() - 4);
	output_name += "-tracked.MP4";
	VideoWriter writer(output_name, fourcc, fps, frame_size);

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
		imshow("Segmented Image", result);
		writer.write(frame);
		waitKey(1);
		cout << "Image " << i << " proccessed" << endl;
		i++;
	}
}



int main_singlethreaded() {
	vector<string> filenames;
	glob("C:\\Users\\fagot\\Videos\\tipe\\*.MP4", filenames, false);
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
	return 0;
}
