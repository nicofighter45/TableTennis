#pragma once

#include "opencv2/opencv.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace cv;

void processVideo(VideoCapture capture, String name) {
	// Define the output video file properties
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4 codec
	double fps = capture.get(CAP_PROP_FPS);
	Size frame_size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	string output_name = name;
	output_name.erase(output_name.size() - 4);
	output_name += "-tracked.MP4";
	VideoWriter writer(output_name, fourcc, fps, frame_size);

	Mat frame;
	while (capture.read(frame)) {
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
		writer.write(result);
	}
}

int main() {
	vector<string> filenames;
	glob("C:\\Users\\fagot\\Videos\\tipe\\*.MP4", filenames, false);
	for (const auto& filename : filenames) {
		VideoCapture capture(filename);
		if (!capture.isOpened()) {
			cerr << "Failed to open this video : " + filename << endl;
			continue;
		}
		processVideo(capture, filename);
		cout << filename << " was processed" << endl;
	}
	return 0;
}
