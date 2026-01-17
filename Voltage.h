//This function will calculate the maximum voltage

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include<iostream>
#include<cmath>
#include <tuple>
#include"GetIrraTemp.h"

using namespace std;

tuple<double, double, double> MaxVolatge(){

    // VARIABLES 
	
	double irra=0, temp=0 , Kelvin=0 , I0=0, Iph=0 , I0nu=0 , Iode=0, I=0 ,V_func=0, 
		V_prime=0 ,errorval=0, V0 = 0, V=0;

	 const double Isc = 3.8, a = 0.00065, Tr = 298.15, q = 1.602 * pow(10, -19), Voc = 21.1,
		b = 0.08, Rs = 0.008 * 36, Rsh = 1000 * 36, n = 2.1 , k= 1.3806*pow(10,-23) , Ns= 36,
		error = 0.00001;
		

    V0 = Voc;

    tie(irra,temp) = TempIrraVal();



    // CONVERT TEMPERATURE TO KALVIN
    temp = temp +273.15;



    // CALCULATE THE VALUES OF I0 AND Iph
    
    I0 = (Isc + (a*(temp-Tr)) ) / ( ( exp( (q*(Voc+(b*(temp-Tr)))) / (n * k * temp * Ns ) ) ) -1 );
	Iph =(Isc + ( a * (temp - Tr)) )* (irra/1000);



    // GETTING THE VALUE OF V WHEN I = 0;

    do {
        
        V_func = Iph - I0 * ( ( exp((q * V0) / (n * k * temp * Ns) ) )- 1 )  - (V0 / Rsh);

        V_prime = ( (-I0) * ( q / (n * k * temp * Ns) ) * ( exp( ( q * V0 ) / (n * k * temp * Ns) ) ) ) - (1/Rsh);


        V = V0 - (V_func/V_prime);
        errorval = abs(V - V0);

        V0 = V;

    } while (errorval > error);
	
    // cout << "V max is: " << V << endl;

    return make_tuple(V, irra, temp);

}


#endif