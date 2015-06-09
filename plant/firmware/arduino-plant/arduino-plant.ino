/*
 * @date    15.03.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Firmware for a I2C plant device.
 * 
 * Commands:
 * - 0x01 : Opens solenoid valve.
 * - 0x02 : Closes solenoid valve.
 * - 0x09 : Retrieves the soil moisture percentage value.
 * - 0x0A : Retrieves the solenoid valve state.
 */

// i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x04

// relais
#define RELAIS_PIN 2

// soil moisture sensor
#define MAX_SENSOR_VALUE 1023.0  // max value of the analog input pin
#define SOIL_MOISTURE_SENSOR_PIN 0        // sensor pin
int sensor_value = 0;     // analog pin input value
float humidity = 0;         // soil moisture in percentage

// commands
#define CMD_OPEN_SOLENOID_VALVE 0x01
#define CMD_CLOSE_SOLENOID_VALVE 0x02
#define CMD_READ_SOIL_MOISTURE 0x09
#define CMD_READ_SOLENOID_VALVE_STATE 0x0A


// states
#define VALVE_CLOSED 0
#define VALVE_OPEN 1


int received_cmd = 0x00;
int valve_state = VALVE_CLOSED;


// function prototypes
void send_data();
void receive_data();
void clear_cmd();
void send_float(float data);
void send_humidity_value();
void send_solenoid_state_byte();
void open_solenoid_valve();
void close_solenoid_valve();


void setup() {
  pinMode(RELAIS_PIN, OUTPUT);
  Serial.begin(9600);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0; 
}


/*
 * Closes the solenoid valve.
 */
void close_solenoid_valve() {
 digitalWrite(RELAIS_PIN, LOW); 
  valve_state = VALVE_CLOSED;
}


/*
 * Opens the solenoid valve.
 */
void open_solenoid_valve() {
 digitalWrite(RELAIS_PIN, HIGH);
 valve_state = VALVE_OPEN;
}


/*
 * Main loop.
 */
void loop() {
  sensor_value = analogRead(SOIL_MOISTURE_SENSOR_PIN);    // read the value from the sensor
  humidity = (1.0 - ((float)sensor_value)/MAX_SENSOR_VALUE) * 100;
  
  if(received_cmd == CMD_OPEN_SOLENOID_VALVE) {
    open_solenoid_valve();
  } else if(received_cmd == CMD_CLOSE_SOLENOID_VALVE) {
    close_solenoid_valve();
  } else {
    
  }
}


/*
 * i2c receive data.
 */
void receive_data(int byteCount){ 
  while(Wire.available()) {
   received_cmd = Wire.read();
  }
}


/*
 * Sends humidity value as word over i2c.
 */
void send_humidity_value() {
  sensor_value = analogRead(SOIL_MOISTURE_SENSOR_PIN);    // read the value from the sensor
  //humidity = (1.0 - ((float)sensor_value)/MAX_SENSOR_VALUE) * 100;
  humidity = (((float)sensor_value)/MAX_SENSOR_VALUE) * 100;
  Serial.println(sensor_value);
  send_float(humidity);
}


/*
 * Sends a float over the i2c bus.
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
 * Sends solenoid state value as byte over i2c.
 */
void send_solenoid_state_byte() {
  char data[] = { valve_state };
  Wire.write(data, 1);
}


/*
 * i2c send data.
 */
void send_data(){
  if(received_cmd == CMD_READ_SOIL_MOISTURE) {
    send_humidity_value();
    clear_cmd();  
  } else if(CMD_READ_SOLENOID_VALVE_STATE) {
    send_solenoid_state_byte();
    clear_cmd();
  }
}
