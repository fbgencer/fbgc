#include <iostream>
#include <fstream>
#include <valarray>

using namespace std;

void ort(valarray< valarray<int> > film_puanlari, valarray<double> & puan_ortalamasi)
{
	int top=0;

	for (std::size_t i=0; i<film_puanlari.size(); i++)
	{
		for (std::size_t j=0; j<film_puanlari[i].size(); j++)
		{
			top += film_puanlari[i][j];
		}

		puan_ortalamasi[i] = top / film_puanlari[i].size();
		top=0;
	}
}


int main()
{
	valarray< valarray<int> > fp;
	valarray<double> po;
	int n, yil, sayi, F, S, yil_sayisi;

	cout << "Gireceginiz ilk yil: ";
	cin >> F;
	cout << "Gireceginiz son yil: ";
	cin >> S;
	yil=0;
	yil_sayisi = S-F+1;
	fp.resize(yil_sayisi);

	for (int i=0; i<yil_sayisi; i++)
	{
		yil=i+F;
		cout << yil << " yilinda yaninlanan kac filmi izledin: ";
		cin >> n;
		fp[i].resize(n);

		for (std::size_t j=0; j<fp[i].size(); j++)
		{
			sayi=j+1;
			cout << sayi << ". filmin puanini gir: ";
			cin >> fp[i][j];
		}

	}


	ort(fp,po);

	/*ofstream myfile;
	myfile.open ("film_statistics.txt");

	if (!myfile.is_open())
	{
		cout << "DOSYA YOK !";
	}

	for (int i=0; i<yil_sayisi; i++)
	{
		yil=i+F;
		myfile << yil << " yilinda izledigin filmlerin puan ortalamasi: " << po[i] << endl;
	}

	myfile.close();*/

	return 0;
}