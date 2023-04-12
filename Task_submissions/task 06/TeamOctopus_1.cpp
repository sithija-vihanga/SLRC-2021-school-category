//Team name : Team Octopus

#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>            
#include <webots/Motor.hpp>
#include <cstdlib>

#define TIME_STEP 32
using namespace webots;

int main(int argc, char **argv) {
  
  Robot *robot = new Robot();
  
  DistanceSensor *DS[10];                  
  Motor *wheels[2];
  
  char wheels_names[2][18] = {"left wheel motor", "right wheel motor"};                      //Initialize mototr names
  char dsNames[10][13] = {"IR1", "IR2", "IR3", "IR4", "IR5", "IR6", "IR7","ps2","ps5"};      //Initialize 7 ground IR sensors and ps2 and ps5 sensors

  
  for (int j = 0; j<9; j++){                                    //Enabling distance sensors using for a loop
     DS[j] = robot->getDistanceSensor(dsNames[j]);                 
     DS[j]->enable(TIME_STEP);                                      
 }
 
 for (int i = 0; i < 2; i++) {                                 //Initialize motors using for loop
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
  }

 
 double data[9];             //To store sensor readings
 int black_white[7];         //To identify black or white colors detected by ground sensors
 int turning_fac = 0;        //To store how much robot should rotate in a bend(+ and - signs represents opposite directions)
 int loop_counter = 0;       //To count number of loops 
 bool hard_turn = false;     //To identify whether the bend is hard turn or not(like 90 degree turn)
 int initial_count =-15;     //To initialize number of loops to count time
 double velocity;            //velocity given to motors with turning speeds
 double left_sensor;         //to store left sensor value (ps5)
 double right_sensor;        //to store right sensor value (ps2)
 int left_obstacle;          //To store 1 or 0 to detect obstacles
 int right_obstacle;         //To store 1 or 0 to detect obstacles
 int no_left_obstacles = 0;  //To count number of 1s for one obstacle
 int no_right_obstacles = 0; //To count number of 1s for one obstacle
 int counter_left =0;        //count number of pillars in left side without noise
 int counter_right =0;       //count number of pilars in right side without noise
 bool stopped =false;        //To store whether robot has stopped or not
  

 while (robot->step(TIME_STEP) != -1) {

       for (int j= 0; j<7; j++){                           
          data[j] = DS[j]->getValue();        //Read sensor values and store them 
          if (data[j] <60.0){                 //detect whether reading is black or white and store 1 or 0 respective to the colour.
            black_white[j] = 1;
            }
          else {
            black_white[j] = 0;
            }  
       }
       left_sensor = DS[8]->getValue();       //Read left sensor
       right_sensor = DS[7]->getValue();      //Read right sensor
       left_obstacle = 0;                     //Initialize left and right obstacles = 0
       right_obstacle = 0;
       if (right_sensor >80.0){              //to detect an obstacle at right side
          right_obstacle = 1;                
       }
       if (left_sensor >80.0){               //to detect an obstacle is at left side
          left_obstacle = 1;
       }
       
       
       if (left_obstacle == 1){           //count the number of 1s for one obstacle
             no_left_obstacles +=1;
             }
       else{
         if(no_left_obstacles >8 ){     //denoise the readings using number of 1s
            counter_left+=1; 
         }
         no_left_obstacles = 0;        //reset variable to detect other obstacles
       }
       
       if (right_obstacle == 1){          //count the number of 1s for one obstacle
             no_right_obstacles +=1;
             }
       else{
         if(no_right_obstacles >8){      //denoise the readings using number of 1s
           counter_right+=1;
         }
         no_right_obstacles = 0;         //reset variable to detect other obstacles
       }
           
       //to get smooth turns at bends detect whether turn is hard turn or not and turn according to that
       turning_fac =  black_white[6]*2+black_white[5]+black_white[4]-black_white[2]-black_white[1]-black_white[0]*2;
       
       
       if ((not stopped) and (  black_white[0] ==  black_white[6])  and ( black_white[1] ==  black_white[5])  and ( black_white[2] ==  black_white[4]) ){
           wheels[0]->setVelocity(4);          //move forward if detected colors are symmetrical
           wheels[1]->setVelocity(4); 
           } 
       else if (turning_fac != 0 and (black_white[3] == 0 )){     //if middle sensor detects black and turning_fac >0 stop the robot 
       
           wheels[0]->setVelocity(0);
           wheels[1]->setVelocity(0);
           if ( not hard_turn){                              
               hard_turn = true;                                //So store it has a hard turn
               initial_count = loop_counter;                    //store number of loops so we can measure time from there
               }
           if ((not stopped) and ( loop_counter<initial_count+12) and turning_fac>0){        //rotating the robot for 12 more loops of 32 miliseconds
               wheels[0]->setVelocity(-4);
               wheels[1]->setVelocity(+4);
                              }
           else if ((not stopped) and (loop_counter<initial_count+12) and turning_fac<0){   //rotating the robot for 12 more loops of 32 miliseconds
               wheels[0]->setVelocity(4);
               wheels[1]->setVelocity(-4);
                              }
           else{                                             //after the hard turn make hard turn = false and stop the robot
               wheels[0]->setVelocity(0);
               wheels[1]->setVelocity(0);
               hard_turn = false;
               } 
           }
       
           
       else if (turning_fac != 0  and black_white[3] == 1){    //if middel sensor is white and turning_fac not equals to 0 turn the robot with respective speeds          
            velocity = turning_fac*0.7;                        // velocity given by turing_fac
            if (velocity >2){
              velocity = 2;                                   //to keep max velocity as 2
              }
              if (velocity <-2){
              velocity = -2;                                 //to keep max velocity as 2
              }
            wheels[0]->setVelocity(4-velocity);              //move the robot with turning speeds
            wheels[1]->setVelocity(4+velocity); 
           }
       
       else{
           wheels[0]->setVelocity(0);                      //if not stop the robot
           wheels[1]->setVelocity(0);
           }
      //if robot detect a complete black line stop the robot
      if ((not stopped) and loop_counter>100 and ( black_white[0] == 0 and black_white[1] == 0 and black_white[2] == 0 and black_white[3] == 0 and black_white[4] == 0 and black_white[5] == 0 and black_white[6] == 0)){
           wheels[0]->setVelocity(0);              //stop the robot
           wheels[1]->setVelocity(0);
           std::cout <<"left side pillars -  "<<counter_left<<std::endl;               //print the number of pillars to the console
           std::cout <<"Right side pillars -  "<<counter_right<<std::endl;
           stopped = true;                                                                //Imply the robot has stopped
           }
       
         loop_counter+=1;                                                                     //Increase the number of loops
         }

  delete robot;
  return 0;
}
