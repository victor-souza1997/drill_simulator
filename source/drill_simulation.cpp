#include <math.h>  
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

double g = 9.8;
double pi = 3.14;
class Fluid{ //classe representado os fluidos
	public:
		//Given variables
		double theta_600;//constante do fluido
		double theta_300;//constante do fluido
		double At; //Área total dos orifícios da broca
		double rho_f; //massa específica do fluido
		double q; //volume por unidade de tempo
		//calculated variables
		double fluid_visconsity;//viscosidade do fluido
		double v; //Velocidade média do fluido
		double Re; // Número de Reynolds
		double f; //Constante de fricção
		double Perda_f; //A perda será comum ao fluido
		double Perda_f_b; //perda na broca
		Fluid(double a, double b, double c, double d){
			theta_300 = a;
			theta_600 = b;
			rho_f = c;
			q = d;
			
		};
		/*
		set_flux_rate(P_){
			DeltaP  = P_now - P_before
			q = DeltaP*pow(D,4)/(128*fluid_visconsity*L);
		};*/
		void set_velocity(double D){//Cálculo da velocidade média do fluido, comum a todos os fluidos
			v =  q/(2.448*pow(D,2));
		};

		void set_Reynolds(double D){//Calculo do número de Reynolds, comum a todos os fluidos
			Re = 928*rho_f*v*D/fluid_visconsity;
		};
		void set_friction_Laminar(){// Calculo da fricção laminar, comum a todos os fluidos
			f = 16/Re;
		};
		void set_drill_loss(double C_d){ //perda de carga na broca
			Perda_f_b = 8.311*pow(10,-5)*rho_f*pow(q,2)/(pow(C_d,2)*pow(At,2));
		};
		void set_drill_area(double OD, int n_holes){ //definindo área total da saída dos jatos da broca
			At = 0;
			for(int i = 0; i<n_holes; i++){
				At = At + pow(OD,2)*pi/4;
			};
		};
		
};
class Fluid_Newton: public Fluid{ // classe representando aos fluidos newtonianos
	public:


		Fluid_Newton(double theta_300, double theta_600, double rho_f, double q) : Fluid(theta_300, theta_600, rho_f, q) {}

		double Perda_f; //Perda de carga 
		
		void set_visconsity(){
			fluid_visconsity = theta_300; //viscosidade do fluido newtoniano
		};
		
		void set_friction_turbulent(){ //calculo da friccao turbulenta é feito interativamente
			double f_i_1 = 0.01;
			float error = 10;
			double F, F_, f_i;	
			while(error>0.01){//o codigo irá rodar até que o erro seja menor que 1%
				F = 4*log10(Re*sqrt(f_i_1))-1/sqrt(f_i_1)-0.395;
				F_ = -2/(Re*pow(f_i_1,2))+1/(2*sqrt(pow(f_i_1,3)));
				f_i = f_i_1-F/F_;
				error = sqrt(pow((f_i_1-f_i),2));
				f_i_1 = f_i;

			};
			f = f_i; 
		}

