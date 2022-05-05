// MyRettePreExam

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void myRette(Mat& src, Mat& out);

int main()
{
    //Lettura immagine    
    Mat src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 8\\autostrada.jfif", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output;
    myRette(src, output);
    imshow("Output", output);
    waitKey(0);

    return 0;
}

void myRette(Mat& src, Mat& out)
{
    Mat edgeCanny;

    GaussianBlur(src, src, Size(7, 7), 2, 2);

    Canny(src, edgeCanny, 50, 150, 3);

    int dist = hypot(src.rows, src.cols);
    Mat votes = Mat::zeros(dist * 2, 180, CV_8U);

    double rho, theta;

    for (int x = 0; x < edgeCanny.rows; x++)
    {
        for (int y = 0; y < edgeCanny.cols; y++)
        {
            if (edgeCanny.at<uchar>(x, y) == 255)
            {
                for (theta = 0; theta < 180; theta++)
                {
                    rho = dist + y * cos((theta - 90) * CV_PI / 180) + x * sin((theta - 90) * CV_PI / 180);
                    votes.at<uchar>(rho, theta)++;
                }
            }
        }
    }

    out = src.clone();

    for (int r = 0; r < votes.rows; r++)
    {
        for (int t = 0; t < votes.cols; t++)
        {
            if (votes.at<uchar>(r, t) >= 200)
            {
                theta = (t - 90) * CV_PI / 180;
                int x = (r - dist) * cos(theta);
                int y = (r - dist) * sin(theta);
                double sin_t = sin(theta);
                double cos_t = cos(theta);

                Point p1(cvRound(x + dist * (-sin_t)), cvRound(y + dist * cos_t));
                Point p2(cvRound(x - dist * (-sin_t)), cvRound(y - dist * cos_t));
                line(out, p1, p2, Scalar(255), 2, 8);
            }
        }
    }
}


