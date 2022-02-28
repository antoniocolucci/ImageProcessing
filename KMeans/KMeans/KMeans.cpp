/*
  K-MEAND
  STEP:
  - Inizializzo i centri dei cluster.
  -	Assegno ogni pixel al centro più vicino:
        Per ogni pixel Pj calcolare la distanza dai k centri Ci ed assegnare Pj al cluster con il centro Ci più vicino.
  -	Aggiornare i centri:
        Calcolare la media dei pixel in ogni cluster.
  - Ripetere i punti 2 e 3 finchè il centro (media) di ogni cluster non viene più modificato (ovvero i gruppi non vengono modificati).
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double myDistance(Scalar p1, Scalar p2)
{
	return pow(p1[0] - p2[0], 2) +
		   pow(p1[1] - p2[1], 2) +
		   pow(p1[2] - p2[2], 2);
}

void myKmeans(Mat& src, Mat& dst, int nClusters)
{
	//Vettore che contiene i colori dei centri.
	vector<Scalar> centers_colors;
	vector<vector<Point>> clusters;

	RNG random(getTickCount());

	//Calcolo i centri ciclando sui clusters.
	for (int k = 0; k < nClusters; k++)
	{
		Point center;
		center.x = random.uniform(0, src.cols); //Calcolo posizione x del centro.
		center.y = random.uniform(0, src.rows); //Calcolo posizione y del centro.

		//Scalare che contiene i colori del centro appena calcolato.
		Scalar center_colors(src.at<Vec3b>(center)[0], src.at<Vec3b>(center)[1], src.at<Vec3b>(center)[2]);
		centers_colors.push_back(center_colors); //Aggiungo lo scalare al vettore che contiene i colori dei centri
		//Vettore che conterrà i pixel che verranno assegnati al cluster k.
		vector<Point> t;
		clusters.push_back(t); // Aggiungo un vettore di punti vuoto al vettore dei cluster.
	}

	//Assegno i pixel ai cluster, ricalcolo i centri usando le medie, fino a che la differenza < 0.1
	double oldCenterSum = 0;
	double difference = INFINITY;

	//Cicla finchè la differenza tra le vecchie medie e le nuove medie è superiore ad una threshold (0.1).
	while (difference > 0.1)
	{
		//Pulisco i vettori di punti presenti in clusters
		for (int k = 0; k < nClusters; k++)
		{
			clusters[k].clear();
		}

		//Aggiorno i pixels nei clusters
		for (int x = 0; x < src.rows; x++)
		{
			for (int y = 0; y < src.cols; y++)
			{
				//Calcolo le distanze da ogni centro dei cluster e posiziono il pixel nel cluster più vicino
				double minDistance = INFINITY;
				int clusterIndex = 0;
				Point pixel(y, x);
				//Scalar pixel = src.at<Vec3b>(x, y);
				//Per ogni cluster estraggo il centro e ne calcolo la distanza col punto attuale
				for (int k = 0; k < nClusters; k++)
				{
					//Estraggo lo scalare del centro dal vettore contenente i colori.
					Scalar center = centers_colors[k];
					Scalar point(src.at<Vec3b>(x, y)[0], src.at<Vec3b>(x, y)[1], src.at<Vec3b>(x, y)[2]);
					//Calcolo la distanza tra il centro e il punto attuale dell'immagine
					double distance = myDistance(center, point);

					//Se la distanza è minima la aggiorno.
					//Calcolo il centroide k più vicino al pixel in posizione (x,y).
					if (distance < minDistance)
					{
						minDistance = distance;
						clusterIndex = k; //Salvo il pixel nel cluster di indice k.
					}
				}
				//Aggiungo il pixel nel vettore k dei clusters.
				clusters[clusterIndex].push_back(pixel);
			}
		}

		//Ricalcolo la media di ogni cluster.
		double newCenterSum = 0;
		for (int k = 0; k < nClusters; k++)
		{
			//Definisco un vettore che conterrà tutti i punti presenti nel cluster k.
			vector<Point> clusterPoints = clusters[k];
			double blue = 0, green = 0, red = 0;
			//cout << "Cluster points size " << clusterPoints.size() << endl;

			//Scorro su tutti i punti del vettore e sommo le componenti dei colori nelle apposite variabili.
			for (int i = 0; i < clusterPoints.size(); i++)
			{
				Point pixel = clusterPoints[i];
				blue += src.at<Vec3b>(pixel)[0];
				green += src.at<Vec3b>(pixel)[1];
				red += src.at<Vec3b>(pixel)[2];
			}

			//Calcolo media dei colori del nuovo centro dividendo la somma calcolata prima per il numero di punti.
			blue /= clusterPoints.size();
			green /= clusterPoints.size();
			red /= clusterPoints.size();

			//Estraggo il centro dal vettore contenente gli scalari dei centri.
			Scalar center = centers_colors[k];
			//Definisco il nuovo centro con le medie calcolate
			Scalar newCenter(blue, green, red);
			//Calcolo la distanza del nuovo centro dal vecchio e l'aggiungo a newCenterSum.
			newCenterSum += myDistance(newCenter, center);
			//Aggiorno il nuovo centro
			centers_colors[k] = newCenter;
		}
		
		//Calcolo la media dividendo la somma per il numero di cluster.
		newCenterSum /= nClusters;
		//Calcolo la differenza tra la vecchia somma e quella appena calcolata.
		difference = abs(oldCenterSum - newCenterSum);
		cout << "Differenza " << difference << endl;
		oldCenterSum = newCenterSum;
	}

	//Disegno l'immagine di output
	for (int k = 0; k < nClusters; k++)
	{
		vector<Point> clusterPoints = clusters[k];
		Scalar center = centers_colors[k];
		for (int i = 0; i < clusterPoints.size(); i++)
		{
			//Assegno il valore del centro agli altri pixel del cluster. Tutti i pixel di un determinato cluster riceveranno il valore medio(center) di quel cluster.
			Point pixel = clusterPoints[i];
			dst.at<Vec3b>(pixel)[0] = center[0];
			dst.at<Vec3b>(pixel)[1] = center[1];
			dst.at<Vec3b>(pixel)[2] = center[2];
		}
	}
}

int main(int argc, char* argv[])
{

	//Lettura immagine    
	Mat input = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 11\\lena.png", IMREAD_COLOR);

	if (input.empty())
	{
		cout << "Can't open or find the image" << endl;
		exit(-1);
	}

	int nClusters = 0;
	cout << "Scegliere il numero dei clusters: ";
	cin >> nClusters;

	Mat dst(input.rows, input.cols, input.type());
	myKmeans(input, dst, nClusters);

	imshow("Input image", input);
	imshow("K-Means", dst);
	waitKey(0);
	return 0;
}
