//Calculating the power .

#ifndef FINDPOWER_H
#define FINDPOWER_H


#include <iostream>
#include <vector>
#include <tuple>
#include "GetIrraTemp.h"

using namespace std;

void generateGraph3(const vector<double>& volt, const vector<double>& power){
	
    // Output the data into a file
    ofstream outputFile("data.txt");
    if (outputFile.is_open()) {
		
        // Write the data into the file
		
        for (int i = 0; i < volt.size(); i++) {
			outputFile << volt[i] << " " << power[i] << endl;

        }
		
        outputFile.close();
		
    } else {
        cout << "Error: Unable to create the data file." << endl;
        return;
    }

	
    ofstream gnuplotScript("plot_script.gnu");
    if (gnuplotScript.is_open()) {
        gnuplotScript << "set terminal pngcairo enhanced font 'Verdana,12'\n";
        gnuplotScript << "set output 'Powgraph.png'\n";
        gnuplotScript << "set title 'Power Comparison'\n";
        gnuplotScript << "set xlabel 'Voltage (V)'\n";
        gnuplotScript << "set ylabel 'Power (W)'\n";
        gnuplotScript << "set key top left\n";
		gnuplotScript << "plot '-' with lines title 'Power 1'\n";
        
		
		// Write data for power 1
        for (int i = 0; i < volt.size(); i++) {
            gnuplotScript << volt[i] << " " << power[i] << "\n";
        }
        gnuplotScript << "e\n";

        gnuplotScript.close();

        // Run the Gnuplot script
        system("gnuplot plot_script.gnu");
    } else {
        cout << "Error: Unable to create the Gnuplot script file." << endl;
    }
}




tuple<vector<double>, vector<double>, vector<double>> Max_power(){

     const double Isc = 3.8, a = 0.00065, Tr = 298.15, q = 1.602 * pow(10, -19), Voc = 21.1,
		b = 0.08, Rs = 0.008 * 36, Rsh = 1000 * 36, n = 2.1 , k= 1.3806*pow(10,-23) , Ns= 36,
		error = 0.00001;

    // Variables for the solar cell model
    double irra = 0, temp = 0, temp2=0, I0 = 0, Iph = 0, I = 0, I_func = 0, I_prime = 0, errorval = 0,
		I_0 = 0, expo = 0, V_max=0;
		
    char ans ;

    vector<double> volt, current, power, volt2, current2, power2;

    tie(V_max, irra, temp) = MaxVolatge(); // her it will ask the user for time and fetch irra and temp
	

    // Find the volt current and power for the first time the user entered 

    I0 = (Isc + (a*(temp-Tr)) ) / ( ( exp( (q*(Voc+(b*(temp-Tr)))) / (n * k * temp * Ns ) ) ) -1 );
	

    Iph =(Isc + ( a * (temp - Tr)) )* (irra/1000);


    for (int i = 0; i <= 100; i++){
		double V0 = (i) * (V_max / 100);
       	volt.push_back(V0);
	}


	//Calculate the current
    for( int i = 0; i <= 100; i++){
		try{

			I = 0;
			I_0 = Isc;

			do{
			

				I_func= I_0 - Iph + (I0 * (exp ( q *( volt[i]+ (I_0 *(Rs)))/(n*k*temp*Ns))-1)) + ((volt[i] +(I_0*(Rs) ))/Rsh);

				I_prime= 1 +  (I0*((( q *Rs)/(n*k*temp*Ns)) * (exp ((q*( volt[i]+ (I_0 *(Rs))))/(n*k*temp*Ns)))))+ (Rs/Rsh);

				I = I_0 - (I_func / I_prime);
				errorval = abs(I - I_0);

				I_0 = I;

				if(errorval <= error){
					current.push_back(I);
				}		

			}
			while(errorval > error);
			
		}
		catch (...){
			break;
		}
	}
	
	
	//Calculate the power
    double powerval = 0, temp_power=0, max_power =0;

    for (int i = 0; i<=100; i++){
        powerval = volt[i] * current[i];
        power.push_back(powerval);

        if (powerval > temp_power){
            max_power = powerval;
        }
        temp_power = powerval;
    }

    cout << endl;
    cout << "max power when the temperature= "<<temp<<" K, and the irradiance= "<<irra <<" W/m2, is: "<< max_power <<" P."<< endl;
	cout<<endl;
	
	generateGraph3(volt, power);
	
	return make_tuple(volt, current, power);
	}
	#endif