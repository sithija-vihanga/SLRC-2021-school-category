// Team Name: Team Octopus

#include <webots/Receiver.hpp>
#include <iostream>
#include <math.h> 
#include <webots/Robot.hpp>
#include <webots/LED.hpp>

#define TIME_STEP 64

using namespace std;
using namespace webots;

Robot *robot = new Robot();

Receiver *receiver; 
char rec_name[10] = {"receiver"};

LED *led[4];
char led_name[4][6] = {"led_1","led_2","led_3","led_4"};


void initReceiver(){ //this function initialize the receiver. This is an example.Don't need to change anything in here.
  receiver = robot->getReceiver(rec_name);
  receiver->enable(TIME_STEP);
}

void initLED(){
//*******************************************************************************
//initialize the LEDs here
  for( int i = 0; i<4; i++){
    led[i] = robot->getLED(led_name[i]);                   //initialize 4 leds using a for loop
  }
  
//*******************************************************************************
}


void ReceiveAndDecode(){
//*******************************************************************************
//type your code to receive the message, decode it and get the required colour numbers
  if (receiver->getQueueLength() > 0) {                                   //check queue length before calling getData function
      string message((const char *)receiver->getData());
      receiver->nextPacket();                                             //To get next data packet
      string ledNumber_str = message.substr(0,1);                         //to slice the string and get emitter number
      string colorCode_str = message.substr(2,1);                         // to slice and get colour number
     
      int ledNumber = std::stoi(ledNumber_str);                           //to convert sliced string in to integer
      int colorCode = std::stoi(colorCode_str);                           //to convert sliced string to integer
      for (int j = 0; j < 4; j++){                                        //for loop is used to switch off all the other bulbs except the bulb in input data
         if (j!= (ledNumber-1)){                                          //when j not equal to led with input data j th bulb is disabled
             led[j] ->set(0);
         }
      }
      
      led[ledNumber-1] ->set(colorCode);                                  //turn on the respective bulb with given colour 
      }
//*******************************************************************************
}


int main(int argc, char **argv) {
   
    initReceiver();
    initLED();
    
//*********************************************************************************************
//call the ReceiveAndDecode() function and light up the LEDs in here.Use a while loop if needed.
while (robot->step(TIME_STEP) != -1) {         //TIME_STEP is used because emitter's time step also equals to TIME_STEP
     ReceiveAndDecode();                       //to iterate the recieveanddecode function so we can get all the inputs
    
    }
//**********************************************************************************************

    delete robot;
    return 0;
}