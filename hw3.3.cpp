// File: hw3.3.cpp
// Author(s): Jingyi Guo

#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

// define the NormCDF function here
double NormCDF(double x)//(a)
{
	return (1 / 2.0 + 1 / 2.0 * erf(x / sqrt(2.0)));
}

double BSMEuroCallPrice(double s0, double k, double r, double t, double sigma)//(a)
{
	double d1 = (log(s0 / k) + (r + pow(sigma, 2.0) / 2.0)*t) / (sigma*sqrt(t));
	double d2 = d1 - sigma*sqrt(t);
	return (s0*NormCDF(d1) - exp(-r*t)*k*NormCDF(d2));
}

void CallDisplay(double s0, double k, double r, double t, double sigma)//(b)
{
	cout << setw(10) << s0 << setw(10) << k
		<< setw(10) << r << setw(10) << t
		<< setw(10) << sigma << setw(15)
		<< BSMEuroCallPrice(s0, k, r, t, sigma) << '\n';
}

double Black76CallPrice(double f, double k, double r, double t, double sigma)//(a)
{
	double d1 = (log(f / k) + (pow(sigma, 2.0) / 2.0)*t) / (sigma*sqrt(t));
	double d2 = d1 - sigma*sqrt(t);
	return (exp(-r*t)*(f*NormCDF(d1) - k*NormCDF(d2)));
}

double ImpliedVol(double C, double f, double k, double r, double t)//(b)
{
	double a = 0.0;
	double b = 3.0;
	double c = (a+b)/2.0;
	while (true)
	{
		c = (a + b) / 2.0;
		double fa = C - Black76CallPrice(f, k, r, t, a);
		double fb = C - Black76CallPrice(f, k, r, t, b);
		double fc = C - Black76CallPrice(f, k, r, t, c);
		if (fabs(fc) <= 0.0001)
			return c;
		if (fa*fc > 0)
			a = c;
		if (fa*fc < 0)
			b = c;
	}
}

int main()
{
	ifstream fin("CL_and_NG_expirations_and_settlements.txt");
	ofstream fout("strike_vs_impvol.csv");
	string line;
	fout << "Strike, ImpVol\n";
	double fprice = 48.33;
	while (getline(fin, line))//(c)
	{
		if (line.substr(0, 25) == "CL        2016-11    Call")
		{
			double strike = atof(line.substr(32, 6).c_str());
			double cprice = atof(line.substr(42, 6).c_str());
			fout << strike << ", "  << ImpliedVol(cprice, fprice, strike, 0.02, 56.0 / 365) << "\n";
		}
	}
	/*(d)
	The assumption that volatility is constant in Black-Scholes-Mertion model is improper according to my plot,
	because in my plot, the graph of implied volatility against strike is a skewed "smile" curve rather than a flat line.
	*/
	return 0;
}
