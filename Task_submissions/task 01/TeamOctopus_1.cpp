// Team Name: Team Octopus

#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <iostream>
#include <stdio.h>
//#include <algorithm>
#include <bits/stdc++.h> 
using namespace std; 

using namespace webots;

int main(int argc, char **argv) {

  Robot *robot = new Robot();
  int timeStep = 64;
  DistanceSensor*ds;
  ds=robot->getDistanceSensor("distance sensor");
  ds->enable(timeStep);
  
  Motor *motor_1=robot->getMotor("motor");
  motor_1->setPosition(INFINITY);
  motor_1->setVelocity(0.5);
  //*****************************************************************
  //define your variables here
  double Distance;                           //value given by the distance sensor
  double total = 0;                          
  int counter = 0 ;                         
  double average;                            //to get an average value of sensor readings
  double distance_to_pillers[10] ;           // an array to hold distances
  int i =0;
  int no_loops = 0 ;                         //to read how many times that loop have been iterated
  
   
  //*****************************************************************

  
  while (robot->step(timeStep) != -1) {
  //*****************************************************************
    //type your code here
    
    Distance = ds->getValue();                 //to read sensor values
    if ((Distance>0) and (no_loops < 191)){    // Distance>0 is used to find whether there is a piller or not.no_loops for 0ne spin is approximately 190.so we can stop after one spin
      total+=Distance;     
      counter+=1;                  //to find how many numbers that have been added to total
      }
    
    if ((Distance == 0) and (no_loops < 191)){  //Distance == 0 is used to indicate there is no piller.so we can compute average from above values
      if (total>0){
        
        average = total/(counter);
        if ((average >59.6) and (average <858.4)){  //59.6 refers to 100cm limt and 858.4 refers to 20cm limit
         distance_to_pillers[i] = average;          //insert values to the array
         i+=1;                                      //count number of elements in the array
         }
        
        }
      total= 0;                           //reset total and counter to read values of other pillers
      counter =0;
      
      }
      
      if (no_loops == 192){          //to indicate motor has spun one round
         double max;                 //variable to hold max value
         int index;                  // variable to hold index of an element
       
         for (int j = 0; j<i; j++){                 //for loop to iterate through the elements of the array to find max 
           max = *max_element(distance_to_pillers,distance_to_pillers+i+1);        //find the max value
           auto dis =find(distance_to_pillers,distance_to_pillers+i+1,max);        //find the index of the max value
           index = distance(distance_to_pillers,dis);
           std::cout <<"Object" <<" " <<index+1 <<std::endl;                      //to give output to the console object + index
           distance_to_pillers[index] = 0.0;                               //set max value to 0 so that in next iteration we can find the second largest element
             
          }
          
        }
        no_loops+=1;                                      //incresing number of loops to calculate number of times that loop has iterrated
        
  //*****************************************************************
    
  };

 
  delete robot;
  return 0;
}