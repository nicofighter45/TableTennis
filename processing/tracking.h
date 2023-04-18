#include <opencv.hpp>
#include <vector>
#include <tuple>

using Coordinate = std::tuple<float, float>;

int main() {
  cv::Mat image = cv::imread("path/to/your/image.jpg");
  if (image.empty()) {
    std::cerr << "Error opening image file" << std::endl;
    return -1;
  }

  cv::Mat hsv_image, mask;
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

  cv::Scalar lower_orange(10, 100, 100);
  cv::Scalar upper_orange(20, 255, 255);
  cv::inRange(hsv_image, lower_orange, upper_orange, mask);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  std::vector<Coordinate> ball_positions;
  for (const auto& contour : contours) {
    double area = cv::contourArea(contour);
    if (area < 100) {
      continue;
    }

    cv::Moments moments = cv::moments(contour, true);
    float x = moments.m10 / moments.m00;
    float y = moments.m01 / moments.m00;
    ball_positions.push_back({x, y});
  }

  for (const auto& pos : ball_positions) {
    std::cout << "Ball position: (" << std::get<0>(pos) << ", " << std::get<1>(pos) << ")" << std::endl;
  }

  return 0;
}
