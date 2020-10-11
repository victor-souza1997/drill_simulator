#include <math.h>  
#include <stdio.h>
#include <iostream>
using namespace std;


class Well{
	public:
				
	double get_stress(double alpha, double rho_w, double LDA, double rho,double Z){

		return alpha*(rho_w*LDA+rho*Z);

	};

};

class Fluid{
	public:
		Fluid(int TYPE, KIND){
			int fluid_type = TYPE; // pode ser  
			int fluid_kind = KIND; //newtoniano, brigham, ou lei das potências
			double fluid_visconsity;
			double fluid_Shear_stress0;
			double fluid_n_power_law;
			double fluid_K_power_law;
		};

		void visconsity(double theta_300, double theta_600, int n){
			if(fluid_type == 1){fluid_visconsity = theta_300;};
			if else(fluid_type == 2){fluid_visconsity = theta_600-theta_300; fluid_Shear_stress0 = theta_300  - fluid_visconsity;};
			else{fluid_n_power_law = 3.32*log(theta_600/theta_300); fluid_K_power_law = 510*theta_300/(pow(510,n))}
		};
};

class Drill{
	public:

};


int main(){

	Well well1;
	cout << well1.get_stress(1,0.001,10,0.001,10)<<"\n";


	return 0;

	};