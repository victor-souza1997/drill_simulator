#include <math.h>  
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

double g = 9.8;
class Well{ //classe para o poço
	public:				
	double get_stress(double alpha, double rho_w, double LDA, double rho,double Z){
		return alpha*(rho_w*LDA+rho*Z);
	};
	double get_hydrostatic(double rho_f, double z, double z0, double P0){
		return rho_f*g*(z-z0)+P0;
	};

};
class Fluid{ //classe representado os fluidos
	public:
		double theta_600;
		double theta_300;
		double fluid_visconsity;
		Fluid(double a, double b){
			theta_300 = a;
			theta_600 = b;
			 //viscosidade comum para todos os fluidos
		};
};
class Fluid_Newton: public Fluid{ // classe representando aos fluidos newtonianos
	public:
		void set_visconsity(){
			fluid_visconsity = theta_300; //viscosidade do fluido newtoniano
		};

};
class Fluid_Elastic: public Fluid{ //classe representado aos fluidos elásticos
	public:
		double fluid_Shear_stress0; //variável representado a constante de cisalhamento necessária para que o fluido seja deformado
		void set_visconsity(){
			fluid_visconsity = theta_600-theta_300; 
			fluid_Shear_stress0 = theta_300  - fluid_visconsity;
		};
};
class Fluid_Power_Law: public Fluid{ //classe repressentando aos fluidos modelados com a leis das potências
	public:
		double fluid_n_power_law;
		double fluid_K_power_law;
		void set_visconsity(int n){
			fluid_n_power_law = 3.32*log(theta_600/theta_300); 
			fluid_K_power_law = 510*theta_300/(pow(510,n));
		};	
};

class Drill{ //classe para a broca
	public:

};
int main(){

	Well well1;
	ofstream outfile("hydrostatic.txt");
	for(double z = 0;z<2000; z=z+0.1){		
		outfile << z << " " << well1.get_hydrostatic(0.01,z,0,1000) << "\n";//std::endl;
		cout << z << " " << well1.get_hydrostatic(0.01,z,0,1000) << "\n";
	};
	outfile.close();
	return 0;
	};