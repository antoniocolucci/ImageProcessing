#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

	int dim, th;

	//Lettura immagine.
	Mat image;
	image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 3\\napoli.jpg", IMREAD_GRAYSCALE);

	if (image.empty())
	 {
		cout << "Could not open or find the image.";
		return -1;
	 }

	//Lettura dimensione filtro.
	cout << "Inserire la dimensione del filtro: ";
	cin >> dim;

	//Allocazione filtro e convoluzione.
	Mat average_filter = Mat::ones(dim, dim, CV_32F) / (float)(dim * dim); //creazione filtro media. Creo la matrice di tutti 1 della dimensione dim.

	//Per la convoluzione ruotare il filtro di 180°.
	//rotate(filter,r_filter,ROTATE_180)

	Mat output1; //immagine di output.
	filter2D(image, output1, image.type(), average_filter);

	//Blurring average filter.
	Mat output2;
	blur(image, output2, Size(dim, dim));

	//Blurring average filter.
	Mat output3;
	boxFilter(image, output3, image.type(), Size(dim, dim)); //outuput identico a blur, è la versione generalizzata.

	//Median filter.
	Mat output4;
	medianBlur(image, output4, dim);

	//Gaussian filter.
	Mat output5;
	GaussianBlur(image, output5, Size(dim, dim), 0, 0);


	/*imshow("Original image", image);
	imshow("Filter2D image", output1);
	imshow("Blur image", output2);
	imshow("BoxFilter image", output3);
	imshow("MedianBlur image", output4);
	imshow("GaussianBlur image", output5);
	waitKey(0);*/


	cout << "Inserire il valore del threshold: ";
	cin >> th;

	//Thresholding.
	Mat mask;
	threshold(output5, mask, th, 1, CV_THRESH_BINARY);

	Mat mask255;
	threshold(output5, mask255, th, 255, CV_THRESH_BINARY);
	imshow("Mask", mask255);

	//Immagine * maschera per mostrare solo le regioni di interesse.
	imshow("Image*mask", image.mul(mask)); //prodotto puntuale tra la maschera e l'immagine.
	waitKey(0);

	return 0;
}
