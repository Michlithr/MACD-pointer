#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

//wczytywanie wig20
void loadWIG20(double * out, int size);
double ema(double* probki, int dzien, int liczba_okresow);

int main() {
	int size = 1000;
	double* WIG20 = new double[size];
	loadWIG20(WIG20, size);

	ofstream out;
	out.open("macd&signal.txt");
	out << "day macd singal\n";
	int macdSize = size - 26;
	int signalSize = macdSize - 9;
	double* macd = new double[macdSize];
	double* signal = new double[signalSize];
	for (int i = 0, j = 0; i < macdSize; i++) {
		macd[i] = ema(WIG20, 26 + i, 12) - ema(WIG20, 26 + i, 26);
		out << i + 27 << ' ' << setprecision(6) << macd[i] << ' ';
		if (i < 9) out << '\n';
		else {
			signal[j] = ema(macd, 9 + j, 9);
			out << setprecision(6) << signal[j++] << '\n';
		}
	}
	delete macd, signal;
	return 0;
}

void loadWIG20(double * out, int size) {
	ifstream input("WIG20.txt");
	int k = 0;
	if (input.is_open()) {
		while (k < size) {
			string number;
			int data;
			getline(input, number); //read number
			out[k] = (double)atof(number.c_str()); //convert 
			k++;
		}
	}
}

double ema(double* probki, int dzien, int liczba_okresow) {
	double alpha = 2.0 / (liczba_okresow - 1.0);
	double licznik = 0.0, mianownik = 0.0;
	for (int i = 0; i <= liczba_okresow; i++, dzien--) {
		licznik += pow(1 - alpha, i) * probki[dzien];
		mianownik += pow(1 - alpha, i);
	}
	return licznik / mianownik;
}