#include <math.h>  
#include <stdio.h>
#include <iostream>
using namespace std;


class Well{ //classe para o poço
	public:
				
	double get_stress(double alpha, double rho_w, double LDA, double rho,double Z){
		return alpha*(rho_w*LDA+rho*Z);
	};

};
class Fluid{ //classe representado os fluidos
	public:
		Fluid(){
			double fluid_visconsity; //viscosidade comum para todos os fluidos
		};
};
class Fluid_Newton: public Fluid{ // classe representando aos fluidos newtonianos

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
	void set_visconsity(){
		fluid_n_power_law = 3.32*log(theta_600/theta_300); 
		fluid_K_power_law = 510*theta_300/(pow(510,n));
	};	
};


class Drill{ //classe para a broca
	public:

};


int main(){

	Well well1;
	cout << well1.get_stress(1,0.001,10,0.001,10)<<"\n";


	return 0;

	};