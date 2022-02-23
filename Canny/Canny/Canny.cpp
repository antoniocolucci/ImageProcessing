/* 
  Algoritmo di Canny
  STEP:
  - Convoluzione dell'immagine di input con un filtro Gaussiano.
  - Calcolare la magnitudo e l'orientazione del gradiente.
  - Applicare la non maxima suppression
  - Applicare il thresholding con isteresi.
*/

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void CannyThreshold(int, void*);

//Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int num = 3;
int kernel_size = 3;
const char* window_name = "Edge Map";

int main(int argc, char** argv) {

    //Lettura immagine    
    Mat src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 7\\building.png", IMREAD_COLOR);

    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
  
    dst.create(src.size(), src.type());

    cvtColor(src, src_gray, CV_BGR2GRAY);

    namedWindow(window_name, CV_WINDOW_AUTOSIZE);

    /// Create a Trackbar for user to enter threshold
    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    /// Show the image
    CannyThreshold(0, 0);

    /// Wait until user exit program by pressing a key
    waitKey(0);

    return 0;
}


void CannyThreshold(int, void*)
{
    /// Reduce noise with a kernel 3x3
    blur(src_gray, detected_edges, Size(3, 3));

    /// Canny detector
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*num, kernel_size);

    cout << "Low: " << lowThreshold * num;

    /// Using Canny's output as a mask, we display our result
    //dst = Scalar::all(0);
    
    //src.copyTo(dst, detected_edges);
    imshow(window_name, detected_edges);
}




