/*
 * @date    2.06.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * !!!PLEASE USE FOR THE RF433 MHZ TRANSMITTER 
 * A FULLY CHARGED 9 VOLT BATTERY!!!
 *
 *
 * Skeleton for a I2C light bulb device.
 * 
 * Commands:
 * - 0x01 : Turns the lightbulb on.
 * - 0x02 : Turns the lightbulb off.
 * - 0x09 : Retrieves the state of the lightbulb.
 * - 0x0A : Retrieves the illuminance value of the sensor.
 *
 */
 
// sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
 
// rc switch
#include <RCSwitch.h>
RCSwitch rc_switch = RCSwitch();
#define RC_SWITCH_PIN 3
#define ADDRESS_CODE 1
#define CHANNEL_CODE 4

// led
#define LED_PIN 11
#define LED_PIN_DEBUG 13
 
 // i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x04

// commands
#define CMD_LIGHT_ON 0x01
#define CMD_LIGHT_OFF 0x02
#define CMD_READ_STATE 0x09
#define CMD_READ_ILLUMINANCE 0x0A

// light sensor
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define LUX_TRESHOLD 1500
long lux_value = 0;

int received_cmd = 0x00;

void switch_on();
void switch_off();
void clear_cmd();
void receive_data(int byte_count);

void send_state();

void send_data(char c);
void send_int32(long value);
void send_int16(int value);
void send_float(float value);

void update_sensor_value();
void update_led();
int is_light_on();


void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN_DEBUG, OUTPUT);
  
  rc_switch.enableTransmit(RC_SWITCH_PIN);
  
  if(!tsl.begin()) { // i2c sensor
    Serial.println("Could not find a valid TSL2561 sensor, check wiring!");
  }
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
}


/*
 * Main loop.
 */

unsigned long last_update = millis();
#define INTERVAL 2000

void loop() {
  if(millis() - last_update > INTERVAL) {
    last_update = millis();
    update_sensor_value();
    update_led();
    digitalWrite(LED_PIN_DEBUG, HIGH);
  } else {
   digitalWrite(LED_PIN_DEBUG, LOW); 
  }
  
  if(received_cmd == CMD_LIGHT_ON) {
    switch_on();
    clear_cmd();
  } else if(received_cmd == CMD_LIGHT_OFF) {
    switch_off();
    clear_cmd(); 
  }
}


/*
 * Updates the illuminance sensor value.
 */
void update_sensor_value() {
  sensors_event_t event;
  tsl.getEvent(&event);
  lux_value = event.light; 
}


/*
 * Updates the led.
 */
void update_led(){
 if(lux_value > LUX_TRESHOLD) {
  digitalWrite(LED_PIN, HIGH); 
 } else {
  digitalWrite(LED_PIN, LOW); 
 }
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0; 
}


/*
 * Turns on the light.
 */
void switch_on() {
  rc_switch.switchOn(ADDRESS_CODE, CHANNEL_CODE);
}


/*
 * Turns off the light.
 */
void switch_off() {
  rc_switch.switchOff(ADDRESS_CODE, CHANNEL_CODE);
}


/*
 * i2c receive data.
 */
void receive_data(int byte_count){ 
  while(Wire.available()) {
   received_cmd = Wire.read();
  }
}


/*
 * i2c send data.
 */
void send_data(){
  if(received_cmd == CMD_READ_STATE) {
    send_state();
    clear_cmd();
  } else if(received_cmd == CMD_READ_ILLUMINANCE) {
    send_int32(lux_value);
    clear_cmd();
  }
}


/*
 * Send the state of this light bulb over the i2c bus.
 */
void send_state() {
  char data[] = {
    lux_value > LUX_TRESHOLD
  };
  Wire.write(data, 1);
}


/*
 * Sends a float in the big-endian format over the i2c bus.
 */
void send_float(float value_f) {
  float *v = &value_f;
  long *vl = (long*) v;
  long value_l = *vl;
  
  char data[4] = {
    (value_l & 0xff000000) >> 24,
    (value_l & 0x00ff0000) >> 16,
    (value_l & 0x0000ff00) >> 8,
    value_l & 0x000000ff
  };
  Wire.write(data, 4);
}


/*
 * Sends a byte over the i2c bus.
 */
void send_byte(char b) {
  char data[] = { b };
  Wire.write(data, 1);
}


/*
 * Sends an 32 bit integer in the big-endian format over the i2c bus.
 */
void send_int32(long value_l) {
  char data[4] = {
    (value_l & 0xff000000) >> 24,
    (value_l & 0x00ff0000) >> 16,
    (value_l & 0x0000ff00) >> 8,
    value_l & 0x000000ff
  };
  Wire.write(data, 4);
}


/*
 * Sends an 16 bit integer in the big-endian format over the i2c bus.
 */
void send_int16(int value_i) {
  char data[2] = {
    (value_i & 0xff00) >> 8,
    value_i & 0x00ff
  };
  Wire.write(data, 2);
}
