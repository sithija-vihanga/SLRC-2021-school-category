//Team -  Team Octopus

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

//---------------------------------------
  //Import the libraries you need here
#include <webots/InertialUnit.hpp>
#include <webots/LED.hpp>
#include <cmath>                                
                                         //Imported cmath to compute absolute values
//---------------------------------------
using namespace webots;

#define TIMESTEP 64
#define MAX_SPEED 6.28
#define SCALE 0.6

int main() {
  Robot *robot = new Robot();
  
  /*---------------------------------------------------------------------
    Get the devices from the implemented robot and name them in order to
    use them in your code.
    Note that the names of the required nodes are,
        Inertial unit : inertial unit
        Green LED : led_green
        Red LED : led_red
 */
  
  int num_ramps = 0;                        //variable to store number of ramps 
  double total = 0 ;                        //to store sum of all angles given by inertial unit corresponding to given axis
  int no_of_loops = 0;                      //To calculate number of loops passed
  int condition = 3;                        //This variable holds -1,0,1 respectively for inclined down ,horizontal and inclined up.3 is assigned at first to avoid confussion with previous values
  double data[3] ={0,0,0};                  //To store 3 consecutive condition values to decide whether it is horizontal or inclined
  int status[3]= {0,0,0};                   //To store 3 consecutive values horizontal inclined up or inclined down to decide whether it is  a ramp or not.
  int temp= 3;                              //To temperary store condition's value while processing.
  
  InertialUnit *imu;                                          //To initialize inertial unit
  imu = robot->getInertialUnit("inertial unit");          
  imu->enable(TIMESTEP);                                           //Enable inertial unit
  
  LED *led[2];
  char led_name[2][10] = {"led_green","led_red"};                 //store leds' names
  
  for( int i = 0; i<2; i++){
    led[i] = robot->getLED(led_name[i]);                   //initialize 2 leds using a for loop
  }
//--------------------------------------------------------------------
  DistanceSensor *s1 = robot->getDistanceSensor("so");
  Motor *back_left_motor = robot->getMotor("back left wheel");
  Motor *back_right_motor = robot->getMotor("back right wheel");
  Motor *front_left_motor = robot->getMotor("front left wheel");
  Motor *front_right_motor = robot->getMotor("front right wheel");
  
  back_left_motor->setPosition(INFINITY);
  back_right_motor->setPosition(INFINITY);
  front_left_motor->setPosition(INFINITY);
  front_right_motor->setPosition(INFINITY);

  back_left_motor->setVelocity(0.0);
  back_right_motor->setVelocity(0.0);
  front_left_motor->setVelocity(0.0);
  front_right_motor->setVelocity(0.0);
  
  while (robot->step(TIMESTEP) != -1) {
  
    back_left_motor->setVelocity(SCALE * MAX_SPEED);
    back_right_motor->setVelocity(SCALE * MAX_SPEED);
    front_left_motor->setVelocity(SCALE * MAX_SPEED);
    front_right_motor->setVelocity(SCALE * MAX_SPEED);
    
    /*-------------------------------------------------------------------
      Type the required code needed to light the LEDs accordingly
        
      Develop an algorithm to count the number of ramps and 
      name it as "num_ramps" so that that could be displayed 
      which is coded already in the following piece of code. 
      
    */
    s1->enable(TIMESTEP);                                //enable distance sensor
    if(condition == 1){                                  //check whether it is inclined upwards
        led[0] ->set(1);                                 //light up green led
      }
    else if (condition == -1){                           //check whether it is inclined downwards
      led[1] ->set(1);                                   //light up red led
      } 
    else{
      led[0] ->set(0);                                   //if it is horizontal to off both bulbs
      led[1] ->set(0);
      }
     
    total += imu->getRollPitchYaw()[0];                  //get sum of all angles so effect of small noises can be reduced

    data[0] = data[1];                                   //To change data arrays values and update new value to array
    data[1] = data[2];
    data[2] = total;
     if (no_of_loops <3){                                //It takes 3 loops to complete the data array because there are 3 variables.so to decide inclinedness we directly use angles' value because there is no much noise at the beginning.
         if (imu->getRollPitchYaw()[0] <0.1 and imu->getRollPitchYaw()[0] > -0.1){   //if angle given by imu is small (-0.1<angle <0.1) to reduce noise
            condition = 0;                                                          // condition's value for horizontal plane
             } 
         if (imu->getRollPitchYaw()[0] >0.15 ){                                    //if angle is positive (angle>0.15)   to reduce noise
            condition = 1;                                                         //condition's value for inclined up
             } 
         if (imu->getRollPitchYaw()[0] <-0.15){                                     //if angle is negative (angle<-0.15)  to reduce noise
            condition = -1;                                                         //condition's value for inclined down
             } 
         }            
      
     if (no_of_loops > 3) {                                      //seperate the area with much noise due to combination of more inclined planes
         if ((data[0] < data[1]) and (data[1] < data[2])){       //compare values of total of angles and find whether it is increasing
             condition = 1;
         }
         if ((data[0] > data[1]) and (data[1] > data[2])){        //compare values as the previous one and find wheter it is decreasing
             condition =-1;
          }
         if (abs(data[0] - data[1]) < 0.1 and (abs(data[1]-data[2])< 0.1)){   //To compare absolute values of three total values and check whether they are approximately equal
             condition = 0;
         }
      }
     
     if (temp != condition){                         //To filter when temp not equals to condition (it was initialized with 3)
           temp = condition;                        
           status[0] = status[1];                   //To upload values of consecutive inclination values(condition variable) to status array to decide whether we have passed a ramp or not
           status[1] = status[2];
           status[2] = temp;
        }
     if (status[0] == 1 and status[1] == 0 and status[2] == -1){   //if status value equals to 1 0 -1 it means a ramp is there
             num_ramps +=1;                                         // increase number of ramps by 1
             for(int i=0; i<3; i++){                                //removing all values so it will not uploaded again with in the next iteration
                 status[i] = 0;
                 }  
         }
     no_of_loops += 1;                                             //increase number of loops by 1
     //----------------------------------------------------------------
     
    double distance = s1->getValue();
    if (distance>900.0){
      back_left_motor->setVelocity(0.0);
      back_right_motor->setVelocity(0.0);
      front_left_motor->setVelocity(0.0);
      front_right_motor->setVelocity(0.0);
      imu->disable();
      s1->disable();
      std::cout << "Number of ramps : " << num_ramps << std::endl;
      break;
    }
  };
  
  // -----------------------------------------
  //Enter clean up code here
  //--------------------------------------------
  delete robot;
  return 0;
}