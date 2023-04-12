//team name : Team Octopus

#include <webots/Robot.hpp> 
#include <iostream>
#include <webots/DistanceSensor.hpp>           
#include <webots/Motor.hpp>
#include <string.h>
#define TIME_STEP 32
#include <webots/Emitter.hpp>

using namespace webots;

Emitter *emitter;
Motor *wheels[2];
DistanceSensor *DS[13];   

char wheels_names[2][18] = {"Left_motor", "Right_motor"};                        //Names of the wheels             
char dsNames[12][8] = {"ds_1", "ds_2", "ds_3", "ds_4", "ds_5","ds_6","ds_7", "ds_8", "ds_9","ds_10","ds_11","ds_12"};     //Names of distance sensors
double data[7];                                                                  // variable to store sensor readings
double wallReadings[7];                                                          //store sensor readings of walls          
int distance[3] = {0,0};                                                         //To store the actual distances to pillars
int black_white[7];                                                              //variable to store processed sensor reading(1 or 0 depending on black and white)
double turning_fac = 0;                                                          //+ and - values describes rotating directions of the robot                 
int starting_loop =0;                                                            //variable to initialize number of loops which robot has executed
int loop_counter = 0;                                                            //counting number of loops
bool condition_R = false;                                                        //To indicate robot turning right
bool condition_L = false;                                                        //To indicate robot turning left
bool choiceL = false;                                                            //To indicate choosing path at 'T' junction
bool choiceR = false;                                                            //To indicate choosing path at 'T' junction 
bool isHardTurn = false;                                                         //To whether robot is turning 90 degrees
int whiteLineCounter = 0;                                                        //count number of white lines  so we can detect white boxes
int blackLineCounter = 0;                                                        //To identify whether robot has entered wall following
int whiteBoxes = 0;                                                              //To store how many white boxes that robot has passed
bool decision = false;                                                           
int lineFollower_action;
int forward_counter = 0;
double distanceToWall = 0;
int nWallReadings = 0;
bool secondBox = false;                                                          //Identify the second white box
int running_sum = 0;                                      
char boxColor;                                                                   //Store the color of the box at the end
int last_steps = 0;                                                              //number of steps to move forward before stopping on the last coloured box
bool on_line = false;
std::string message;                                                             //Message to be send to the reciever robot



void forward(int Lspeed , int Rspeed);                                           //function to move robot forward
void reverse();                                                                  //To reverse the robot
void turnRight();                                                                //To turn the robot to right 
void turnLeft();                                                                 //To turn the robot to left
void stop();                                                                     //To stop the robot
void readSensors();                                                              // To read the sensor values in each loop
void lineFollower();                                                             // Algorithm to follow the line
void observer();                                                                 // Observe and detect the pattern on the floor
void readWallSensors();                                                          //Reading wall sensors      
void distanceCounter();                                                          //Read the sensor values and convert them to actual distances
void boxDetector();                                                              //Detecting whether robot has passed a white box
void red_blue_detector();                                                        

int speed = 4;                                                                   //maximum angular velocity for wheels
int forward_speed = 2;

int main(int argc, char **argv) {
  
  Robot *robot = new Robot();
  
  emitter = robot->getEmitter("emitter");                
  for (int i = 0; i < 2; i++) {                                                  //Enabling motors using for loop           
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
  }
  
  for (int j = 0; j<12; j++){                                                    //Enabling distance sensors using for  loop
     DS[j] = robot->getDistanceSensor(dsNames[j]);                 
     DS[j]->enable(TIME_STEP);                                      
 }
 
  

 
   while (robot->step(TIME_STEP) != -1) {
        
       readSensors();                                             //Read sensor values in each loop
       if((boxColor == 'R') or (boxColor == 'B')){                //Check whether robot has reached to the final T junction
           red_blue_detector(); 
           } 
       if (last_steps == 0){                                                                            
             observer();                   //Detect the white lines
         }                                                
       
        loop_counter += 1;                                                     //Increment number of loops by 1
   }
   }
void forward(int Lspeed , int Rspeed){                                        //function to move the robot forward
        wheels[0]->setVelocity(Lspeed);
        wheels[1]->setVelocity(Rspeed);
 }
