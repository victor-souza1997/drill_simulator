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
			double fluid_visconsity; \\
		
};
class Fluid_Newton: public Fluid{

	void set_visconsity(){
		fluid_visconsity = theta_300;
	}

};
class Fluid_Elastic: public Fluid{
	public:
		double fluid_Shear_stress0;
	void set_visconsity(){
		fluid_visconsity = theta_600-theta_300; 
		fluid_Shear_stress0 = theta_300  - fluid_visconsity;
	}
};
class Fluid_Power_Law: public Fluid{
	public:
		double fluid_n_power_law;
		double fluid_K_power_law;
	void set_visconsity(){
		fluid_n_power_law = 3.32*log(theta_600/theta_300); 
		fluid_K_power_law = 510*theta_300/(pow(510,n));
	};	
};


class Drill{
	public:

};


class Fluid_Newton{

};
class Fluid_Elastic{

};
class Fluid_


int main(){

	Well well1;
	cout << well1.get_stress(1,0.001,10,0.001,10)<<"\n";


	return 0;

	};