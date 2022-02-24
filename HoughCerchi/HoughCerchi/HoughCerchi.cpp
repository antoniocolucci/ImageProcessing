// HoughCerchi

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {


    //Lettura immagine    
    Mat src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 8\\monete.jpg", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    GaussianBlur(src, src, Size(3, 3), 0, 0);

    Mat edgeCanny;
    //Canny(src, edgeCanny, 150, 230, 3);
    Canny(src, edgeCanny, 70, 180, 3);
    //imshow("Output", edgeCanny);
    //waitKey(0);

    int r_min = 40;
    int r_max = 80;
    int sizes[] = { edgeCanny.rows, edgeCanny.cols, r_max - r_min + 1 };  //r_max ed r_min stabiliscono l'intervallo di valori che voglio considerare sulla terza dimensione.
    
    //Creo lo spazio di voto tridimensionale di tutti 0.
    Mat votes = Mat(3, sizes, CV_8U, Scalar(0)); 

    //Scansione dell'immagine di Canny.
    for (int x = 0; x < edgeCanny.rows; x++) {
        for (int y = 0; y < edgeCanny.cols; y++) {
            if (edgeCanny.at<uchar>(x, y) == 255)
            {
                for (int r = r_min; r <= r_max; r++) //I pixel di edge voteranno per ogni raggio da r_min ad r_max
                {
                    for (int theta = 0; theta < 360; theta++)
                    {
                        //Calcolo le coordinate del centro (a,b).
                        int a = y - r * cos(theta * CV_PI / 180);
                        int b = x - r * sin(theta * CV_PI / 180);

                        //Controllo se le coordinate del centro sono interne all'immagine.
                        if (a >= 0 && a < edgeCanny.cols && b >= 0 && b < edgeCanny.rows)
                            votes.at<uchar>(b, a, r - r_min)++;
                    }
                }
            }
        }
    }

    
    //Analizzo lo spazio di voto.
    for (int r = r_min; r <= r_max; r++) //Ciclo per ogni raggio.
    {
        for (int b = 0; b < edgeCanny.rows; b++) //Ciclo sulle dimensioni dello spazio di voto (a e b).
        {
            for (int a = 0; a < edgeCanny.cols; a++)
            {
                if (votes.at<uchar>(b, a, r - r_min) > 160) //Verifico se la cella ha un numero di valori maggiori di una soglia (140).
                {
                    circle(src, Point(a, b), 3, Scalar(0), 2, 8, 0); //Cerchio piccolo per visualizzare il centro.
                    circle(src, Point(a, b), r, Scalar(0), 2, 8, 0); //Cerchio più grande di centro (a,b) e raggio r.
                }
            }
        }
    }

    imshow("HoughCerchi", src);
    waitKey(0);

    return 0;
}

