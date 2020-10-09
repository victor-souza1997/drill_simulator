#include <cmath>
#include <stdio.h>
#include <iostream>
using namespace std;


class Well{
	public:
		
	double get_stress(double alpha, double rho_w, double LDA, double rho,double Z){

		return alpha*(rho_w*LDA+rho*Z);

	};

};



int main(){

	Well well1;
	cout << well1.get_stress(1,0.001,10,0.001,10)<<"\n";


	return 0;

	};