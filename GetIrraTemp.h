//This function will read from excel file and let the user enter the time to get the
//Irradiance and the temperature

#ifndef GETIRRATEMP_H
#define GETIRRATEMP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>

using namespace std;

tuple<double, double> TempIrraVal(){

    // VARIABLES

    string line, field, filename, input_time, am_pm, data_time_after, x1hr, x2hr, data_time, data_am_pm, data_am_pmx2, hour, min, xam_pm, timex2;
    double temp, irra, xHour, xMin, x, x1,x2, dataHr, dataMin, dataMinx2, dataHrx2, G1, T1 , G2, T2, datax;
    double r1 = 0;
    double r2 = 0;
    vector <vector<string>> data;
    fstream datafile;

    temp = 0;
    irra = 0;

    double first_vale = 8;
    double last_vale = 16;



    // OPENING THE FILE

    filename = "test.csv";
    datafile.open(filename);


    // ADDING THE FILE ELEMENTS TO THE 2D VECTOR

   while (getline(datafile, line)){ // it will read each line wont stop at the commas 
        stringstream ss(line);
        vector<string> row;

        while(getline(ss, field, ',')){
            row.push_back(field);
        }

        data.push_back(row);
   }

   data.erase(data.begin());





   // FIND THE TEMPERATURE AND THE IRRADIANCE 


    try{


        while (true){

		
            cout << "Enter the time in format HH:MM AM/PM: ";
            getline(cin, input_time);





            // FINDING THE VALUE OF X

                    
            if (input_time.size() == 7){

                xHour = stoi(input_time.substr(0,1));
                xMin = stoi(input_time.substr(2,3));
                xam_pm = input_time.substr(5,6);
            }

            else if (input_time.size() == 8 ) {

                xHour = stoi(input_time.substr(0,2));
                xMin = stoi(input_time.substr(3,3));
                xam_pm = input_time.substr(6,8);
            }

            if (xam_pm == "PM" && xHour != 12){
                xHour = xHour + 12;
            }

            x = xHour + (xMin / 60);

            if (8 <= x and x <= 16){


                if (xam_pm == "AM" or xam_pm == "PM"){
                    break;
                }

                else {
                    cout << endl;
                    cout << "Use Capital PM / AM .... Try again..." << endl;
                    cout << endl;
                }
            }
            else{
                cout << endl;
                cout << "Error" << endl;
                cout << "The time You Entered is not in the data file "<<endl;
                cout << endl;
                continue;
            }

        }







        // FIND THE VALUE OF TEMPERATUE AND IRRADINCE WHEN IT IS DECLARE IN THE FILE 

        for (int i = 0; i < data.size(); i++){

            // this to check if the user's time is in the datafile 
            if (data[i][0] == input_time){

                temp = stoi(data[i][3]);
                irra = stoi(data[i][2]);

                return make_tuple(irra, temp);
            }
    
        }

        






        // FIND VALUE OF TEMPERATURE AND IRRADIANCE WHEN IT IS IN THE RANGE OF 2 TIMES 



        for (int i = 0; i < data.size(); i++){

            // Finding the value of the current time in the loop 
            string data_time = data[i][0];





            // finding the value of hr - min - am_pm 
            if (data_time.size() == 8){

                dataHr = stoi(data_time.substr(0,2));
                dataMin = stoi(data_time.substr(3,3));
                data_am_pm = data_time.substr(6,8);  

            }
            else if (data_time.size() == 7){

                dataHr = stoi(data_time.substr(0,1));
                dataMin = stoi(data_time.substr(2,3));
                data_am_pm = data_time.substr(5,6);

            }  

            if (data_am_pm == "PM" && dataHr != 12 ){
                dataHr = dataHr + 12;
            }
            

            // find the integer of the current time in the loop  
            datax = dataHr + (dataMin / 60);

            







            // if the value of the current time in the loop is smaller then the value of the time the user entered we will enter the if statement, the if statment will stop when the value of the current time in the loop is bigger than the time the user entered 
            if (x > datax){



                // fiding the value of x2, irradiance 1 and 2 and temperature 1 and 2 
                x1 = dataHr + (dataMin / 60);

                G1 = stoi(data[i][2]);
                T1 = stoi(data[i][3]); 

                G2 = stoi(data[i + 1][2]);
                T2 = stoi(data[i+ 1][3]);

                timex2 = data[i+1][0];


                // converting the x2 to integer 

                if (timex2.size() == 8){

                    dataHrx2 = stoi(timex2.substr(0,2));
                    dataMinx2 = stoi(timex2.substr(3,3));
                    data_am_pmx2 = timex2.substr(6,8);
                        

                }
                else if (timex2.size() == 7){

                    dataHrx2 = stoi(timex2.substr(0,1));
                    dataMinx2 = stoi(timex2.substr(2,3));
                    data_am_pmx2 = timex2.substr(5,6);

                }  

                if (data_am_pmx2 == "PM" && dataHrx2 != 12 ){
                dataHr = dataHr + 12;
                }


                // value of x2 as an integer 
                x2 = dataHrx2 + ( dataMinx2 / 60);

            }




            // CALCUATIONS




            // FINDING THE SLOPE a

            double ag = (G2 - G1) / (x2 - x1);
            double at = (T2 - T1) / (x2 - x1);


            // FINDING THE Y-INTERCEPT USING ONE OF THE POINTS 

            // y = ax + b 
            // b = y - ax 
            double bg = G2 - (ag) * (x2);
            double bt = T2 - (at) * (x2);


            // FINDING THE VALUE OF THE CORESPONDING IRRADIANCE AND TEMPERATURE 

            // y = ax + b
            irra =  (ag) * (x) + (bg);
            temp = (at) * (x) + (bt);



        }

        
        
        
        return make_tuple(irra, temp);

        }
        catch(...) {

            cout << "Error" << endl;
            cout << "Something Went Wrong  "<< endl;
            return make_tuple(r1,r2);
        }

    return make_tuple(irra, temp);
}


#endif
