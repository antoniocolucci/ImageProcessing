/*
  HoughRette
  STEP:
  - Inizializzare l'accumulatore H.
  - Applicare l'algoritmo di Canny per individuare i punti di edge.
  - Per ogni punto (x,y) di edge
  - Per ogni angolo theta = 0:180 calcolare rho = x cos theta + y sin theta.
  - Incrementare H(rho,theta)=H(rho,thatea)+1.
  - Le celle H(rho,theta) con un valore maggiore della di soglia th corrispondono alle rette nell'immagine.
*/

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

    Mat edgeCanny;

    //Applico un Blurring Gaussiano per ripulire l'immagine da eventuali rumori in modo da ridurre il problema dei voti spuri, quindi delle celle che accumulano voti spuri.
    GaussianBlur(src, edgeCanny, Size(7, 7), 2, 2); //SigmaX e SigmaY sono l'ampiezza lungo la direzione x ed y della campana Gaussiana.

    //Applico l'algoritmo di Canny che restituirà i punti di edge all'interno dell'immagine.    
    Canny(edgeCanny, edgeCanny, 50, 150, 3);
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
    for (int x = 0; x < edgeCanny.rows; x++) 
    {
        for (int y = 0; y < edgeCanny.cols; y++) 
        {
            if (edgeCanny.at<uchar>(x, y) == 255) //Considero solo i pixel con valore 255, cioè i punti di edge (x,y) che voteranno nello spazio di voto.
            { 
                /*
                  Per votare bisognerà fare una scansione da 0 a 180° (siccome ogni pixel vota per tutte le infinite rette ma considero solo 180° (-90° + 90°)).
                  Il for cicla sugli angoli, sulle colonne della matrice di voto. Il ciclo for corrisponde ad una sinusoide.
                  Faccio (theta - 90) per rientrare nell'intervallo [-90;+90], dopodichè lo converto in radianti perchè le funzioni seno e coseno prendono come argomento l'angolo passato in radianti.
                */
                for (theta = 0; theta < 180; theta++) 
                {
                    rho = dist + y * cos((theta - 90) * CV_PI / 180) + x * sin((theta - 90) * CV_PI / 180);
                    if (rho >= 0)
                      votes.at<uchar>(rho, theta)++; //A partire da rho e theta vado a inserire il voto nella matrice di voto con il ++.
                }

            }
        }
    }

        /*
          Devo estrarre dallo spazio di voto le celle che hanno un numero di voti maggiore di una certa soglia.
          Le celle corrisponderanno ad una coppia(rho, theta) che utilizzerò per disegnare una retta all'interno dell'immagine.
        */

        for (int r = 0; r < votes.rows; r++) 
        {
            for (int t = 0; t < votes.cols; t++) 
            {
                if (votes.at<uchar>(r, t) >= 200) //Se il voto in posizione (rho,theta) è maggiore di 200 (cioè quella retta caratterizzata dai parametri (rho,theta) è stata votata 200 volte, allora è un possibile parametro della retta).
                { 
                    theta = (t - 90) * CV_PI / 180;
                    //Mi ricavo i parametri x e y come punto appartenente alla retta.
                    int x = (r-dist) * cos(theta);
                    int y = (r-dist) * sin(theta);
                    double sin_t = sin(theta);
                    double cos_t = cos(theta);
                    /*
                      Prendo i 2 estremi della retta.
                      Calcolo le coordinate nello spazio immagine dei 2 punti.
                    */
                    Point pt1(cvRound(x + dist * (-sin_t)), cvRound(y + dist * cos_t));
                    Point pt2(cvRound(x - dist * (-sin_t)), cvRound(y - dist * cos_t));
                    //Disegno la linea che passa per pt1 e pt2.
                    line(src, pt1, pt2, Scalar(255), 2, 8);
                }
            }
        }

    imshow("Output2", src);
    waitKey(0);

   return 0;
}

