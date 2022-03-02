// Sharpening


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    
    //Lettura immagine.
    Mat image;
    image = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 4\\colosseo.jpg", IMREAD_GRAYSCALE);
    
    
    if (image.empty())
    {
        cout << "Could not open image" << endl;
    }
    
    
    //Laplacian
    Mat output1;
    Laplacian(image, output1, image.type());

    
    Mat output2;
    //Il quarto e il quinto parametro indica se voglio o meno applicare uno dei due filtri di Sobel (gy e gy) o entrambi.
    Sobel(image, output2, image.type(), 1, 1);

    Mat output3;
    Sobel(image, output3, image.type(), 0, 1);

    Mat output4;
    Sobel(image, output4, image.type(), 1, 0);

    
    imshow("Original image", image);
    imshow("Laplacian image", output1);
    imshow("Sobel image", output2);
    imshow("SobelX image", output3);
    imshow("SobelY image", output4);
    waitKey(0);

}