void reverse(){                                                              //function to reverse the robot  
        wheels[0]->setVelocity(-speed);
        wheels[1]->setVelocity(-speed);
 }
 void turnRight(){                                                           //function to turn the robot in right direction
        wheels[0]->setVelocity(speed);
        wheels[1]->setVelocity(-speed);
 }
 void turnLeft(){                                                            //function to turn the robot in left direction
        wheels[0]->setVelocity(-speed);
        wheels[1]->setVelocity(speed);
 }
 void stop(){                                                                //function to stop the robot
        wheels[0]->setVelocity(0);
        wheels[1]->setVelocity(0);
 }
 void readSensors(){
       for (int j= 0; j<7; j++){                           
          data[j] = DS[j]->getValue();                                      //Read sensor values and store them 
          if (data[j] <200.0){                                              //detect whether reading is black or white and store 1 or 0 respective to the colour.
            black_white[j] = 1;
            }
          else {
            black_white[j] = 0;
            }  
           
          }
        }
void readWallSensors(){
       for (int j= 7; j<12; j++){                           
          wallReadings[j-7] = DS[j]->getValue();                                      //Read sensor values and store them 

          }
        }
void boxDetector(){
      if( whiteLineCounter > 6){                                           //If number of white lines are more than 6 detect it as a white box
            whiteLineCounter = 0;                                           //reset the number of white lines
            whiteBoxes+= 1;
      }
} 
void red_blue_detector(){            //To identify when robot enter to the colored box
    if (((data[5] < 350) and (data[5] > 320)) or ((data[6] <350) and(data[6])>320) or((data[5] < 120) and (data[5] > 100)) or ((data[6] <120) and(data[6])> 100) ){ 
        last_steps += 1;
        
        if (last_steps>25){         //To move the robot 25 steps before stopping
          stop();
          }
        else{
            forward(4,4);
        }
        
        if(last_steps == 26){ 
        emitter->send( message.c_str(),(int)strlen(message.c_str()) + 1);      //Sending the message to reciever robot after stopping
        std::cout<<message<<" : "<<running_sum<<std::endl;                     //printing the running sum
        }
       }
     
  } 
            
 void distanceCounter(){
     readWallSensors();                                                      //read wall sensors
     for (int l =0 ; l<2; l++){
         distance[l] = 0;
     }
     if((wallReadings[4] > 150 )and  (wallReadings[4] < 350)){               // detect pillar in 13 cm range if sensor readings are in this range
         distance[0] = 13;                                                   
     }
     else if((wallReadings[4] <550 )and (wallReadings[4] >350) ){            // detect pillar in 18 cm range if sensor readings are in this range
         distance[0] = 18;
     }
     else if((wallReadings[4] <750)and (wallReadings[4] >550) ){            // detect pillar in 23 cm range if sensor readings are in this range
         distance[0] = 23;
     }
     else{
         distance[0] = 0;                                                   //If any sensor reading is not in the above range there is no pillar
     }
     
     if((wallReadings[0] > 150) and  (wallReadings[0] < 350 ) ){            //Do the same process as above and capture the distances to pillars at right
         distance[1] = 13;
     }
     else if((wallReadings[0] <550 )and (wallReadings[0] >350) ){
         distance[1] = 18;
     }
     else if((wallReadings[0] <750 )and (wallReadings[0] >550) ){
         distance[1] = 23;
     }
     else{
         distance[1] = 0;
     }
     
     if ((distance[0] > 0) and not secondBox){                               //Identify pillar at left side
        std::cout <<"Pillar at left side – "<<distance[0]<<std::endl;
        running_sum += 3*distance[0];                                       //Add value to running sum
     }
     if ((distance[1] > 0) and not secondBox){                              //Identify pillar at right side
        std::cout <<"Pillar at Right side – "<<distance[1]<<std::endl;    
        running_sum += 5*distance[1];                                       //Add the value to running sum
     }
     if (secondBox == true){                                                //Compare the distances to two pillars 
        if(distance[0] > distance[1] ){
          std::cout <<"Right side pillar is the closest"<<std::endl;
          
        }
        if(distance[1] > distance[0] ){
          std::cout <<"Left side pillar is the closest"<<std::endl;
        }
        running_sum += 3*distance[0];                                      //Add the values to running sum
        running_sum += 5*distance[1];
        secondBox = false;
     }
     

 }
        
 void lineFollower(){                                                        
     if ((black_white[5] == 1) and (black_white[6] == 1)){                //If 2 sensors infront of the robot are detecing white move forward
         speed = 4;
         forward(speed,speed);
         lineFollower_action = 0;
         
     }
    
     else{                                                               // If 2 sensors are not detecting both as white
       
         if (not((black_white[1] == 1) and (black_white[3] == 1))){      //If  both 2 sensor around central sensor, is not white and if one of them are white turn the robot to make both front sensors white
               if ( black_white[1] ==1){
                           turnLeft();
                           lineFollower_action = 0;
                           
                   } 
               if (black_white[3] ==1){
                     turnRight();
                     lineFollower_action = 0;
                    
             } 
          }
        if ((black_white[5] == 1) and (black_white[6] == 0)){            //If only one of the front sensors are white rotate the robot to get both sensors on to the white line
             if ((lineFollower_action == 2) and (forward_counter < 1)){
                 forward(4,4);
                 forward_counter += 1;  
               }
              else{
                turnLeft();
                lineFollower_action = 1;
                forward_counter = 0;
              }
             
             
     }
         else if ((black_white[5] == 0) and (black_white[6] == 1)){          //If only one of the front sensors are white rotate the robot to get both sensors on to the white line
             if ((lineFollower_action == 1) and (forward_counter < 1)){
                 forward(4,4);
                 forward_counter += 1;  
               }
               else{
                 turnRight();
                 lineFollower_action = 2;
              }
              
                 }
             
     }
 }
 
 void wallFollower(double range){
 
     turning_fac = 0;
     if ( (wallReadings[4] > range) and (wallReadings[3] > 1.5*range) and (wallReadings[2] > range) and (wallReadings[1] > 1.5*range) and (wallReadings[0] > range)){   //Initialize rough boundraries for the robot and keep the robot with in the boundraries
              forward(4,4);
                 
  }  
      if ((wallReadings[4] < range) and ( wallReadings[3] < 1.5*range)) {
           //turnRight
           
           turning_fac +=1;    //Indicate turning right
           
     }
     if ((wallReadings[0] < range) and ( wallReadings[1] < 1.5*range)){
           //turnLeft
         
           turning_fac -=1;    //Indicate turning left
       
     }
     if (((wallReadings[4] < range) and ( wallReadings[3] < 900 ) and ( wallReadings[3] > 1.5*range)) or ((wallReadings[0] > range) and (wallReadings[0] < 900) and ( wallReadings[1] < 1.5*range))) {
            //turnLeft
            
            turning_fac -=1;
           
     }
     if (((wallReadings[0] < range) and ( wallReadings[1] <900) and ( wallReadings[1] > 1.5*range)) or ((wallReadings[4] > range) and (wallReadings[4]< 900) and ( wallReadings[3] < 1.5*range)) ){
           //turnRight
          
           turning_fac +=1;
      
     }
     
     if (wallReadings[2] <500){                  //if there is an obstacle in front reduce the speed  
        forward_speed = 1;
     }
     else if (wallReadings[2] >500){
        forward_speed = 2;
        }
         
     
     forward(forward_speed+ turning_fac, forward_speed-turning_fac);    //Moving forward with differential speeds according to the truning_fac
    
     
 }
  
 
 void observer(){
       if (not(condition_L) and not(condition_R) and  (black_white[1] == 1) and (black_white[2] == 1) and (black_white[3] == 1) and (black_white[5] == 1)and (black_white[6] == 1)){
       //If all sensors are white activate line follower to follow the line
             blackLineCounter = 0;
             lineFollower();
             whiteLineCounter +=1;
             if (whiteBoxes == 1 and whiteLineCounter >10 and whiteLineCounter <12){
                 secondBox = true;
                 distanceCounter();
                 decision = (distance[0] > distance[1]);    //Indicate turning right
             }
             secondBox = false;
             
      
     
 }    else if ((whiteBoxes == 1) and not(on_line) ){          //To get the robot on to the white line with out detecting patterns for hard turns
             lineFollower();
             if ((black_white[0] == 0) and (black_white[4] == 0) ){
                   on_line =true;
             }
             
 }
      else if (not(condition_L) and not(condition_R) and not(choiceL) and not(choiceR) and not(condition_R) and  (black_white[2] == 1) and (black_white[5] == 1)and (black_white[6] == 1) and  (black_white[1] == 0) and  (black_white[3] == 0)){
       //If sensors detect  white line  activate line follower to follow the line
             blackLineCounter = 0;
             lineFollower();
             boxDetector();
             
     }
       else if (not(condition_L) and not(condition_R) and not(choiceL) and not(choiceR) and not(condition_R) and(black_white[0] == 1) and (black_white[1] == 1) and (black_white[2] == 1) and (black_white[3] == 1) and (black_white[4] == 1) and (black_white[5] == 0)  and (black_white[6] == 0)){
       //If all sensors are white excpet front 2 sensors , stop the robot beacuse it is a 'T' junction
             stop();
             blackLineCounter = 0;
             
             
             
             if ((whiteBoxes >= 4) and not(choiceR) and not(choiceL)){
               if( running_sum%2 == 0){              //deciding the color of the box
                   choiceL = true;                   //turning left
                   boxColor = 'B';
                   message = "Blue";
               }
              if( running_sum%2 == 1){              //deciding the color of the box
                  choiceR = true;                   //turning right
                  boxColor = 'R';
                  message = "Red";
              }
              
              }
             if (whiteBoxes == 2){            //turn the robot at the first junction
              
                 if (decision == true){  
                       choiceL = true;
                      
                       
                     }
                 else if (decision == false){
                       choiceR = true;
                       
                    }
                 whiteBoxes += 2;            //Increse number of white boxes
             }
            
            
            
             
             
     
 }            
       else if ((((black_white[0] == 1) and (black_white[1] == 1) and (black_white[2] == 1) and (black_white[4] == 0))  or condition_L or choiceL) and not choiceR and not(condition_R) ){   //turn left
       //If sensors on the left detecting white , it is a 90 degree turn so rotate robot    
                 boxDetector();
                 blackLineCounter = 0;
                 if (not(condition_L)){                       //counting number of loops to keep the rotating function untill robot turn 90 degrees
                 condition_L = true;
                 
                 distanceCounter();           
                 starting_loop = loop_counter;
                
                 
             }
             if ((loop_counter - starting_loop) <= 19){       //It takes 19 loops to rotate nearly 90 degrees
                 turnLeft();
             } 
             else{
                 forward(speed,speed);                       //After rotating move the robot forward to avoid detecting same junction again
             }
             if ((loop_counter - starting_loop) > 22){      //Move the robot forward to avoid detecting noises
                  condition_L = false;
                  choiceL = false;
                 
                
             }
     
 }    
       else if ((((black_white[2] == 1) and (black_white[3] == 1) and (black_white[4] == 1) and (black_white[0] == 0)) or condition_R or choiceR )and not choiceL and not(condition_L)){      //turn right
             //If sensors on the right detecting white , it is a 90 degree turn so rotate robot 
             boxDetector();
             blackLineCounter = 0;
             if (not(condition_R)){                         //counting number of loops to keep the rotating function untill robot turn 90 degrees
                 condition_R = true;
                 
                 distanceCounter();
                 starting_loop = loop_counter;
                
             }
             if ((loop_counter - starting_loop) <= 19){     //It takes 19 loops to rotate nearly 90 degrees
                 turnRight();
             } 
             else{
                 forward(4,4);
             }
             if ((loop_counter - starting_loop) > 22){      //After rotating move the robot forward to avoid detecting same junction again
                  condition_R = false;
                  choiceR = false;
                  
            
                 }
             } 
       else if ((black_white[0] == 0) and(black_white[1] == 0) and (black_white[2] == 0) and (black_white[3] == 0) and (black_white[5] == 0)and (black_white[6] == 0)){
             //If all sensors detect black check for wall following or dotted line
             blackLineCounter+=1;
             if (blackLineCounter >10){     //To detect wall following region
                 readWallSensors();   
                 
                 if ( nWallReadings <6 ){                 //Take some average sensor readings to calculate boundraries
                   if( wallReadings[4] < 900 ){
                     distanceToWall += wallReadings[4];
                     nWallReadings += 1;
                     }
                   if( wallReadings[0] < 900 ){
                     distanceToWall += wallReadings[0];
                     nWallReadings += 1;
                     }
                     
                 }
                 if (nWallReadings == 6 ){               //Calculate average distance to wall
                     distanceToWall = distanceToWall/nWallReadings ;
                     nWallReadings = 7;
                    }
                 if (nWallReadings == 7){
                       wallFollower(distanceToWall);       //Send average distance to walls to wall follower
                 }
                 
                 
             }
             else{
                lineFollower();
                boxDetector();
             }
             
           } 
       else{
          
           lineFollower();     //If sensors are not detecting white line or junction or hard turn , initiate the line follower to adjust the robots path       
           boxDetector();
       }
 } 
