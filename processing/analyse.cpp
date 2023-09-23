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
    resultMatrice = Mat(actualMatrice.size(), actualMatrice.type(), Scalar(0, 0, 0));
}

Pos Analyser::findBall(Mat& tActualMatrice) {
    actualMatrice = tActualMatrice;
    if (isInitialSearch) {
        cout << "initial search" << endl;
        return initialCalculation();
    }
    cout << "not initial search" << endl;
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
    cout << "We didn't find a ball: " << actualIndex << " " << searchPixelMaxSpacing << " " << searchPixelSpacing << endl;
    return NULL_POS;
}

Pos Analyser::calculateCenter(Pos position) {
    cout << "calculating center" << endl;
    maskMatrice.at<Vec3b>(position.x, position.y) = Vec3b(255, 0, 0);
    resultMatrice.at<Vec3b>(position.x, position.y) = Vec3b(255, 0, 0);
    vector<thread> threads;
    int totalX = position.x;
    int totalY = position.y;
    int number_of_pixel_in_range = 1;
    //TODO make it work when thread number is not 8
    for (int i = 0; i < number_of_threads; i++) {
        const int thread_number = i;
        //threads.emplace_back([&, thread_number]() {
            Area* area = nullptr;
            if (thread_number % 2 == 0) {
                area = new PairArea(thread_number/2, position);
            }
            else {
                area = new UnpairArea((thread_number+1)%8/2, position);
            }
            Pos position = area -> getNextPosition();
            while (position != NULL_POS) {
                cout << "Searching pos " << position << endl;
                if (pixelIsInHSVRange(actualMatrice, position)) {
                    number_of_pixel_in_range += 1;
                    totalX += position.x;
                    totalY += position.y;
                    maskMatrice.at<Vec3b>(position.x, position.y) = Vec3b(255, 255, 255);
                    resultMatrice.at<Vec3b>(position.x, position.y) = actualMatrice.at<Vec3b>(position.x, position.y);
                    imshow("mat", maskMatrice);
                    waitKey(1000);
                }
                else {
                    area->nextRaw();
                }
                position = area -> getNextPosition();
            }
            delete area;
            //return;
        //});
    }
    for (int i = 0; i < number_of_threads; i++) {
        threads[i].join();
    }
    Pos center = { static_cast<double>(totalX) / number_of_pixel_in_range,  static_cast<double>(totalY) / number_of_pixel_in_range };
    maskMatrice.at<Vec3b>(center.x, center.y) = Vec3b(0, 255, 0);
    resultMatrice.at<Vec3b>(center.x, center.y) = Vec3b(0, 255, 0);
    cout << "Caluclate Center is terminate : " << center << endl;
    return center;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
}

Pos Analyser::initialCalculation() {
    Mat mat(300, 1000, CV_8UC3, Scalar(0, 0, 0));
    waitKey(1000);
    for (int x = searchPixelSpacing - 1; x < width - searchPixelSpacing;  x += searchPixelSpacing) {
        for (int y = searchPixelSpacing -1; y < height - searchPixelSpacing; y += searchPixelSpacing) {
            Pos position = { x, y };
            cout << position << endl;
            if (pixelIsInHSVRange(actualMatrice, position)) {
                cout << "We find a ball point" << endl;
                mat.at<Vec3b>(y, x) = Vec3b(255, 255, 0);
                imshow("test", mat);
                waitKey(-1);
                return calculateCenter(position);
            }
        }
    }
    cerr << "No ball found" << endl;
    return NULL_POS;
}

Pos Analyser::getSearchPos() {
    int straightDecal = static_cast<int>((1 + actualIndex / 8) * searchPixelSpacing);
    int diagonalDecal = static_cast<int>(straightDecal * 0.7);
    return getSearchPos(straightDecal, diagonalDecal);
}

Pos Analyser::getSearchPos(int straightDecal, int diagonalDecal) {
    Pos position = getPreSearchPos(straightDecal, diagonalDecal);
    if (position.x > width) {
        if (position.y > height) {
            if (previous.y == position.y) {
                return NULL_POS;
            }
        }
        if (previous.x == position.x) {
            return NULL_POS;
        }
    }
    else if (position.y > height) {
        if (previous.y == position.y) {
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

Mat& Analyser::getResultMatrice() {
    return ref(resultMatrice);
}

Mat& Analyser::getMixedMatrice(float conversion) {
   /*
    Mat mixedMatrice = resultMatrice.clone();
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            mixedMatrice.at<Vec3b>(x, y) = actualMatrice.at<Vec3b>(x, y) * conversion;
        }
    }
    */
    return actualMatrice;
}


bool pixelIsInHSVRange(Mat& matrice, Pos position) {
    HSVColor& color = RGBtoHSV(ref(matrice.at<Vec3b>(position.y, position.x)));
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

HSVColor& RGBtoHSV(Vec3b& vector) {
    HSVColor color = { 0, 0, 0 };
    double r_normalized = vector[0] / 255.0;
    double g_normalized = vector[1] / 255.0;
    double b_normalized = vector[2] / 255.0;

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

    cout << color << " " << vector << endl;

    return ref(color);
}