		double get_fluid_loss(double D){//Cálculo da perda de carga para um determinado solo
			set_visconsity();
			set_flux_rate();
			set_velocity( D);
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
		
		void set_visconsity(){// Encontrar a viscosidade do fluido, junto com a tensão de cisalhamento inicial para que o fluido se deforme
			fluid_visconsity = theta_600-theta_300; 
			fluid_Shear_stress0 = theta_300  - fluid_visconsity;
		};
		void set_Hedstron(double D){
			He = 37100*rho_f*fluid_Shear_stress0*D/pow(fluid_visconsity,2); // Encontrar o número de Hedstron
		};
		void set_Critical_Reynolds(){ //Funcao para calcular o valor de Reynolds crítico
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
		void set_friction_turbulent(){ //calculo da friccao turbulenta é feito interativamente
			double f_i_1 = 0.01;
			float error = 10;
			double F, F_, f_i;	
			while(error>0.01){//o codigo irá rodar até que o erro seja menor que 1%
				F = 4*log10(Re*sqrt(f_i_1))-1/sqrt(f_i_1)-0.395;
				F_ = -2/(Re*pow(f_i_1,2))+1/(2*sqrt(pow(f_i_1,3)));
				f_i = f_i_1-F/F_;
				error = sqrt(pow((f_i_1-f_i),2));
				f_i_1 = f_i;

			};
			f = f_i; 
		}
		double get_fluid_loss(double D){//Cálculo da perda de carga para um determinado solo
			set_visconsity();
			set_Critical_Reynolds();
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
		double Re_c;
		Fluid_Power_Law(double theta_300, double theta_600, double rho_f, double q) : Fluid(theta_300, theta_600, rho_f, q) {}

		double get_velocity() {return v;};
		void set_visconsity(double D){
			fluid_n_power_law = 3.32*log10(theta_600/theta_300); 
			fluid_K_power_law = 510*theta_300/(pow(510,fluid_n_power_law));
			fluid_visconsity = fluid_K_power_law/96*pow(D/v,1-fluid_n_power_law)*pow((3+1/fluid_n_power_law)/0.0416,fluid_n_power_law);
		};	

		void set_Critical_Reynolds(){
			Re_c = -9188.5*fluid_n_power_law+5947.6;
			if(Re_c<2100) Re_c = 2100;
		}
		void set_friction_turbulent(){ //calculo da friccao turbulenta é feito interativamente
			double f_i_1 = 0.001;
			float error = 10;
			double F, F_, f_i;	
			while(error>0.01){//o codigo irá rodar até que o erro seja menor que 1%
				F = 4*log10(Re*pow(f_i_1,1-fluid_n_power_law/2))/pow(fluid_n_power_law,0.75)-1/sqrt(f_i_1)-0.395/pow(fluid_n_power_law,1.2);
				F_ = 1/(2*pow(f_i_1,1.5))+4*(1-0.5*fluid_n_power_law)/(f_i_1*pow(fluid_n_power_law,0.75)*log(10));
				f_i = f_i_1-F/F_;
				error = sqrt(pow((f_i_1-f_i),2));
				f_i_1 = f_i;
			};
			f = f_i; 
		};
		double get_fluid_loss(double D){//Cálculo da perda de carga para um determinado solo
			set_visconsity(D);
			set_Critical_Reynolds();
			set_velocity(D);
			set_Reynolds(D);
			if(Re>Re_c) set_friction_Laminar();
		
			else set_friction_turbulent();
		
			Perda_f =  f*rho_f*pow(v,2)/(25,8*D);
			return Perda_f;
		};


	};

class Drill{ //classe para a broca
	public:
		double f1, f2, f3, f4, f5, f6, f7, f8;
		double a1, a2, a3, a4, a5, a6, a7, a8;

		void set_f1(){f1 = exp(a1*2.303);};
		void set_f2(double Z){f2 = exp(2.303*a2*(10000-Z));}
		void set_f3(double Z, double G_p){f3 = exp(2.303*a3*pow(Z,0.69)*(G_p - 9));};
		void set_f4(double Z, double G_p, double ECD){f4 = exp(2.303*a4*Z*(G_p - ECD));};
		void set_f5(double WOB, double D_b, double minimal_force){f5 = pow(((WOB/D_b)-minimal_force)/(4-minimal_force), a5);};
		void set_f6(double N){f6 = pow(N/60,a6);};
		void set_f7(double h){f7 = exp(-a7*h);};
		void set_f8(double Fj){f8 = pow(Fj/1000, a8);};
};

class Well{ //classe para o poço
	
	
	private:
		Fluid_Newton fluid;
	public:

		double *Ph = new double[1];//declaracao do pressão estática ao longo do poço. Ponteiro 
		double *Flux_loss = new double[1];//declaração da perda de pressão ao longo do poço. Ponteiro
		double *height = new double[1];//declaração de cada etapa da altura do poço. Ponteiro
		double theta_600, theta_300, D, q, rho_f;
		
		Well(double a, double b, double c, double d, double e):fluid(a, b, c, d){
			rho_f = c;
			theta_600 = b;
			theta_300 = a;
			q = d;
			D = e; 
			Ph[0] = 0;
			Flux_loss[0] = 0;
			height[0] = 0;
			//A * a = new A();
			//Fluid_Newton * Fluid1 = new Fluid_Newton(a, b, c, d);


		};
	
		
		void add_height(double step, int size){
			double * temp = new double [size + 1];
			for (int i = 0; i < size; i++) 
    		{	
    			temp[i] = height[i];
    			//cout<<"counting: "<<i <<" "<<height[i]<<"\n";
    		}
    		temp[size] = step;
    		delete [] height;
    		height = temp;
		};
		
		void add_Ph(int size){
			double *temp = new double[size+1];
			 for (int i = 0; i < size; i++) 
    			temp[i] = Ph[i];
    		temp[size] = get_hydrostatic(rho_f, height[size], 0, 0) + temp[size];
    		delete [] Ph;
    		Ph = temp;
		};
		
		void add_Fluxloss(int size){

			double *temp = new double[size+1];
			 for (int i = 0; i < size; i++) {
			 	temp[i] = Flux_loss[i];
			 	cout<<temp[i]<<"\n";
    		}		
    		temp[size] = fluid.get_fluid_loss(D) + temp[size-1];
    		delete [] Flux_loss;
    		Flux_loss = temp;
		};

		double get_stress(double alpha, double rho_w, double LDA, double rho,double Z){
			return alpha*(rho_w*LDA+rho*Z);
		};//Calcular a tensão de um certo volume ao longo do poço
		double get_hydrostatic(double rho_f, double z, double z0, double P0){
			return rho_f*g*(z-z0)+P0;
		};// Calculo da hidroestática 


};

int main(){

	double q, rho_f, theta_600, theta_300;
	q = 200; //m^3/s
	rho_f = 2;
	theta_600 = 20;
	theta_300 = 10;
	double DI = 2;// DO = 3;
	
	Well well1(theta_300, theta_600, rho_f, q, DI);
	
	//Fluid_Power_Law Fluid1(theta_300, theta_600, rho_f, q);
	
	//ofstream outfile1("hydrostatic.txt");
	//ofstream outfile2("tunnel_loss.txt");
	
	//double DI[4], DO[4];q/Fluid1.get_velocity()
	
	double Pump = 10000;
	int size = 1;	
	for(double z = 0; z < 1; z=z+0.1){
		//cout<< "altura : "<<z<<"\n";
		//outfile1 << z << " " << well1.get_hydrostatic(0.01,z,0,0) << "\n";//std::endl;
		//outfile2 << z << " " << Pump +well1.get_hydrostatic(0.01,z,0,0)- Fluid1.get_fluid_loss(DI)*z <<" "<<Fluid1.get_velocity()/q<<" "<< DI <<" "<< well1.get_hydrostatic(0.01,z,0,0)<<" \n";//std::endl;
		well1.add_height(z, size);
		well1.add_Ph(size);
		well1.add_Fluxloss(size);
		size++;	
	};



	
	//outfile1.close();
//	outfile2.close();
	return 0;
	};