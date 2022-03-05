//MyOtsu2


#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

vector<double> NormalizedHistogram(Mat img);
vector <int> Otsu2(vector<double> his);
void MultipleThreshold(Mat img, Mat& out, vector<int> thresh);

int main(int argc, char** argv) {

    //Lettura immagine    
    Mat img = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 9\\eye.jpeg", IMREAD_GRAYSCALE);

    if (img.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    GaussianBlur(img, img, Size(3, 3), 0);

    //Calcolo l'istogramma normalizzato.
    vector<double> hist = NormalizedHistogram(img);

    //Applico Otsu con 2 soglie.
    Mat out;
    vector<int> thresh = Otsu2(hist);
    MultipleThreshold(img, out, thresh);
    imshow("Otsu2", out);
    waitKey(0);
    return 0;

}

/*
  L'istogramma normalizzato consiste nel contare il numero di pixel che assumono un particolare livello di intentistà
  e dividerlo per il numero di righe moltiplicato il numero di colonne.
 */
vector<double> NormalizedHistogram(Mat img)
{
    //Creo un vettore di 256 elementi (livelli di intensità) e li inizializzo a 0.
    vector <double> his(256, 0.0f);

    for (int x = 0; x < img.rows; x++)
    {
        for (int y = 0; y < img.cols; y++)
        {
            his[img.at<uchar>(y, x)]++;
            for (int i = 0; i < 256; i++)
            {
                his[i] /= img.rows * img.cols; //Divisione tra l'elemento dell'istogramma per il numero di pixel. (ni/M*N)
            }
        }
    }
    return his;
}

//Otsu con soglie multiple.
vector <int> Otsu2(vector<double> his)
{
    double globalMean = 0.0f;
    double currVar = 0.0f;
    double maxVar = 0.0f;
    vector<int> thresh(2, 0);
    vector<double> prob(3, 0.0f);
    vector<double> currMediaCum(3, 0.0f);
    vector<double> mean(3, 0.0f);

    //Calcolo della media cumulativa globale.
    for (int i = 0; i < 256; i++)
    {
        globalMean += i * his[i];
    }

    //Con 2 soglie vengono identificati 3 intervalli, per cui dovrò calcolare i valori P1, P2, e P3 nelle 3 classi (3 intervalli di intensità).
    for (int i = 0; i < 256 - 2; i++) //Primo intervallo (P1)
    {
        prob[0] += his[i];
        currMediaCum[0] += i * his[i];
        mean[0] = currMediaCum[0] / prob[0];

        for (int j = i + 1; j < 256 - 1; j++) //Secondo intervallo (P2)
        {
            prob[1] += his[j];
            currMediaCum[1] += i * his[j];
            mean[1] = currMediaCum[1] / prob[1];

            for (int k = j + 1; k < 256; k++) //Terzo intervallo (P3)
            {
                prob[2] += his[k];
                currMediaCum[2] += i * his[k];
                mean[2] = currMediaCum[2] / prob[2];

                //double variance = prob[0] * (currMediaCum[0] - globalMean) * (currMediaCum[0] - globalMean) + prob[1] * (currMediaCum[1] - globalMean) * (currMediaCum[1] - globalMean) + prob[2] * (currMediaCum[2] - globalMean) * (currMediaCum[2] - globalMean);
                double variance = 0.0f;
                for (int w = 0; w < 3; w++)
                 {
                        currVar += prob[w] * pow(currMediaCum[w] / prob[w] - globalMean, 2);
                }

                if (currVar > maxVar) //Salvo la varianza interclasse massima in corrispondenza delle 2 soglie i e j.
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
    return thresh;
}

//La funzione assegna dei livelli di intensità ai pixel in base all'intervallo a cui appartengono.
void MultipleThreshold(Mat img, Mat &out, vector<int> thresh)
{
    out = Mat::zeros(img.size(), img.type());
    for (int y = 0; y < img.rows; y++) 
    {
        for (int x = 0; x < img.cols; x++) 
        {
             if (img.at<uchar>(y, x) >= thresh[1])
                    out.at<uchar>(y, x) = 255;
                else if (img.at<uchar>(y, x) >= thresh[0] && img.at<uchar>(y, x) <= thresh[1]);
                out.at<uchar>(y, x) = 127;
        }
    }
}
