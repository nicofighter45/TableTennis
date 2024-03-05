#include <opencv2/opencv.hpp>
#include "analyse.hpp"
#include "configuration.hpp"
#include "area.hpp"

using namespace cv;
using namespace std;

Analyser::Analyser(Mat& tActualMatrice)
    : actualMatrice(tActualMatrice) {
    isInitialSearch = true;
    actualIndex = 0;
    previous = center;
    maskMatrice = Mat(actualMatrice.size(), actualMatrice.type(), Scalar(0, 0, 0));
}

Pos Analyser::findBall() {
    actualIndex = 0;
    maskMatrice = Mat(actualMatrice.size(), actualMatrice.type(), Scalar(0, 0, 0));
    if(reloadFromCamera != NULL_POS){
        reloadFromCamera.x = static_cast<int>(reloadFromCamera.x);
        reloadFromCamera.y = static_cast<int>(reloadFromCamera.y);
        return calculateCenter(reloadFromCamera);
    }
    if (isInitialSearch) {
        return initialCalculation();
    }
    center.x = static_cast<int>(center.x);
    center.y = static_cast<int>(center.y);
    while (actualIndex != 8 * searchPixelMaxSpacing / searchPixelSpacing) {
        Pos position = getSearchPos();
        if (position == NULL_POS) {
            break;
        }
        maskMatrice.at<Vec3b>(position.x, position.y) = black;
        if (pixelIsInHSVRange(actualMatrice, position)) {
            isInitialSearch = false;
            return calculateCenter(position);
        }
    }
    return initialCalculation();
}

Pos Analyser::calculateCenter(Pos position) {
    vector<thread> threads;
    int totalX = position.x;
    int totalY = position.y;
    int number_of_pixel_in_range = 1;
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
                    maskMatrice.at<Vec3b>(position.x, position.y) = colors[thread_number];
                }
                else {
                    maskMatrice.at<Vec3b>(position.x, position.y) = white;
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
    if (number_of_pixel_in_range < minimumPixelNeeded) {
        return NULL_POS;
    }
    center = { static_cast<double>(totalX) / number_of_pixel_in_range,  static_cast<double>(totalY) / number_of_pixel_in_range };
    if (center != NULL_POS) {
        isInitialSearch = false;
    }
    return center;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
}

Pos Analyser::initialCalculation() {
    Pos first = NULL_POS;
    Mat mat(height, width, CV_8UC3, white);
    for (int x = searchPixelSpacing - 1 + roi.y; x < roi.y + roi.height - searchPixelSpacing;  x += searchPixelSpacing) {
        for (int y = searchPixelSpacing - 1 + roi.x; y < roi.x + roi.width - searchPixelSpacing; y += searchPixelSpacing) {
            Pos position = { x, y};
            maskMatrice.at<Vec3b>(x, y) = white;
            if (first != NULL_POS) {
                return calculateCenter(position);
            }
            if (pixelIsInHSVRange(actualMatrice, position)) {
                first = position;
            }
        }
    }
    return NULL_POS;
}

Pos Analyser::getSearchPos() {
    int straightDecal = static_cast<int>(searchPixelSpacing);
    int diagonalDecal = static_cast<int>(straightDecal * 0.7);
    return getSearchPos(straightDecal, diagonalDecal);
}

Pos Analyser::getSearchPos(int straightDecal, int diagonalDecal) {
    Pos position = getPreSearchPos(straightDecal, diagonalDecal);
    if (position.x < roi.y || position.y < roi.x || position.y >= roi.x + roi.width || position.x >= roi.y + roi.height) {
        return NULL_POS;
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

Mat& Analyser::getMatriceWithCenter() {
    addCubeToImage(ref(actualMatrice), center, 1, black);
    return ref(actualMatrice);
}

Mat& Analyser::getMixedMatrice(float conversion) {
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            Vec3b maskPixel = maskMatrice.at<Vec3b>(x, y);
            if (maskPixel != Vec3b(0, 0, 0)) {
                Vec3b& mixedPixel = actualMatrice.at<Vec3b>(x, y);
                mixedPixel[0] = static_cast<int>(mixedPixel[0] * conversion + (1 - conversion) * maskPixel[0]);
                mixedPixel[1] = static_cast<int>(mixedPixel[1] * conversion + (1 - conversion) * maskPixel[1]);
                mixedPixel[2] = static_cast<int>(mixedPixel[2] * conversion + (1 - conversion) * maskPixel[2]);
            }
        }
    }
    if (center != NULL_POS) {
        addCubeToImage(ref(actualMatrice), center, 1, black);
    }
    return ref(actualMatrice);
}

Vec3b reducePixelStrength(const Vec3b& originalPixel, float conversion) {
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
    int startX = std::max(0, static_cast<int>(position.x) - size);
    int startY = std::max(0, static_cast<int>(position.y) - size);
    int endX = std::min(height, static_cast<int>(position.x) + size);
    int endY = std::min(width, static_cast<int>(position.y) + size);

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            matrice.at<Vec3b>(x, y) = color;
        }
    }
}

void Analyser::setIsInitialSearch(bool state) {
    isInitialSearch = state;
}