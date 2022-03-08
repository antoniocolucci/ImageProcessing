/*
  Algoritmo di Canny
  STEP:
  - Convoluzione dell'immagine di input con un filtro Gaussiano.
  - Calcolare la magnitudo e l'orientazione del gradiente.
  - Applicare la non maxima suppression.
  - Applicare il thresholding con isteresi. 
*/

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

static void myCanny(Mat& src, Mat& output, int kernel_size);
static void nonMaxSuppression(Mat& mag, Mat& orientations, Mat& nms);
void thresholding(Mat& img, Mat& out, int lth, int hth);

const int kernel_size = 3;

int main(int argc, char** argv) {

    //Lettura immagine    
    Mat image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 7\\building.png", IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output;
    myCanny(image, output, kernel_size);
    imshow("Canny", output);
    waitKey(0);

    return 0;
}


static void myCanny(Mat& src, Mat& output, int kernel_size)
{
    Mat Gauss;
    GaussianBlur(src, Gauss, Size(5, 5), 0, 0); //Applico la gaussiana.
    
    //Applico i filtri di Sobel per ottenere le due immagini in cui ho le due componenti del vettore gradiente.
    Mat Dx;
    Sobel(Gauss, Dx, CV_32FC1, 1, 0, kernel_size);

    Mat Dy;
    Sobel(Gauss, Dy, CV_32FC1, 0, 1, kernel_size);

    //Calcolo della magnitudo M(radice quadrata della somma delle componenti al quadrato oppure con la somma dei valori assoluti delle 2 componenti).
    Mat mag, Dx2, Dy2;
    pow(Dx, 2, Dx2);
    pow(Dy, 2, Dy2);
    sqrt(Dx2 + Dy2, mag);
    normalize(mag, mag, 0, 255, NORM_MINMAX, CV_8U);

    //Calcolo dell'orientazione.
    Mat orientations;
    phase(Dx, Dy, orientations, true);

    Mat nms = Mat::zeros(mag.rows, mag.cols, CV_8U);
    nonMaxSuppression(mag, orientations, nms);

    Mat out = Mat::zeros(mag.rows, mag.cols, CV_8U);
    thresholding(nms, out, 35, 85);

    //output = out;
}

static void nonMaxSuppression(Mat& mag, Mat& orientations, Mat& nms)
{
    for (int i = 1; i < mag.rows - 1; i++)
    {
        for (int j = 1; j < mag.cols - 1; j++)
        {
            float angle = orientations.at<float>(i, j);
            angle = angle > 180 ? angle - 360 : angle;

            //Analizzo i 4 intervalli di angoli.
            //Verifico se il pixel centrale mag.at(i,j) è maggiore dei suoi vicini in quella direzione.
            if ((-22.5 < angle) && (angle <= 22.5) || (-157.5 < angle) && (angle <= 157.5)) {
                if (mag.at<uchar>(i, j) >= mag.at<uchar>(i, j - 1) && mag.at<uchar>(i, j) >= mag.at<uchar>(i, j + 1)) //Direzione verticale.
                    nms.at<uchar>(i, j) = mag.at<uchar>(i, j);
            }
            else if ((-67.5 < angle) && (angle <= -22.5) || (112.5 < angle) && (angle <= 157.5)) {
                if (mag.at<uchar>(i, j) >= mag.at<uchar>(i - 1, j - 1) && mag.at<uchar>(i, j) >= mag.at<uchar>(i + 1, j + 1))
                    nms.at<uchar>(i, j) = mag.at<uchar>(i, j);
            }
            else if ((-112.5 < angle) && (angle <= -67.5) || (67.5 < angle) && (angle <= 112.5)) {
                if (mag.at<uchar>(i, j) >= mag.at<uchar>(i - 1, j) && mag.at<uchar>(i, j) >= mag.at<uchar>(i + 1, j)) //Direzione orizzontale.
                    nms.at<uchar>(i, j) = mag.at<uchar>(i, j);
            }
            else if ((-157.5 < angle) && (angle <= -112.5) || (22.5 < angle) && (angle <= 67.5)) {
                if (mag.at<uchar>(i, j) >= mag.at<uchar>(i + 1, j - 1) && mag.at<uchar>(i, j) >= mag.at<uchar>(i + 1, j + 1))
                    nms.at<uchar>(i, j) = mag.at<uchar>(i, j);
            }
        }
    }
    imshow("Nms", nms);
    imshow("Angle", orientations);
    imshow("Mag", mag);
    waitKey(0);
}

void thresholding(Mat& img, Mat& out, int lth, int hth)
{
    for (int i = 1; i < img.rows - 1; i++)
    {
        for (int j = 1; j < img.cols - 1; j++)
        {
            if (img.at<uchar>(i, j) > hth) //Verifico se il pixel è maggiore della soglia alta. 
            {
                for (int u = -1; u <= 1; u++) //Intorno 3x3 del pixel.
                {
                    for (int v = -1; v <= 1; v++)
                    {
                        if (img.at<uchar>(i + u, j + v) > lth && img.at<uchar>(i + u, j + v) < hth) //Se il pixel è maggiore della soglia alta verifico se nel suo intorno c'è un pixel maggiore della soglia bassa.
                            out.at<uchar>(i + u, j + v) = 255; //Lo promuovo a pixel forte.
                    }
                }
            }
            else if (img.at<uchar>(i, j) < lth)
                out.at<uchar>(i, j) = 0; //Altrimenti lo lascio a 0.
        }
    }
}
