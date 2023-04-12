// Team Name: Team Octopus

#include <webots/Receiver.hpp>
#include <iostream>
#include <webots/Robot.hpp>
#include <webots/LED.hpp>
#include <string.h>

#define TIME_STEP 32

using namespace std;
using namespace webots;

Robot *robot = new Robot();

Receiver *receiver; 
char rec_name[10] = {"receiver"};                      //Name of the reciever

LED *led[2];
char led_name[2][10] = {"led_red","led_blue"};        //Names of LED s

void initReceiver(){            // initialize the receiver
  receiver = robot->getReceiver(rec_name);
  receiver->enable(TIME_STEP);
}

void initLED(){

  for( int i = 0; i<2; i++){
    led[i] = robot->getLED(led_name[i]);                   //initialize 2 leds using a for loop
  }
  
}


void ReceiveAndDecode(){               

  if (receiver->getQueueLength() > 0) {                                   //check queue length before calling getData function
      string message_1((const char *)receiver->getData());
      receiver->nextPacket();                                             //To get next data packet
    
     if (message_1 == "Red"){            //Check whether th message_1 is red
         led[0] ->set(1);                //Turn on the red LED
     }
     if (message_1 == "Blue"){           //Check whether th message_1 is blue
          led[1] ->set(3);               //Turn on the Blue LED
     }
     
   }

}


int main(int argc, char **argv) {
   
    initReceiver();              //Initialize the reciever 
    initLED();                   //Initialize LEDs
    

while (robot->step(TIME_STEP) != -1) {         
     ReceiveAndDecode();                       //To decode the message and perform the respective tasks
     
    }

    delete robot;
    return 0;
}