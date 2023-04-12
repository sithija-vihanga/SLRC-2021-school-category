// Team Name: 

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
  
  
  
  
  
  //*****************************************************************

  
  while (robot->step(timeStep) != -1) {
  //*****************************************************************
    //type your code here
    
    
    
    
    
    
  //*****************************************************************
    
  };

 
  delete robot;
  return 0;
}