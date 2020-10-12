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
	};//Calcular a tensão de um certo volume ao longo do poço
	double get_hydrostatic(double rho_f, double z, double z0, double P0){
		return rho_f*g*(z-z0)+P0;
	};// Calculo da hidroestática 

};
class Fluid{ //classe representado os fluidos
	public:
		double theta_600;//constante do fluido
		double theta_300;//constante do fluido
		double fluid_visconsity;//viscosidade do fluido
		double rho_f; //massa específica do fluido
		double q; //volume por unidade de tempo
		double v;
		double Re;
		double f;
		double Perda_f;
		Fluid(double a, double b){
			theta_300 = a;
			theta_600 = b;
			 //viscosidade comum para todos os fluidos
		};
};
class Fluid_Newton: public Fluid{ // classe representando aos fluidos newtonianos
	public:
		double Perda_f; //Perda de carga 
		
		void set_visconsity(){
			fluid_visconsity = theta_300; //viscosidade do fluido newtoniano
		};
		void set_velocity(double D){//Cálculo da velocidade média do fluido
			v =  q/(2.448*pow(D,2));
		};
		void set_Reynolds(double D){//Calculo do número de Reynolds
			Re = 928*rho_f*v*D/fluid_visconsity;
		};
		void set_friction_Laminar(){
			f = 16/Re;
		};
		void set_friction_turbulent(){ //calculo da friccao turbulenta é feito interativamente
			double f_i_1 = 0.01;
			float error = 10;
			double F, F_, f_i;	
			while(error>0.01){//o codigo irá rodar até que o erro seja menor que 1%
				F = 4*log(Re*sqrt(f_i_1))-1/sqrt(f_i_1)-0.395;
				F_ = -2/(Re*pow(f_i_1,2))+1/(2*sqrt(pow(f_i_1,3)));
				f_i = f_i_1-F/F_;
				error = sqrt(pow((f_i_1-f_i),2));
				f_i_1 = f_i;

			};
			f = f_i; 
		}

		double get_fluid_loss(double D){//Cálculo da perda de carga para um determinado solo
			set_velocity(D);
			set_Reynolds(D);
			if(Re<2000) set_friction_Laminar();

			else set_friction_turbulent();
	
			Perda_f =  f*rho_f*pow(v,2)/(25,8*D);
			return Perda_f;
		};
	};
class Fluid_Elastic: public Fluid{ //classe representado aos fluidos elásticos
	public:
		double fluid_Shear_stress0; //variável representado a constante de cisalhamento necessária para que o fluido seja deformado
		double He, Re_c;
		
		void set_visconsity(){
			fluid_visconsity = theta_600-theta_300; 
			fluid_Shear_stress0 = theta_300  - fluid_visconsity;
		};
		void set_velocity(double D){//Cálculo da velocidade média do fluido
			v =  q/(2.448*pow(D,2));
		};
		void set_Reynolds(double D){//Calculo do número de Reynolds
			Re = 928*rho_f*v*D/fluid_visconsity;
		};
		void set_Hedstron(double D){
			He = 37100*rho_f*fluid_Shear_stress0*D/pow(fluid_visconsity,2);
		};
		void set_Critical_Reynolds(){
			double x_i_1 = 1;
			double x_i;
			float error = 10;
			double F, F_;
			while(error>0.01){
				F = He*pow((1-x_i_1),3)/16800;
				F_ = -3*He*pow((1-x_i_1),2)/16800-1;
				x_i = x_i_1 - F/F_;

				error = sqrt(pow((x_i_1-x_i),2));
				x_i_1 = x_i;
			};
			Re_c = x_i;
		};
		void set_friction_Laminar(){
			f = 16/Re;
		};
		void set_friction_turbulent(){ //calculo da friccao turbulenta é feito interativamente
			double f_i_1 = 0.01;
			float error = 10;
			double F, F_, f_i;	
			while(error>0.01){//o codigo irá rodar até que o erro seja menor que 1%
				F = 4*log(Re*sqrt(f_i_1))-1/sqrt(f_i_1)-0.395;
				F_ = -2/(Re*pow(f_i_1,2))+1/(2*sqrt(pow(f_i_1,3)));
				f_i = f_i_1-F/F_;
				error = sqrt(pow((f_i_1-f_i),2));
				f_i_1 = f_i;

			};
			f = f_i; 
		}
		double get_fluid_loss(double D){//Cálculo da perda de carga para um determinado solo
			set_velocity(D);
			set_Reynolds(D);
			if(Re>Re_c) set_friction_Laminar();
		
			else set_friction_turbulent();
		
			Perda_f =  f*rho_f*pow(v,2)/(25,8*D);
			return Perda_f;
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