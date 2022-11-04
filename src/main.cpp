/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Motor1               motor         1               
// Motor11              motor         11              
// Motor20              motor         20              
// Inertial3            inertial      3               
// Motor17              motor         16              
// Motor18              motor         18              
// Motor19              motor         19              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#define PI 3.1415926

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
void initialize(){
  Inertial3.calibrate();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Initializing...");
  waitUntil(!Inertial3.isCalibrating());
  Controller1.Screen.clearScreen();

}

/*
        =^           *********         @^       ***** =@^     
       @@@@@@@@@@.   @^     =@.        @^      *@^ @^ =@^     
     =@@@@@@@@@@.    @^     =@.        @^      *@^=@. =@^     
       =@OOOOOO@.    @@@@@@@@@.    =@. @^ =^   *@^@^  =@^     
       =@     =@.    @^     =@.    @^  @^ =@^  *@^=@. =@@     
       ,[@/[[[[[.    @^     =@.   ,@.  @^  \@. *@^.@\ @/@`    
       @@@@@@@@@     @@@@@@@@@         @       @@  @  @ @     
     ./@[\\..@/                   ,    @^   .  *@^@@`/^  \`   
         .@@@\.    ]]]]]]]]]]]]]       @^      *@^ ./^   ,@\  
     =@@@/[  [[@@/ ,[[[[[[[[[[[[     ,@/.      *@^.\/     ,/. 
*/

void print_logo(){
  Brain.Screen.print("   =^           *********         @^       ***** =@^     ");
  Brain.Screen.newLine();
  Brain.Screen.print("  @@@@@@@@@@.   @^     =@.        @^      *@^ @^ =@^    ");
  Brain.Screen.newLine();
  Brain.Screen.print("=@@@@@@@@@@.    @^     =@.        @^      *@^=@. =@^ ");
  Brain.Screen.newLine();
  Brain.Screen.print("  =@OOOOOO@.    @@@@@@@@@.    =@. @^ =^   *@^@^  =@^");
  Brain.Screen.newLine();
  Brain.Screen.print("  =@     =@.    @^     =@.    @^  @^ =@^  *@^=@. =@@ ");
  Brain.Screen.newLine();
  Brain.Screen.print("  ,[@/[[[[[.    @^     =@.   ,@.  @^  \\@. *@^.@\\ @/@` ");
  Brain.Screen.newLine();
  Brain.Screen.print("  @@@@@@@@@     @@@@@@@@@         @       @@  @  @ @     ");
  Brain.Screen.newLine();
  Brain.Screen.print("./@[\\..@/                   ,    @^   .  *@^@@`/^  \\`  ");
  Brain.Screen.newLine();
  Brain.Screen.print("    .@@@\\.    ]]]]]]]]]]]]]       @^      *@^ ./^   ,@\\ ");
  Brain.Screen.newLine();
  Brain.Screen.print("=@@@/[  [[@@/ ,[[[[[[[[[[[[     ,@/.      *@^.\\/     ,/. ");
  Brain.Screen.newLine();
}
void select_mode(int &mode){
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Please Select Mode: ");
  Controller1.Screen.newLine();
  Controller1.Screen.print("L2. Traditionnal Mode");
  Controller1.Screen.newLine();
  Controller1.Screen.print("R2. Advanced Mode");
  waitUntil(!Controller1.ButtonL2.pressing()&&!Controller1.ButtonR2.pressing());
  waitUntil(Controller1.ButtonL2.pressing()||Controller1.ButtonR2.pressing());
  if(Controller1.ButtonL2.pressing()) mode=1;
  if(Controller1.ButtonR2.pressing()) mode=2;
  Controller1.Screen.clearScreen();
}

void hint(int mode){
  Controller1.Screen.setCursor(1,1);
  Controller1.Screen.print("Your choice: Mode %d",mode);
  Controller1.Screen.newLine();
  Controller1.Screen.print("Press L2/R2 to quit");
  if(mode==2){
  Controller1.Screen.newLine();
  Controller1.Screen.print("Press X to reset");
  }
}
void print_v(motor m,int row){  //To monitor the motors
  //Brain Output
  Brain.Screen.setCursor(row,1);
  Brain.Screen.clearLine();
  Brain.Screen.print("Motor: V = %3.1f rpm ",m.velocity(rpm));
  //Cntroller Output
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(row,1);
  Controller1.Screen.print("%3.1f rpm",m.velocity(rpm));
}

void run(motor m,int scale){
  m.spin(fwd,scale,pct); //a short cut to start the motor
}

double calculate_theta(int x,int y){//as its name
  if(x>0) return atan(double(y)/x);
  else if(x<0) return PI+atan(double(y)/x);
  else{//if you can't calculate the angle,rotatation won't take effect!
   if(y>0) return PI/2;
   if(y<0) return -PI/2;
   else return 0;
  }

}
void get_translation_scale(double angle_,double scale,double &scale_a,double &scale_b,double &scale_c){
  //calculate what it takes for each motor to translate
  scale_a=scale*sin(angle_);
  scale_b=scale*cos(angle_+PI/6);
  scale_c=-scale*cos(angle_-PI/6);
} 

