//====================================================
// Walker main program
// Walker026 add variable speed, combined motions and shorter sequences
// Walker025 add make it walk, sidestep and rotate on fixed speed and sequences!!
// Walker024 add initial folded to ready (unfolded) sequence
// Walker023 moved some variable declarations out of legs.h into legs.cpp
// Walker021 replaced array sizes with defined constants
// Walker020 changed some variables to structures, made constants CAPITALS
// Walker019 checkpoint
// Walker018 added body sequence logic
// Walker017 added start initial and final velocities in leg calculations
// Walker016 improved listing and corrections to leg calculations
// Walker015 added listing of leg path and corrections to leg calculations
// Walker014 added leg position tests
// Walker013 added leg position calculations
// Walker012 changed distance and radians to float
//====================================================

#include "debug.h"
#include "com.h"

// apparently need the following include here and in sbus.cpp
#include <FUTABA_SBUS.h> //note, FUTABA_SBUS.h was modified to "#define port Serial2"
#include "sbus.h"

// apparently need the following here and in servo.cpp
#include <PololuMaestro.h> // apparently need this here and in servo.cpp
#include "servo.h"

#include "legs.h"
#include "mode.h"

unsigned long last_loop_time;

//====================================================
// Walker setup
//========================================================
void setup(){
  int8_t indent = 0;
  Serial.begin(115200);
  boolean usb_connected = false;
  if(usb_connected){
    while (!Serial) { 
    } 
  }else{
    delay(1000);
  }
  Serial.println("Serial online"); 
  debug_clr_new_mode();
  const char *routine = "setup";
//  static const boolean local_debug = true;
//  if(!local_debug) indent = -1;
  LOCAL_DEBUG_ENABLED
  if(local_debug) DEBUG_PRINT_BEG(routine, indent);
  sbus_setup(indent+1);
  servo_setup();
//  body_setup();
  legs_setup(indent+1);
//  legs_position_tests();
  mode_setup(indent+1);
  last_loop_time = millis();
  if(local_debug) DEBUG_PRINT_END(routine, indent);
  com_LED_setup(indent);
} // end setup


//========================================================
// Walker loop
//========================================================
void loop(){
  //static unsigned long last_time = 0;
  const static int8_t indent = 0; // for debug prints
  unsigned long this_time = millis();
  static unsigned long led_time = millis();
  const static unsigned long led_period = 1000;
  boolean show_time = true;
  if(show_time){
    Serial.print("In loop, cycle time: ");
    Serial.print(this_time - last_loop_time);
    Serial.print(", mode: ");
    Serial.print(MODE_NAME[mode_mode_phase_get().mode]);
    Serial.print(", phase: ");
    Serial.println(mode_mode_phase_get().phase);
//    Serial.printf("%s, %s, %d\n", __FILE__, __func__, __LINE__);
  }
  last_loop_time = this_time;
//  servo_math_test();
//  servo_test();
  sbus_update(indent+1);
//  sbus_print_channels();
  mode_update(indent+1);
//  body_update();
//  position_update();
//  legs_update();
  mode_display_position();
  servo_print_angle();
  servo_print_target();
  debug_clr_new_mode(); // clear new_mode
//  // now wait until the planned period is over, if it's not over already
//  while((millis() - last_loop_time) < 50){
//    // do nothing
//  }
  this_time = millis();
  boolean low_voltage = com_batt_low(indent+1);
  if((this_time - led_time) > led_period){
    com_LED_off(indent+1); // turn the LED off (LOW is the voltage level) at the end of the period
    led_time = this_time;
  } else if(((this_time - led_time) > led_period>>1) || low_voltage){
    com_LED_on(indent+1); // turn the LED on at the end of the period or if the battery is low
     // turn the LED on (HIGH is the voltage level) half way through the period or if the battery is low
  }
  delay(50); // note, if this is larger than our minumum move time (100 msec) then the system will continually fall behind the current time
} // end loop



