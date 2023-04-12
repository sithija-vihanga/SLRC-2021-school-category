#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

//---------------------------------------
  //Import the libraries you need here
  
  
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
        
        
  *//--------------------------------------------------------------------
  
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
      
    *//----------------------------------------------------------------
    
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
