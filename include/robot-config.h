using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor Motor1;
extern motor Motor11;
extern motor Motor20;
extern inertial Inertial3;
extern motor Motor17;
extern motor Motor18;
extern motor Motor19;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );