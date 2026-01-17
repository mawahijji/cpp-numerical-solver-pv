#include<iostream>
#include <tuple>
#include<string>
#include "GetIrraTemp.h"
#include "Voltage.h"
#include "NewTemp.h"
#include "NewIrra.h"
#include "FindPower.h"
#include "NewTempIrra.h"
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(){
	
	vector<double> volt, current, power, volt2, current2, power2;
	
	//Call the Max_Power to print the the maximum power
	tie(volt, current, power) = Max_power();
	
	
	//Show the list for the user incase if they want to change one of the characteristics
	
	vector<string> choices = {"Change the irradiance", "Change the temperature" ,\
                           "Change bpth of the irradiance and temperature","Quit"};

	cout<<endl;
    int choice;
	
    while (true) {
		
		cout<<endl;
		
		//Entering one of the choices
        cout << "Please choose an option:\n";
		
        for (int i = 0; i < choices.size(); i++) {
            cout << i + 1 << ". " << choices[i] << '\n';
        }
		
        cin >> choice;
		cout<<endl;
        if (choice < 1 || choice > choices.size()) {
            cout << "Invalid choice. Please try again.\n";
        } else {
            if (choice == choices.size()) {
                cout << "Quitting the program...\n";
                break;
            }
            switch (choice) {
                case 1:
                    SetTemp();
					
                    break;
                case 2:
                    SetIrra();
					
                    break;
				case 3:
                    SetIrraTemp();
					
                    break;
                
            }
        }
    }
	
	cout<<endl;
	cout<<endl;
	
	cout<<"-----------------------------------------------------------------------------------"<<endl;
	
	cout<<endl;
	cout<<endl;
	
	cout<<"//This project was developed by Safia Babiker , Alaa Aldybous , Mawa Hijji."<<endl;
	cout<<endl;
	
	
	cout<<"//Safia Babiker : sadbabiker@effat.edu.sa "<<endl;
	cout<<"//Alaa Aldybous : alnaldybous@effat.edu.sa "<<endl; 
	cout<<"//Mawa Hijji    : maahijji@effat.edu.sa "<<endl;
	cout<<" https://pv-module.herokuapp.com/ "<<endl;
	
	cout<<endl;
	cout<<"//Instructor      : Dr.Narjjisse."<<endl;
	
	cout<<endl;
	
	cout<<"-----------------------------------------------------------------------------------"<<endl;
	
	cout<<endl;
	cout << "Thank you for using the program!" <<endl;
    cout << "We hope it was helpful for your needs." << endl;
    cout << "If you have any feedback or suggestions, please let us know." <<endl;
    cout << "Have a great day!" <<endl;

	
	

	return 0;
}

