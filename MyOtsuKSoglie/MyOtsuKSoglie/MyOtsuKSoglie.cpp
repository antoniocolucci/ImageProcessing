#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void Otsu2(Mat& img, Mat& dest);

int main(int argc, char** argv) {

    //Lettura immagine    
    Mat img = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 9\\lena.png", IMREAD_GRAYSCALE);

    if (img.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat smooth;
    GaussianBlur(img, smooth, Size(3, 3), 0);

    //Applico Otsu con 2 soglie.
    Mat out;
    Otsu2(smooth, out);
    imshow("Otsu2", out);
    waitKey(0);
    return 0;
}


//Otsu con soglie multiple.
void Otsu2(Mat& img, Mat& dest)
{
    vector <double> his(256, 0.0f);
    vector <double> prob(3, 0.0f);
    vector <double> currMediaCum(3, 0.0f);
    vector <int> thresh(2, 0);
    double MN = img.rows * img.cols;
    double mediaGlobale = 0.0f;
    double currVar = 0.0f;
    double maxVar = 0.0f;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int val = img.at<uchar>(i, j);
            his[val]++;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        his[i] /= MN;
        mediaGlobale += i * his[i];
    }

    for (int i = 0; i < 256; i++)
    {
        prob[0] += his[i];
        currMediaCum[0] += i * his[i];

        for (int j = i + 1; j < 256; j++)
        {
            prob[1] += his[j];
            currMediaCum[1] += j * his[j];

            for (int k = j + 1; k < 256; k++)
            {
                prob[2] += his[k];
                currMediaCum[2] += k * his[k];
                
                currVar = 0.0f;

                for (int w = 0; w < 3; w++)
                {
                    currVar += prob[w] * pow(currMediaCum[w] / prob[w] - mediaGlobale, 2);
                }

                if (currVar > maxVar)
                {
                    maxVar = currVar;
                    thresh[0] = i;
                    thresh[1] = j;
                }
            }
            prob[2] = currMediaCum[2] = 0.0f;
        }
        prob[1] = currMediaCum[1] = 0.0f;
    }

    dest = Mat::zeros(img.size(), img.type());

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            uchar val = img.at<uchar>(i, j);
            if (val > thresh[1])
            {
                dest.at<uchar>(i, j) = 255;
            }
            else if (val > thresh[0] && val <= thresh[1])
            {
                dest.at<uchar>(i, j) = 127;
            }
        }
    }

}