/* Harris.cpp

  Algoritmo di Harris
  STEP:
  - Calcolare le derivate parziali rispetto ad x e y (Dx e Dy).
  - Calcolare Dx^2, Dy^2 e Dx*Dy.
  - Applicare un filtro gaussiano a Dx^2, Dy^2 e Dx*Dy.
  - Si ottengono C00, C11, C01 (e quindi anche C10).
  - Calcolare l'indice R.
  - Normalizzare l'indice R in [0,255].
  - Sogliare R.
*/

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void cornerHarris_demo(int, void*);

Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

const char* source_window = "Source image";
const char* corners_window = "Corners detected";


int main(int argc, char** argv) {

    //Lettura immagine    
    src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 7\\formegeom.Jpg", IMREAD_COLOR);

    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    namedWindow(source_window);
    createTrackbar("Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo);
    imshow(source_window, src);
    cornerHarris_demo(0, 0);
    
    waitKey(0);
    return 0;
}

void cornerHarris_demo(int, void*)
{
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    Mat dst = Mat::zeros(src.size(), CV_32FC1);
    cornerHarris(src_gray, dst, blockSize, apertureSize, k); //Funzione di Harris fornita da openCV.

    Mat dst_norm, dst_norm_scaled;
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs(dst_norm, dst_norm_scaled);

    for (int i = 0; i < dst_norm.rows; i++) {
        for (int j = 0; j < dst_norm.cols; j++) {
        
            if ((int)dst_norm.at<float>(i, j) > thresh)
            {
                circle(dst_norm_scaled, Point(j, i), 5, Scalar(0), 2, 8, 0);
            }
        }
    }

    namedWindow(corners_window);
    imshow(corners_window, dst_norm_scaled);
}
