#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#define TIME_STEP 32
#define START 0
#define END 1
using namespace webots;
using namespace std;



int main(int argc, char **argv) {

  Robot *robot = new Robot();

  DistanceSensor *dsSensor[2];
  char dsNames[2][9] = {"ds_start", "ds_end"}; 

  double starting_time = 0.0;
  double ending_time = 0.0;
  double total_time = 0.0;
  double remaining_time = 0.0;


  for (int i = 0; i < 2; i++){
    dsSensor[i] = robot->getDistanceSensor(dsNames[i]);
    dsSensor[i]->enable(TIME_STEP);
  }

  while (robot->step(TIME_STEP) != -1) {
    if(dsSensor[START]->getValue() < 650.0){
      starting_time = robot->getTime();
      cout<<"starting time: "<< starting_time <<endl;
      break;
    }
  }

  while (robot->step(TIME_STEP) != -1) {
    if(dsSensor[END]->getValue() < 680.0){
      cout<<" turn back "<<endl;
      break;
    }
  }

  while (robot->step(TIME_STEP) != -1) {
    if(dsSensor[START]->getValue() < 650.0){
      ending_time = robot->getTime();
      cout<<"ending time: "<< ending_time <<endl;
      break;
    }
  }
  total_time = ending_time - starting_time;
  remaining_time = 350.0 - total_time;
  cout<<"total time: "<< total_time <<endl;
  cout<<"time bonus: "<< (remaining_time*30)/350 <<endl;

  delete robot;
  return 0;
}
