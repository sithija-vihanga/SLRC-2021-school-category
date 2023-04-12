//Team Name :- Team Octopus

#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/LED.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#define TIME_STEP 32
using namespace webots;


int main(int argc, char **argv) {

  Robot *robot = new Robot();
  
  //-----------------------------------
  // Type your code here
  Motor *wheels[2];                                   
  DistanceSensor *DS[7];
  
  char wheels_names[2][8] = {"Motor_L", "Motor_R"};                                //store names of motors
  char dsNames[7][13] = {"Sensor_RM", "Sensor_LM", "Sensor_RF", "Sensor_RB", "Sensor_LF", "Sensor_LB", "Sensor_Front"};         //store names of the sensors
  
  for (int i = 0; i < 2; i++) {                                      //Initializing motors using a for loop
    wheels[i] = robot->getMotor(wheels_names[i]);
    wheels[i]->setPosition(INFINITY);
    wheels[i]->setVelocity(6);                                      //set initial velocity to 6
  }
  
   for (int j = 0; j<7; j++){                                      //Initialize distance sensors using a for loop
     DS[j] = robot->getDistanceSensor(dsNames[j]);                 
     DS[j]->enable(TIME_STEP);                                      
 }
   
  double data[7];                           //To store sensor readings
  int turning_fac = 0;                      //To store how much turning is needed
  double max_speed;                         //max forward velocity
  double speed;                             // sum of max velocity + turning_fac
  bool turning = false;                    //To determine whether it is moving forward or it is turning
  int time = 0;                            //To count the time when robot is turning
  
  
   while (robot->step(TIME_STEP) != -1) {
     
     if (not turning){                    //To start the loop when robot is moving forward
       for (int j= 0; j<7; j++){          //Read all the sensor values and store them in an array using a for loop       
          data[j] = DS[j]->getValue();   
       }
       // Below function is used to emphasis the effect of front sensors more than middle sensors and get the least effect by sensors at back.sign of the value is used to determine whether which direction  the robot should rotate.
       
       turning_fac = (data[2]-data[4])*8/(data[2]+data[4]) + (data[0]-data[1])*4/(data[0]+data[1]) +(data[3]-data[5])*2/(data[3]+data[5]);
       if(turning_fac>4){              //To make maximum turning_fac value to 4 to prevent unstable conditions.
         turning_fac = 4;
         }
       max_speed  = 5 - 0.5*turning_fac;     //Changing maximum forward speed along with turning_fac.   ex:- If higher turning_fac lower moving speed if lower turning_fac higher moving speed.
       
       if (turning_fac > 0){                //If turning_fac >0 turn the robot clockwise.
       speed = max_speed+turning_fac;
   
         wheels[0]->setVelocity(speed);    //Write the velocity to left motor.
       }
       else {                              //If turning_fac < 0 turn the robot anti - clockwise.
         speed = max_speed+turning_fac*(-1);
      
         wheels[1]->setVelocity(speed);    //Write the velocity to right motor.
       }
       
       if(data[6] > 28 and data[4] >78 and data[2] >78 ){     //To detect whether robot has come out of the walls
            wheels[0]->setVelocity(0);                       //Stop the robot
            wheels[1]->setVelocity(0);
       
       }
       }
       if (data[6] <6){                                //To detect whether there is an obstacle in front.
         turning = true;                               //To stop moving forward and start turning.
         if (time>0 and time<48){                      //To turn the robot by 180 degrees
            wheels[0]->setVelocity(6.28);
            wheels[1]->setVelocity(-6.28);
            }
         if (time>47){                       //To stop turning after robot has rotated 180 degrees.
           turning = false;
           }
          time+=1;                          //To measure time using the time step.
     
         }

  }
    delete robot;
    return 0;
  
  //-----------------------------------
  
}




