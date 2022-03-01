// Esercizio 1: Realizzare una funzione che effettui il padding di un'immagine.

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat PaddingGray(Mat& image, int top, int bottom, int left, int right);
Mat PaddingRGB(Mat& image, int top, int bottom, int left, int right);

int main(int agrc, char** argv)
{

    Mat imageGray = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 2\\girasole.jpg", IMREAD_GRAYSCALE); //immagine in scala di grigi.
    Mat imageRGB = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 2\\girasole.jpg", IMREAD_COLOR); //immagine a colori.

    int top, bottom, left, right;

    cout << "Digitare il numero dei pixel del bordo superiore: ";
    cin >> top;

    cout << "Digitare il numero dei pixel del bordo inferiore: ";
    cin >> bottom;

    cout << "Digitare il numero dei pixel del bordo sinistro: ";
    cin >> left;

    cout << "Digitare il numero dei pixel del bordo destro: ";
    cin >> right;

    cout << "Numero di righe:" << imageGray.rows << endl;
    cout << "Numero di colonne:" << imageGray.cols << endl;

    Mat padGray = PaddingGray(imageGray, top, bottom, left, right);
    Mat padRGB = PaddingRGB(imageRGB, top, bottom, left, right);

    return 0;
}

Mat PaddingGray(Mat& image, int top, int bottom, int left, int right)
{
    int righe = image.rows + top + bottom;
    int colonne = image.cols + left + right;

    Mat Pad(righe, colonne, CV_8UC1, Scalar(0)); //creo un oggetto di tipo Mat.

    cout << "Numero di righe + pad: " << righe << endl;
    cout << "Numero di colonne + pad: " << colonne << endl;

    cout << "image.rows+top: " << image.rows + top << endl;

    for (int i = top; i < image.rows + top; i++)
        for (int j = left; j < image.cols + left; j++)
            Pad.at<unsigned char>(i, j) = image.at<unsigned char>(i - top, j - left); //copia ogni singolo elemento.

    imshow("Immagine iniziale (Gray)", image);
    waitKey(0);

    imshow("Padding border constant (Gray)", Pad);
    waitKey(0);

    return Pad;
}

Mat PaddingRGB(Mat& image, int top, int bottom, int left, int right)
{
    int righe = image.rows + top + bottom;
    int colonne = image.cols + left + right;

    Mat PadRGB(righe, colonne, CV_8UC3, Scalar(0, 0, 0)); //creo un oggetto di tipo Mat.

    for (int i = top; i < image.rows + top; i++)
        for (int j = left; j < image.cols + left; j++) {
            PadRGB.at<Vec3b>(i, j)[0] = image.at<Vec3b>(i - top, j - left)[0]; //BLU
            PadRGB.at<Vec3b>(i, j)[1] = image.at<Vec3b>(i - top, j - left)[1]; //VERDE
            PadRGB.at<Vec3b>(i, j)[2] = image.at<Vec3b>(i - top, j - left)[2]; //ROSSO
        }

    imshow("Immagine iniziale (RGB)", image);
    waitKey(0);

    imshow("Padding border constant (RGB)", PadRGB);
    waitKey(0);

    return PadRGB;
}
