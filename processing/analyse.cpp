#include <opencv2/opencv.hpp>
#include "analyse.hpp"
#include "configuration.hpp"
#include "area.hpp"

using namespace cv;
using namespace std;

Analyser::Analyser(Mat& tActualMatrice, Rect tROI)
    : actualMatrice(tActualMatrice), ROI(tROI) {
    isInitialSearch = true;
    actualIndex = 0;
    previous = center;
    maskMatrice = Mat(actualMatrice.size(), actualMatrice.type(), Scalar(0, 0, 0));
}

Pos Analyser::findBall(Mat& tActualMatrice) {
    actualMatrice = tActualMatrice;
    if (isInitialSearch) {
        return initialCalculation();
    }
    while (actualIndex != 8 * searchPixelMaxSpacing / searchPixelSpacing) {
        Pos position = getSearchPos();
        if (position == NULL_POS) {
            continue;
        }
        if (pixelIsInHSVRange(actualMatrice, position)) {
            isInitialSearch = false;
            return calculateCenter(position);
        }
    }
    if (!isInitialSearch) {
        return initialCalculation();
    }
    isInitialSearch = true;
    return NULL_POS;
}

Pos Analyser::calculateCenter(Pos position) {
    maskMatrice.at<Vec3b>(position.x, position.y) = Vec3b(0, 255, 0);
    vector<thread> threads;
    int totalX = position.x;
    int totalY = position.y;
    int number_of_pixel_in_range = 1;
    //TODO make it work when thread number is not 8
    for (int i = 0; i < number_of_threads; i++) {
        const int thread_number = i;
        threads.emplace_back([&, thread_number]() {
            Area* area = nullptr;
            if (thread_number % 2 == 0) {
                area = new PairArea(thread_number/2, position);
            }
            else {
                area = new UnpairArea((thread_number-1)/2, position);
            }
            Pos position = area -> getNextPosition();
            while (position != NULL_POS) {
                if (pixelIsInHSVRange(actualMatrice, position)) {
                    number_of_pixel_in_range += 1;
                    totalX += position.x;
                    totalY += position.y;
                    maskMatrice.at<Vec3b>(position.x, position.y) = Vec3b(255, 255, 255);
                }
                else {
                    maskMatrice.at<Vec3b>(position.x, position.y) = Vec3b(255, 0, 0);
                    area -> nextRaw();
                }
                position = area -> getNextPosition();
            }
            delete area;
        });
    }
    for (int i = 0; i < number_of_threads; i++) {
        threads[i].join();
    }
    center = { static_cast<double>(totalX) / number_of_pixel_in_range,  static_cast<double>(totalY) / number_of_pixel_in_range };
    addCubeToImage(ref(maskMatrice), center, 2, Vec3b(0, 0, 255));
    return center;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
}

Pos Analyser::initialCalculation() {
    Mat mat(height, width, CV_8UC3, Scalar(0, 0, 0));
    for (int x = searchPixelSpacing - 1; x < height - searchPixelSpacing;  x += searchPixelSpacing) {
        for (int y = searchPixelSpacing - 1; y < width - searchPixelSpacing; y += searchPixelSpacing) {
            Pos position = { x, y};
            if (pixelIsInHSVRange(actualMatrice, position)) {
                return calculateCenter(position);
            }
        }
    }
    cerr << "No ball found" << endl;
    waitKey(-1);
    return NULL_POS;
}

Pos Analyser::getSearchPos() {
    int straightDecal = static_cast<int>((1 + actualIndex / 8) * searchPixelSpacing);
    int diagonalDecal = static_cast<int>(straightDecal * 0.7);
    return getSearchPos(straightDecal, diagonalDecal);
}

Pos Analyser::getSearchPos(int straightDecal, int diagonalDecal) {
    Pos position = getPreSearchPos(straightDecal, diagonalDecal);
    if (position.y > width) {
        if (position.x > height) {
            if (previous.x == position.x) {
                return NULL_POS;
            }
        }
        if (previous.y == position.y) {
            return NULL_POS;
        }
    }
    else if (position.x > height) {
        if (previous.x == position.x) {
            return NULL_POS;
        }
    }
    actualIndex++;
    return position;
}

