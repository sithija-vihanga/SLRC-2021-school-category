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
 for (int k = 0; k<5; k++){
   S[k] = robot->getDistanceSensor(dsNames[k]);
   S[k]->enable(TIME_STEP);
 }
   LED_show[0] ->set(0);
   LED_show[1] ->set(255);


  
  //---------------------------------------------------------------




  while (robot->step(TIME_STEP) != -1) {
  
    //--------------------------------------------------------------
    // Type yourcode here



       
    //---------------------------------------------------------------

















  }
}



