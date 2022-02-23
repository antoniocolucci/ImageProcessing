// HoughRette

#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv) {


    //Lettura immagine    
    Mat src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 8\\autostrada.jfif", IMREAD_GRAYSCALE);

    if (src.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    //Applico un Blurring Gaussiano per ripulire l'immagine da eventuali rumori in modo da ridurre il problema dei voti spuri, quindi delle celle che accumulano voti spuri.
    GaussianBlur(src, src, Size(5, 5), 0, 0);

    //Applico l'algoritmo di Canny che restituirà i punti di edge all'interno dell'immagine.
    Mat edgeCanny;
    Canny(src, edgeCanny, 90, 160, 3);
    imshow("Output", edgeCanny);
    waitKey(0);

    //La distanza massima che ci può essere in un'immagine è la diagonale.
    int dist = hypot(src.rows, src.cols); //Per calcolare questa diagonale utilizzo la funzione "hypot" che calcola l'ipotenusa date le righe e le colonne. Dist sarebbe Rho.

    /*
      Creo lo spazio dei voti.
      Il mio spazio di voto sarà 2 volte rho per accomodare i valori +rho e -rho e i valori da 0 a 180° (andrò a considerare solo gli angoli a -90° a +90°).
    */

    Mat votes = Mat::zeros(dist * 2, 180, CV_8U); //Lo spazio dei voti sarà una matrice di tutti 0 che dovrà avere una dimensione tale da poter considerare tutte le possibili rette che passano per il piano immagine.
    double rho, theta;

    //Scansione dell'immagine di Canny. In Canny troveremo solo i punti di edge.
    for (int x = 0; x < edgeCanny.rows; x++) {
        for (int y = 0; y < edgeCanny.cols; y++) {
            if (edgeCanny.at<uchar>(x, y) == 255) { //Considero solo i pixel con valore 255, cioè i punti di edge (x,y) che voteranno nello spazio di voto.

                /*
                  Per votare bisognerà fare una scansione da 0 a 180° (siccome ogni pixel vota per tutte le infinite rette ma considero solo 180° (-90° + 90°)).
                  Il for cicla sugli angoli, sulle colonne della matrice di voto. Il ciclo for corrisponde ad una sinusoide.
                  Faccio (theta - 90) per rientrare nell'intervallo [-90;+90], dopodichè lo converto in radianti perchè le funzioni seno e coseno prendono come argomento l'angolo passato in radianti.
                */


                for (theta = 0; theta < 180; theta++) {
                    rho = dist + y * cos((theta - 90) * CV_PI / 180 + x * sin((theta - 90) * CV_PI / 180));
                    votes.at<uchar>(rho, theta)++; //A partire da rho e theta vado a inserire il voto nella matrice di voto con il ++.
                }
            }
        }

        /*
          Devo estrarre dallo spazio di voto le celle che hanno un numero di voti maggiore di una certa soglia.
          Le celle corrisponderanno ad una coppia(rho, theta) che utilizzerò per disegnare una retta all'interno dell'immagine.
        */

        for (int r = 0; r < edgeCanny.rows; r++) {
            for (int t = 0; t < edgeCanny.cols; t++) {
                if (votes.at<uchar>(r, t) >= 200) { //Se il voto in posizione (rho,theta) è maggiore di 200 (cioè quella retta caratterizzata dai parametri (rho,theta) è stata votata 200 volte, allora è una possibile retta).
                    theta = (t - 90) * CV_PI / 180;
                    //Mi ricavo i parametri x e y come punto appartenente alla retta.
                    int x = (r - dist) * cos(theta);
                    int y = (r - dist) * sin(theta);
                    double sin_t = sin(theta);
                    double cos_t = cos(theta);
                    /*
                      Prendo i 2 estremi della retta.
                      Calcolo le coordinate nello spazio immagine dei 2 punti.
                    */
                    Point pt1(cvRound(x + dist * (-sin_t)), cvRound(y + dist * cos_t));
                    Point pt2(cvRound(x - dist * (-sin_t)), cvRound(y - dist * cos_t));
                    //Disegno la linea che passa per pt1 e pt2.
                    line(src, pt1, pt2, Scalar(0), 2, 0);
                }
            }
        }

        imshow("Output", src);
        waitKey(0);

        return 0;
    }
}

