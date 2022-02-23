/*  HarrisFun.cpp

    Algoritmo di Harris
    STEP :
    -Calcolare le derivate parziali rispetto ad x e y(Dx e Dy).
    - Calcolare Dx^2, Dy^2 e Dx * Dy.
    - Applicare un filtro gaussiano a Dx^2, Dy^2 e Dx * Dy.
    - Si ottengono C00, C11, C01(e quindi anche C10).
    - Calcolare l'indice R.
    - Normalizzare l'indice R in [0,255].
    - Sogliare R.
*/

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

static void myHarris(Mat& src, Mat& output, int kernel_size, float k, int thresh);


int main(int argc, char** argv) {

    //Lettura immagine    
    Mat image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 7\\formegeom.Jpg", IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output;
    myHarris(image, output, 3, 0.04, 100);

    imshow("Harris", output);
    waitKey(0);

    return 0;
}

static void myHarris(Mat& src, Mat& output, int kernel_size, float k, int thresh)
{
    //Applico Sobel su entrambe le direzioni.
    Mat Dx, Dy;
    Sobel(src, Dx, CV_32FC1, 1, 0, kernel_size, BORDER_DEFAULT);
    Sobel(src, Dy, CV_32FC1, 0, 1, kernel_size, BORDER_DEFAULT);

    Mat Dx2, Dy2, Dxy;
    pow(Dx, 2.0, Dx2); //Calcolo Dx^2.
    pow(Dy, 2.0, Dy2); //Calcolo Dy^2.
    multiply(Dx, Dy, Dxy); //Calcolo Dx * Dy.

    Mat Dx2g, Dy2g, Dxyg;
    GaussianBlur(Dx2, Dx2g, Size(7, 7), 2.0, 0.0, BORDER_DEFAULT); //C00
    GaussianBlur(Dy2, Dy2g, Size(7, 7), 0.0, 2.0, BORDER_DEFAULT); //C11
    GaussianBlur(Dxy, Dxyg, Size(7, 7), 2.0, 2.0, BORDER_DEFAULT); //C01

    Mat x2y2, xy, mtrace, R;
    multiply(Dx2g, Dy2g, x2y2); //C00 * C11
    multiply(Dxyg, Dxyg, xy); //C10 * C01
    pow((Dx2g + Dy2g), 2.0, mtrace);
    R = (x2y2 - xy) - k * mtrace; //k solitamente è un valore basso (0.04 nel nostro caso). E' il contributo della traccia al calcolo finale di R (quantità di traccia che sottraggo al determinante).

    normalize(R, R, 0, 255, NORM_MINMAX, CV_32FC1);
    output = src.clone();

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if ((int)R.at<float>(i, j) > thresh)
            {
                circle(output, Point(j, i), 5, Scalar(0), 2, 8, 0);
            }
        }
    }
}

