// Team name:  Team Octopus

#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/LED.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#define TIME_STEP 64
using namespace webots;
Motor *wheels[4];

void Stop(){
   for (int i = 0; i < 4; i++) {
      wheels[i]->setVelocity(0);
    }
}



int main(int argc, char **argv) {
  
  Robot *robot = new Robot();
  char wheels_names[4][4] = {"LF", "LB", "RF", "RB"};
  for (int i = 0; i < 4; i++) {
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(6);
  }
  
  
  
  //----------------------------------------------------------------
  // Define your variables here


  int counter = 0;                                             //To count number of white parts in one row
  int data[5];                                                 //To hold all the 5 values given by 5 sensors at a time
  int binary_data[5] = {0,0,0,0,0};                            //To hold 1 and 0 s respective to white and black by converting data's inputs
  double row[5] = {0,0,0,0,0};                                 //To identify new rows and hold 1,0 values of all rows at a time
  int state = 0;                                               //To hold state whether it is started or finished passing starting line or finishing line
  int rows_columns[100][5];                                    //2D array to hold all the 1,0 patterns of all rows with out starting and finishing line
  int no_rows =0;                                              //To hold number of rows
  int stopped = 0;                                             //To indicate whether robot has stopped or not
   
  //----------------------------------------------------------------
  
  
  
  LED *LED_show[7];
  DistanceSensor *S[5];
  char LED_names[7][10] = {"led1", "led2", "led3", "led4", "led5", "Start_LED", "End_LED"};
  char dsNames[5][3] = {"S1", "S2", "S3", "S4", "S5"}; 
  
  
  //---------------------------------------------------------------
  // Initialize sensors and LEDs here
  for( int j = 0; j<7; j++){
    LED_show[j] = robot->getLED(LED_names[j]);                   //initialize 7 leds using a for loop
  }
 for (int j = 0; j<5; j++){
   S[j] = robot->getDistanceSensor(dsNames[j]);                  //initoalize 5 IR sensors using a for loop
   S[j]->enable(TIME_STEP);                                      //Enabling 5 IR sensors
 }
    
  //---------------------------------------------------------------




  while (robot->step(TIME_STEP) != -1) {
  
    //--------------------------------------------------------------
    // Type yourcode here
   
    for (int j= 0; j<5; j++){                 
      data[j] = S[j]->getValue();                             //Reading values of 5 IR sensors and storing them in data variable
      } 
    for(int i = 0;i<5;i++){
      if (data[i]<270){                                      //Identify white colour even there are slight changes in light intensity
          binary_data[i] = 1;                                //Asign 1 for white colour and store it in binary_data array
      }
      if (data[i]> 990){                                     //Identify black colour even thre are slight changes in light intensity
          binary_data[i] = 0;                               //Asign 0 for black colour and store it in binary_data array
      }
    }
    for (int g = 0; g<5; g++){                             
      if(binary_data[g] == 1){                              //If color of a part of a row is white turn on the respective LED
            LED_show[g] ->set(255);                         // 255 is to give maximum intesity
            }
         }
      
    if( binary_data[0] != row[0] or  binary_data[1] != row[1] or  binary_data[2] != row[2] or  binary_data[3] != row[3] or  binary_data[4] != row[4] ){     //To identify whether row is different from the previous row ,so we can identify it as a new row 

        LED_show[5] ->set(0);                  //Turn off start LED when we found a new row
        for(int i =0; i<5; i++){                
            row[i] = binary_data[i];            //assign new 1,0 patterns of new row to row variable
            }
        counter = 0;                            //keep the counters value constant in every iteration
        for(int j =0; j<5; j++){
            if(row[j] == 1){                   //Increasing the counter by 1 if there is a white part 
                counter += 1;
                }
        if (counter == 5){                     //If there are 5 white parts ,identify it as start line or finish line and increase state variable by 1
            state +=1;
            }
        if (counter == 5 and state ==1){      //If there is a white line and state == 1 then it is start line, so light up start LED
            LED_show[5] ->set(255);
            }
               
         
        if (counter == 5 and state != 1){
            LED_show[6] ->set(255);           //If there is a white line ,it is not the start line then it is the finish line, so light up end LED
            Stop();                           //So stop the robot and increase stepped variable to indicate that robot has stopped
            stopped +=1;
            }
        
        }
        if ((row[0] != 1 or row[1] !=1 or row[2] != 1 or row[3] != 1 or row[4] !=1 )and  state >=1){   //if line is not completely white and if it has passed the start line,
            for (int s = 0; s<5; s++){ 
              rows_columns[no_rows][s] = row[s];                                                       //To upload those rows' patterns to rows_columns array
              }                                        // To print 1 and 0 respective to given colors detected by Led1 to Led5 , from left side of the console to right side of the console
            std::cout <<rows_columns[no_rows][0] << "   "<<rows_columns[no_rows][1] << "   "<<rows_columns[no_rows][2] << "   "<<rows_columns[no_rows][3]<< "   "<<rows_columns[no_rows][4] << "   "<<std::endl;     //To print those patterns to the console
            no_rows+=1;                                                   //Increase the number of rows by one
           
          }
          
        for (int b = 0; b<5; b++){
            LED_show[b] ->set(0);           //to turn off all leds at end of a row
            } 
  
       }        
       if(stopped == 1){
           std::cout <<"number of rows is   " << no_rows <<std::endl;        //To print number of rows at last
           stopped+=1;
         
       }
}
  delete robot;
  return 0;
//---------------------------------------------------------------
}