double get_rotation_scale(double rotate,double scale){
  //calculate what it take for each motor to rotate
  return rotate*scale;
}
/*
double PID(){
    double Kp=0.6,Ki=0.1,Kd=0.05;
    double delta_t= Brain.Timer.time(msec)-last_t;
    rotation_angle += 0.0031*rotation_scale*delta_t*6/180*PI; //get the rotation angle
    double actual_angle = Inertial3.rotation(degrees)*PI/180; //get the actual angle
    double error = rotation_angle-actual_angle;
    integral = integral + error*delta_t;
    double derivative = (error - previous_error) /delta_t;
    double correction_output = (Kp*error + Ki * integral + Kd * derivative)*10;
    previous_error = error;
    last_t = Brain.Timer.time(msec);
}
*/

void movement(motor a,motor b,motor c,double scale_a,double scale_b,double scale_c,double ROTATION,double CORRECTION){

  //calculate the actual volocity(also referred to as scale in this code) needed for each motor
  run(a,scale_a+CORRECTION);//top
  run(b,scale_b+CORRECTION);//bottom left
  run(c,scale_c+CORRECTION);//bottom right  
}




/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Initialize the inertial censor 
  initialize();
  print_logo();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  double scale_a,scale_b,scale_c;
  get_translation_scale(PI,60,scale_a,scale_b,scale_c);
  movement(Motor1,Motor11,Motor20,scale_a,scale_b,scale_c,0,0);
  wait(1,sec);
  get_translation_scale(PI/2,60,scale_a,scale_b,scale_c);
  movement(Motor1,Motor11,Motor20,scale_a,scale_b,scale_c,0,0);
  wait(800,msec);
  get_translation_scale(PI,60,scale_a,scale_b,scale_c);
  movement(Motor1,Motor11,Motor20,scale_a,scale_b,scale_c,0,0);
  wait(800,msec);
  movement(Motor1,Motor11,Motor20,0,0,0,0,0);
  run(Motor17,100);
  run(Motor18,100);
  run(Motor19,-100);


}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

  int mode;
  while(1){
  select_mode(mode);
  hint(mode);
  
  Brain.resetTimer();
  double last_t=0;
  double rotation_angle = PI;
  double integral=0;
  double previous_error=0;
  Inertial3.setRotation(180, degrees);
  while (1) {
    
    waitUntil(!Controller1.ButtonL2.pressing()&&!Controller1.ButtonR2.pressing());
    if(Controller1.ButtonX.pressing()){
      if(mode==2) {Inertial3.setRotation(180, degrees);rotation_angle = PI;} //calculate_angle = 0;}
      hint(mode);
    }
    

    int y= Controller1.Axis3.position(pct);
    int x= Controller1.Axis4.position(pct);


    //the scale of translation --> controlled by the joystick
    double scale = Controller1.Axis4.position(pct)*Controller1.Axis4.position(pct)+Controller1.Axis3.position(pct)*Controller1.Axis3.position(pct);

    double point_angle;
    //the angle must be revise by the 
    if(mode==1) x=0; point_angle = calculate_theta(y,x); 
    if(mode==2) point_angle = calculate_theta(y,x)-Inertial3.rotation(degrees)*PI/180; 

    //get the translation volocity
    double scale_a,scale_b,scale_c;
    get_translation_scale(point_angle,scale/100,scale_a,scale_b,scale_c);
    //get the rotation volocity
    double rotation_scale = get_rotation_scale(Controller1.Axis1.position(pct),0.1);
    //get the correction value (Using PID)
    double Kp=0.60,Ki=0,Kd=0.05;
    double delta_t= Brain.Timer.time(msec)-last_t;
    rotation_angle += 0.0031*rotation_scale*delta_t*6/180*PI; //get the rotation angle
    double actual_angle = Inertial3.rotation(degrees)*PI/180; //get the actual angle
    double error = rotation_angle-actual_angle;
    integral = integral + error*delta_t;
    double derivative = (error - previous_error) /delta_t;
    double correction_output=0;
    //std::cout << rotation_angle <<' '<<error<<std::endl;
    if (error>PI/10||error<-PI/10) correction_output = (Kp*error + Ki * integral + Kd * derivative)*100;
    previous_error = error;
    last_t = Brain.Timer.time(msec);
    
    //drive the motor!
    movement(Motor1,Motor11,Motor20,scale_a,scale_b,scale_c,rotation_scale,correction_output);

    //print the message on the screen
    //print_v(Motor1,1);
    //print_v(Motor11,2);
    //print_v(Motor20,3);
    //if (Controller1.ButtonX.pressing()) {sudden_break();break;}
    //last_t=Brain.Timer.time(msec);
    wait(10, msec);
        if(Controller1.ButtonR1.pressing()){
      run(Motor17,-100);
      run(Motor18,-100);
      run(Motor19,100);
    }else if(Controller1.ButtonL1.pressing()){
      run(Motor17,100);
      run(Motor18,100);
      run(Motor19,-100);
    }else{
      run(Motor17,0);
      run(Motor18,0);
      run(Motor19,0);
    }
  
    if(Controller1.ButtonL2.pressing()) break;
    if(Controller1.ButtonR2.pressing()) break;

  }
  }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
