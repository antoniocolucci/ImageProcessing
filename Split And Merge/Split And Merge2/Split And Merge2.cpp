#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#define MIN_DEV 10.7
#define MAX_AREA 64

using namespace std;
using namespace cv;

//Struct che descrive il comportamento della regione.
struct Region
{
	vector<Region> adj;
	bool valid;
	Scalar label;
	Rect area;
};

//Ritorna un valore booleano che rappresenta il predicato.
bool predicate(Mat src)
{
	Scalar dev;
	meanStdDev(src, Scalar(), dev); //Calcola la media e la deviazione della matrice.
	//Se la deviazione è minore di quella minima definita o l'area è minore dell'area massima il predicato sarà vero.
	return (dev[0] < MIN_DEV || src.cols * src.rows < MAX_AREA);
}

Region split(Mat src, Rect area)
{
	Region R;
	R.valid = true;
	R.area = area;

	//Se il predicato è vero, la regione NON può essere divisa.
	if (predicate(src))
	{
		Scalar mean;
		meanStdDev(src, mean, Scalar()); //Calcola la media.
		R.label = mean;					 //Assegna la media alla label della regione.
	}
	else
	{
		//Divide le dimensioni della regione in 2.
		int w = src.cols / 2; //100
		int h = src.rows / 2; //80

		//Suddivide le regioni in 4 sottoregioni chiamando ricorsivamente la funzione split.
		Region r1 = split(src(Rect(0, 0, w, h)), Rect(area.x, area.y, w, h));
		Region r2 = split(src(Rect(w, 0, w, h)), Rect(area.x + w, area.y, w, h));
		Region r3 = split(src(Rect(0, h, w, h)), Rect(area.x, area.y + h, w, h));
		Region r4 = split(src(Rect(w, h, w, h)), Rect(area.x + w, area.y + h, w, h));

		//Aggiunge le regioni appena calcolate al vettore delle regioni adiacenti.
		R.adj.push_back(r1);
		R.adj.push_back(r2);
		R.adj.push_back(r3);
		R.adj.push_back(r4);
	}
	return R;
}

void mergeRegion(Mat src, Region& r1, Region& r2)
{
	//Se 2 regioni non hanno regioni adiacenti.
	if (r1.adj.size() == 0 && r2.adj.size() == 0)
	{
		Rect r12 = r1.area | r2.area; //OR.
		if (predicate(src(r12)))
		{
			//Se il predicato è vero, unisci le aree in r1.
			r1.area = r12;
			r1.label = (r1.label + r2.label) / 2;
			r2.valid = false; //invalida r2 perche la sua area ora è compresa in r1
		}
	}
}

void merge(Mat src, Region& r)
{
	if (r.adj.size() > 0) //Se la regione ha delle regioni adiacenti
	{
		//Controlla se è possibile unire le regioni
		mergeRegion(src, r.adj.at(0), r.adj.at(1));
		mergeRegion(src, r.adj.at(2), r.adj.at(3));
		mergeRegion(src, r.adj.at(0), r.adj.at(2));
		mergeRegion(src, r.adj.at(1), r.adj.at(3));

		//Chiama ricorsivamente la procedura su tutte le regioni adiacenti.
		for (int i = 0; i < r.adj.size(); i++)
		{
			merge(src, r.adj.at(i));
		}
	}
}

void display(Mat& out, Region R)
{
	//Se la regione master non ha sotto-regioni e la regione master è valida
	//disegna un rettangolo.
	if (R.adj.size() == 0 && R.valid)
		rectangle(out, R.area, R.label, FILLED); //Area=Zona di interesse; Label=Colore; FILLED=Tipo di rettangolo
	//Visualizza ricorsivamente le sotto-regioni della regione master.
	for (int i = 0; i < R.adj.size(); i++)
		display(out, R.adj.at(i));
}

int main(int argc, char* argv[])
{
	//Lettura immagine    
	Mat src = imread("C:\\antonio\\Parthenope\\Elim\\Laboratorio\\Slide 10\\stonhenge.jfif", IMREAD_COLOR);

	if (src.empty())
	{
		cout << "Could not open or find the image." << endl;
		exit(-1);
	}

	//Dividi l'immagine. Il rettangolo è l'intera immagine.
	Region r = split(src, Rect(0, 0, src.cols, src.rows));
	merge(src, r);

	Mat out = src.clone();
	display(out, r);

	Mat out2 = Mat::zeros(src.size(), src.type());
	display(out2, r);

	imshow("Input", src);
	imshow("Output-1", out);
	imshow("Output-2", out2);
	waitKey(0);
	return 0;
}
