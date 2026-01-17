//Calculating the power and the new irradiance.

#ifndef NEWIRRA_H
#define NEWIRRA_H


#include <iostream>
#include <vector>
#include <tuple>
#include "GetIrraTemp.h"

using namespace std;


void generateGraph1(const vector<double>& volt, const vector<double>& power, const vector<double>& volt2,
	const vector<double>& power2 , const vector<double>& current , const vector<double>& current2, double irra , double irra2) {
   
   // Output the data into a file
    ofstream outputFile("data.txt");
	
    if (outputFile.is_open()) {
		
        // Write the data into the file
        for (int i = 0; i < volt.size(); i++) {
			
            outputFile << volt[i] << "          " << power[i] << "          " << current[i] 
			<< "             " << volt2[i]<<"                       " <<power2[i] <<"                  " 
			<<current2[i] << endl;
			
        }
		
        outputFile.close();
    } 
	
	
	else {
		
        cout << "Error: Unable to create the data file." << endl;
        return;
		
    }
	
    ofstream gnuplotScript("plot_script.gnu");
	
    if (gnuplotScript.is_open()) {
		
        gnuplotScript << "set terminal pngcairo enhanced font 'Verdana,12'\n";
        gnuplotScript << "set output 'Irragraph.png'\n";
        gnuplotScript << "set title 'I-V curve for a constant temperature'\n";
        gnuplotScript << "set xlabel 'Voltage (V)'\n";
        gnuplotScript << "set ylabel 'current (A)'\n";
        gnuplotScript << "set key bottom left\n";
        gnuplotScript << "plot '-' with lines title 'irradiance= " << irra << "W/m^2', '-' with lines title 'irradiance= " << irra2 << "W/m^2'\n";
		


        // Write data for power 1
        for (int i = 0; i < volt.size(); i++) {
			
            gnuplotScript << volt[i] << " " << current[i] << "\n";
			
        }
		
		
        gnuplotScript << "e\n";

        // Write data for power 2
        for (int i = 0; i < volt2.size(); i++) {
			
            gnuplotScript << volt2[i] << " " << current2[i] << "\n";
			
        }
		
        gnuplotScript << "e\n";

        gnuplotScript.close();

        // Run the Gnuplot script
        system("gnuplot plot_script.gnu");
		
    } 
	
	else {
		
        cout << "Error: Unable to create the Gnuplot script file." << endl;
		
    }
}







