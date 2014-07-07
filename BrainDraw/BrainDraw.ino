#include <AccelStepper.h> // Include the required libraries 
#include <AFMotor.h>
#include <math.h> 
#include <Brain.h>

int steprev = 200; // depending on your stepper motors you will use 200 or 400 steps per revolution

AF_Stepper motor1(steprev, 1); // creates stepper motor objects
AF_Stepper motor2(steprev, 2);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void forwardstep1() {  
  motor1.onestep(FORWARD, SINGLE);
}
void backwardstep1() {  
  motor1.onestep(BACKWARD, SINGLE);
}
// wrappers for the second motor!
void forwardstep2() {  
  motor2.onestep(FORWARD, SINGLE);
}
void backwardstep2() {  
  motor2.onestep(BACKWARD, SINGLE);
}

AccelStepper stepper1(forwardstep1, backwardstep1);
AccelStepper stepper2(forwardstep2, backwardstep2);

int displace = 1500; // intitial offset for your machine
int amp = 500;
Brain brain(Serial);

void setup()
  {  
    Serial.begin(9600);
  
    stepper1.setMaxSpeed(1000);
    stepper1.setAcceleration(100.0);
  //       
    stepper2.setMaxSpeed(1000);
    stepper2.setAcceleration(100.0);
  }
void loop()
  {
  float t = millis()/1000; // start the timer
  
  if (brain.update()) {
    int Ls =abs( brain.readDelta()/100);// Reads the Delta and Theta inputs
    int Rs = abs(brain.readTheta()/100);
    int lss = displace + amp*sin(Ls*t); // sinusoid function based on Delta and Theta inputs
    int rss = displace+ amp*cos(Rs*t);
  
    Serial.println(brain.readCSV());
    if (brain.readSignalQuality() == 200){
      stepper1.moveTo(displace);
      stepper2.moveTo(displace);
    }
    else{
    if (stepper1.distanceToGo() == 0)
    {
    stepper1.moveTo(lss);  
    }
    if (stepper2.distanceToGo() == 0)
    {
      stepper2.moveTo(rss);
    }
  }
  }
  stepper1.run();
  stepper2.run();
  }
