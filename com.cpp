//========================================================
// common.cpp
// common functions
//========================================================

#include "Arduino.h"
#include "debug.h"
#include "mode.h"
#include "com.h"


// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
const uint8_t LED_PIN = 13;
const uint8_t ANALOG_VIN = 0; // this is actually pin 14


//========================================================
// com_free_mem()
//========================================================
void com_free_mem(void){
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is the free, available ram.
    Serial.print(", free: ");
    Serial.println(stackTop - heapTop);
//  extern int __heap_start, *__brkval; 
//  int v; 
//  int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
//  Serial.print("com_free_ram: ");
//  Serial.println(fr);
} // end com_free_ram


//========================================================
// com_indent()
//========================================================
void com_indent(int8_t indent){
  for(int8_t i=0; i<indent; i++){
    Serial.print("|  ");  
  }
} // end com_indent


//========================================================
// com_print_beg()
//========================================================
void com_print_beg(const char *routine, int8_t indent){
  com_indent(indent);
  Serial.printf("Beg %s, millis(): %d\n", routine, millis());
} // com_print_beg


//========================================================
// com_print_beg2()
//========================================================
void com_print_beg2(const char *routine, const char *text, int8_t indent){
  com_indent(indent);
  Serial.printf("Beg %s: %s, millis(): %d\n", routine, text, millis());
} // com_print_beg


//========================================================
// com_print_end()
//========================================================
void com_print_end(const char *routine, int8_t indent){
  com_indent(indent);
  Serial.printf("End %s, millis(): %d\n", routine, millis());
} // com_print_end


//========================================================
// com_sign_mag
//========================================================
void com_sign_mag(float number, int8_t *sign, float *mag){
  static const boolean local_debug = false;
  if(number>=0.0){
    *mag = number;
    *sign = 1;
  } else {
    *mag = -number;
    *sign = -1;
  }
  if(local_debug){
    DEBUG_PRINT("In common_sign_mag, number: ");
    DEBUG_PRINTF("%7.2f", number);
    DEBUG_PRINT(", sign: ");
    DEBUG_PRINT(*sign);
    DEBUG_PRINT(", *mag: ");
    DEBUG_PRINTF("%7.2f\n", *mag);
  }
} // com_sign_mag


//========================================================
// com_err_msg_int
//========================================================
void com_err_msg_int(const char routine[], const char err_msg[]){
  Serial.print("COM_ERR_MSG: routine: ");
  Serial.print(routine);
  Serial.print(", error: ");
  Serial.println(err_msg);
} // end com_err_msg_int


//========================================================
// com_err_msg
//========================================================
void com_err_msg(const char routine[], const char err_msg[]){
  com_err_msg_int(routine, err_msg);
  Serial.println();
} 

//void com_err_msg(const char routine[], const char err_msg[], uint8_t uint8_t_value){
//  com_err_msg_int(routine, err_msg);
//  Serial.print(" uint8_t: ");
//  Serial.println(uint8_t_value);
//}
//
//void com_err_msg(const char routine[], const char err_msg[], float float_value){
//  com_err_msg_int(routine, err_msg);
//  Serial.print(" float: ");
//  Serial.println(float_value);
//}

void com_err_msg(const char routine[], const char err_msg[], uint8_t uint8_t_value, float float_value){
  com_err_msg_int(routine, err_msg);
  Serial.print(" uint8_t: ");
  Serial.print(uint8_t_value);
  Serial.print(", float: ");
  Serial.println(float_value);  
} // end com_err_msg


//========================================================
// com_batt_low()
//========================================================
boolean com_batt_low(int8_t indent){
  static float filtered_voltage = 12.0;
  static unsigned long old_millis = 0;
  const float TIME_CONST = 1000.0; // time constant in milliseconds
  uint16_t voltage_reading = analogRead(ANALOG_VIN);
  const float VOLTAGE_SCALE = (11.79 * 3.3) / (2.85 * 1024.0);  
  const float VOLTAGE_CUTOFF = 3.0 * 3.5; // don't allow the battery to go below 3.5 volts per cell!
  float voltage = float(voltage_reading) * VOLTAGE_SCALE;
  unsigned long new_millis = millis();
  float delta = float(new_millis - old_millis);
  old_millis = new_millis;
  filtered_voltage = (TIME_CONST * filtered_voltage + delta * voltage) / (TIME_CONST + delta); // filters the voltage
  boolean low_voltage = false;
  if(filtered_voltage < VOLTAGE_CUTOFF){
    low_voltage = true; // the battery is low
    com_LED_on(indent+1); // turn the LED on
    mode_shut_down(indent+1);
  }
  com_indent(indent+1);
  Serial.printf("low_voltage: %d, filtered_voltage: %2.3f, voltage: %2.3f, voltage reading: %d\n", low_voltage, filtered_voltage, voltage, voltage_reading);
  return low_voltage;
} // end com_batt_low


//========================================================
// com_LED_setup()
//========================================================
void com_LED_setup(int8_t indent){
  pinMode(LED_PIN, OUTPUT);
  com_LED_on(indent+1);  
} // end com_LED_setup


//========================================================
// com_LED_off()
//========================================================
void com_LED_off(int8_t indent){
  digitalWrite(LED_PIN, LOW);  
} // end com_LED_off


//========================================================
// com_LED_on()
//========================================================
void com_LED_on(int8_t indent){
  digitalWrite(LED_PIN, HIGH);  
} // end com_LED_on
