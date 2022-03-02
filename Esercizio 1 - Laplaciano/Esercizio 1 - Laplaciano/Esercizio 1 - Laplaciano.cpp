/* 
  Esercizio 1 - Laplaciano.
  Implementare il Laplaciano con kernel isotropico a 45° e 90° utilizzando la funzione di correlazione / convoluzione(o filter2D())
  Per normalizzare i livelli di grigio è possibile usare la funzione normalize():
  normalize(src, dst, 0, 255, NORM_MINMAX, CV_8U);
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{

    Mat image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 4\\luna.jpg", IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Could not open image" << endl;
    }

    Mat output1;
    Mat output2;
    Mat output3;

    int data90[] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 }; //filtro Laplaciano 90°.
    Mat filter90 = Mat(3, 3, CV_32S, data90);
    int data45[] = { 1, 1, 1, 1, -8, 1, 1, 1, 1 }; //filtro Laplaciano 45°.
    Mat filter45 = Mat(3, 3, CV_32S, data45);
    
    
    filter2D(image, output1, image.type(), filter90);
    filter2D(image, output2, image.type(), filter45);
    Laplacian(image, output3, image.type(), 1);
    normalize(image, output2, 0, 255, NORM_MINMAX, CV_8U);

    imshow("image", image);
    imshow("output1", output1);
    imshow("output2", output2);
    imshow("Laplacian", output3);
    waitKey(0);
}