tuple<vector<double>, vector<double> , vector<double> , vector<double> ,vector<double> ,
	vector<double> > SetTemp(){

 const double Isc = 3.8, a = 0.00065, Tr = 298.15, q = 1.602 * pow(10, -19), Voc = 21.1,
		b = 0.08, Rs = 0.008 * 36, Rsh = 1000 * 36, n = 2.1 , k= 1.3806*pow(10,-23) , Ns= 36,
		error = 0.00001;
		

    // Variables for the solar cell model
    double irra = 0,irra2=0,  temp = 0, temp2=0, I0 = 0, Iph = 0, I = 0, I_func = 0, I_prime = 0,
	errorval = 0, I_0 = 0, expo = 0, V_max=0, new_V0=0, new_errorval=0 , new_V=0,new_V_func=0, 
	new_V_prime=0,V0=0;

    vector<double> volt, current, power, volt2, current2, power2;
	

    tie(V_max, irra, temp) = MaxVolatge(); // her it will ask the user for time and fetch irra and temp
	

    // Find the volt current and power for the first time the user entered 

    I0 = (Isc + (a*(temp-Tr)) ) / ( ( exp( (q*(Voc+(b*(temp-Tr)))) / (n * k * temp * Ns ) ) ) -1 );
	
    Iph =(Isc + ( a * (temp - Tr)) )* (irra/1000);


    for (int i = 0; i <= 100; i++){
		
		 V0 = (i) * (V_max / 100);
       	volt.push_back(V0);
		
	}

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
	

    // Find the volt current and power for the new Irradiance the user entered 
	
	
	
	cout << "Enter the new irradiance" << endl;
    cin >> irra2;
     
	cout<<endl;
	

    I0 = (Isc + (a*(temp-Tr)) ) / ( ( exp( (q*(Voc+(b*(temp-Tr)))) / (n * k * temp * Ns ) ) ) -1 );
	
    Iph =(Isc + ( a * (temp - Tr)) )* (irra2/1000);
	
	
	//Calculate the voltage after changing the irradiance
	new_V0 = Voc;
	do {
        
        new_V_func = Iph - I0 * ( ( exp((q * new_V0) / (n * k * temp * Ns) ) )- 1 )  - (new_V0 / Rsh);

        new_V_prime = ( (-I0) * ( q / (n * k * temp * Ns) ) * ( exp( ( q * new_V0 ) / (n * k * temp * Ns) ) ) ) - (1/Rsh);


        new_V = new_V0 - (new_V_func/new_V_prime);
        new_errorval = abs(new_V - new_V0);

        new_V0 = new_V;
		
    } while (new_errorval > error);
	
	
	//push data in Volt2
    for (int i = 0; i <= 100; i++){
		
		 new_V0 = (i) * (new_V / 100);
       	volt2.push_back(new_V0);
		
	}

    for( int i = 0; i <= 100; i++){
		try{

			I = 0;
			I_0 = Isc;

			do{
			

				I_func= I_0 - Iph + (I0 * (exp ( q *( volt2[i]+ (I_0 *(Rs)))/(n*k*temp*Ns))-1)) + ((volt2[i] +(I_0*(Rs) ))/Rsh);

				I_prime= 1 +  (I0*((( q *Rs)/(n*k*temp*Ns)) * (exp ((q*( volt2[i]+ (I_0 *(Rs))))/(n*k*temp*Ns)))))+ (Rs/Rsh);

				I = I_0 - (I_func / I_prime);
				errorval = abs(I - I_0);

				I_0 = I;

				if(errorval <= error){
					current2.push_back(I);
				}		

			}
			while(errorval > error);

		}
		catch (...){
			break;
		}
	}

    double powerval2 = 0, temp_power2=0, max_power2 =0;

	//Calculate the power
    for (int i = 0; i<=100; i++){
		
        powerval2 = volt2[i] * current2[i];
        power2.push_back(powerval2);

        if (powerval2 > temp_power2){
			
            max_power2 = powerval2;
			
        }
		
        temp_power2 = powerval2;
    }

    cout << endl;
    cout << "max power when the temperature= "<<temp<<" and the irradiance= "<<irra2 <<" is: "<< max_power2 << endl;
	cout<<endl;
	
	double Percentage_change = ((max_power2 - max_power) / max_power) * 100;
	
		//Report
		
		if(Percentage_change<0){
		cout<<"According to the output of the code, the original power output was "<<max_power<<
		" P and the new power output is "<<max_power2<<" P.This represents a decrease in power output of "<<Percentage_change<<
		"%. This means that the new power output is slightly lower than the original power output."<<endl;
		}
		
		else if(Percentage_change>0){
			cout<<"According to the output of the code, the original power output was "<<max_power<<
			" P, and the new power output is "<<max_power2<<" P.This represents an increase in power output of "<<Percentage_change<<
			"%. This means that the new power output is slightly higher  than the original power output."<<endl;
		}
		
		else {
			   cout << "According to the output of the code, the original power output was " << max_power <<
    " P, and the new power output is " << max_power2 << " P. This represents no change in power output with a percentage change of "<<Percentage_change<<
	"%. This means that the new power output is equal to the original power output." << endl;
		}
		
	
 

    generateGraph1(volt, power, volt2, power2,current,current2, irra, irra2);

  

    return make_tuple(volt, current, power, volt2, current2, power2);

}



#endif