Pos Analyser::getPreSearchPos(int straightDecal, int diagonalDecal) {
    switch (actualIndex % 8) {
    case 0:
        return Pos{ center.x + straightDecal, center.y };
    case 1:
        return Pos{ center.x + diagonalDecal, center.y + diagonalDecal};
    case 2:
        return Pos{ center.x, center.y + straightDecal };
    case 3:
        return Pos{ center.x - diagonalDecal, center.y + diagonalDecal };
    case 4:
        return Pos{ center.x - straightDecal, center.y };
    case 5:
        return Pos{ center.x - diagonalDecal, center.y - diagonalDecal };
    case 6:
        return Pos{ center.x, center.y - straightDecal };
    case 7:
        return Pos{ center.x + diagonalDecal, center.y - diagonalDecal };
    }
}

Mat& Analyser::getMaskMatrice() {
    return ref(maskMatrice);
}

Mat Analyser::getMixedMatrice(float conversion) {
    Mat mixedMatrice = Mat(actualMatrice.size(), actualMatrice.type(), Scalar(0, 0, 0));
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            Vec3b originalPixel = actualMatrice.at<Vec3b>(x, y);
            Vec3b maskPixel = maskMatrice.at<Vec3b>(x, y);
            Vec3b black(0, 0, 0);
            if (maskPixel == black) {
                mixedMatrice.at<Vec3b>(x, y) = reducePixelStrength(ref(originalPixel), conversion);
            }
            else {
                mixedMatrice.at<Vec3b>(x, y) = actualMatrice.at<Vec3b>(x, y);
            }
        }
    }
    if (center != NULL_POS) {
        addCubeToImage(ref(mixedMatrice), center, 2, Vec3b(0, 0, 255));
    }
    return mixedMatrice;
}

Vec3b reducePixelStrength(Vec3b& originalPixel, float conversion) {
    return Vec3b(static_cast<int>(originalPixel[0] * conversion), 
        static_cast<int>(originalPixel[1] * conversion),
        static_cast<int>(originalPixel[2] * conversion));
}


bool pixelIsInHSVRange(Mat& matrice, Pos position) {
    HSVColor color = RGBtoHSV(ref(matrice.at<Vec3b>(position.x, position.y)));
    if (color.H > upper_color.H) {
        return false;
    }
    if (color.H < lower_color.H) {
        return false;
    }
    if (color.S > upper_color.S) {
        return false;
    }
    if (color.S < lower_color.S) {
        return false;
    }
    if (color.V > upper_color.V) {
        return false;
    }
    if (color.V < lower_color.V) {
        return false;
    }
    return true;
}

HSVColor RGBtoHSV(Vec3b& vector) {
    HSVColor color = { 0, 0, 0 };
    double r_normalized = vector[2] / 255.0;
    double g_normalized = vector[1] / 255.0;
    double b_normalized = vector[0] / 255.0;

    double cmax = std::max({ r_normalized, g_normalized, b_normalized });
    color.V = static_cast<int>(cmax * 255);

    double cmin = std::min({ r_normalized, g_normalized, b_normalized });
    double delta = cmax - cmin;

    if (delta != 0) {
        if (cmax == r_normalized) {
            color.H = static_cast<int>(60 * (g_normalized - b_normalized) / delta) %360;
            if (color.H < 0) {
                color.H += 360;
            }
        }
        else if (cmax == g_normalized) {
            color.H = static_cast<int>(60 * (((b_normalized - r_normalized) / delta) + 2));
        }
        else if (cmax == b_normalized) {
            color.H = static_cast<int>(60 * (((r_normalized - g_normalized) / delta) + 4));
        }
        else {
            cerr << "Calcul of HSV failed" << endl;
        }
    }

    if (cmax != 0) {
        color.S = static_cast<int>((delta / cmax) * 255);
    }
    return color;
}

void addCubeToImage(Mat& matrice, Pos position, int size, Vec3b color) {
    for (int x = 0; x < 2*size; x++) {
        if (position.x - size + x < 0 || position.x - size + x >= height) {
            continue;
        }
        for (int y = 0; y < 2 * size; y++) {
            if (position.y - size + y < 0 || position.y - size + y >= width) {
                continue;
            }
            matrice.at<Vec3b>(x + static_cast<int>(position.x) - size, y + static_cast<int>(position.y) - size) = color;
        }
    }
}
