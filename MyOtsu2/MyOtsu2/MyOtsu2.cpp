//MyOtsu2

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void Otsu2(Mat& img, Mat& dest);

int main(int argc, char** argv) {

    //Lettura immagine    
    Mat img = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 9\\eye.jpeg", IMREAD_GRAYSCALE);

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
    double histogram[256] = { 0 };
    double normHistogram[256] = { 0 };
    double MN = img.rows * img.cols;
    double mediaGlobale = 0;

    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            int val = img.at<uchar>(i, j);
            histogram[val]++;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        normHistogram[i] = histogram[i] / MN;
        mediaGlobale += i * normHistogram[i];
    }
    
    double maxVarianza = 0;
    int thresh1 = 0, thresh2 = 0;
    double P1 = 0, P2 = 0, P3 = 0, currMeanP1 = 0, currMeanP2 = 0, currMeanP3 = 0, m1 = 0, m2 = 0, m3 = 0;

    //Con 2 soglie vengono identificati 3 intervalli, per cui dovrò calcolare i valori P1, P2, e P3 nelle 3 classi (3 intervalli di intensità).
    for (int i = 0; i < 256; i++) //Primo intervallo (P1)
    {
        P1 += normHistogram[i];
        currMeanP1 += i * normHistogram[i];
        m1 = currMeanP1 / P1;

        for (int j = i + 1; j < 256; j++) //Secondo intervallo (P2)
        {
            P2 += normHistogram[j];
            currMeanP2 += j * normHistogram[j];
            m2 = currMeanP2 / P2;

            P3 = 1 - P2 - P1;
            currMeanP3 = mediaGlobale - currMeanP2 - currMeanP1;
            m3 = currMeanP3 / P3;

            double variance = P1 * pow((m1 - mediaGlobale), 2) + P2 * pow((m2 - mediaGlobale), 2) + P3 * pow((m3 - mediaGlobale), 2);
         
           if (variance > maxVarianza) //Salvo la varianza interclasse massima in corrispondenza delle 2 soglie i e j.
           {
                    maxVarianza = variance;
                    thresh1 = i;
                    thresh2 = j;
           }
        }         
    }

    dest = Mat::zeros(img.rows, img.cols, CV_8UC1);
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            uchar val = img.at<uchar>(i, j);
            if (val > thresh2)
            {
                dest.at<uchar>(i, j) = 255;
            }
            else if (val > thresh1 && val <= thresh2)
            {
                dest.at<uchar>(i, j) = 128;
            }
        }
    }
}