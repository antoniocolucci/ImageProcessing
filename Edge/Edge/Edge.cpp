// Edge.cpp


#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    int dim;

    //Lettura immagine    
    Mat image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 6\\building.png", IMREAD_GRAYSCALE);

    if (image.empty()) 
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    cout << "Inserire la dimensione del filtro: ";
    cin >> dim; //5

    Mat output;
    GaussianBlur(image, output, Size(dim, dim), 0, 0);

    //Con i filtri di Sobel ottengo le componenti x ed y del vettore gradiente.
    Mat Dx; //Dx e Dy sono le componenti per ogni pixel del vettore gradiente.
    Sobel(output, Dx, CV_32FC1, 0, 1);

    Mat Dy;
    Sobel(output, Dy, CV_32FC1, 1, 0);

    //Calcolo magnitudo
    Mat Mag = abs(Dx) + abs(Dy);
    normalize(Mag, Mag, 0, 255, NORM_MINMAX, CV_8U);

    Mat orientations;
    phase(Dx, Dy, orientations, true); //Calcolo l'arcotangente dei vettori gradienti in ogni posizione. Vengono calcolate le direzioni del vettore gradiente in ogni posizione, quindi l'angolo.
    normalize(orientations, orientations, 0, 255, NORM_MINMAX, CV_8U); //Phase, e quindi l'arcotangente, restituisce valori a -pigreco/2 e +pigreco/2, ma con la funzione "normalize" li riportiamo nell'intervallo 0-255.


    imshow("Original", image);
    imshow("Magnitudo", Mag);
    imshow("Orientations", orientations);
    waitKey(0);

    double th;
    cout << "Inserire il valore di threshold: "; //Serve per sogliare la magnitudo ed ottenere un'immagine più pulita.
    cin >> th; //75

    //Tutto ciò che supera il valore di threshold viene messo a 255 in modo che possiamo mostrare l'immagine.
    Mat Th;
    threshold(Mag, Th, th, 255, CV_THRESH_BINARY);

    imshow("Magnitudo threshold", Th);
    waitKey(0);

    return 0;
}

