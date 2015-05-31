/*
 * @date    15.03.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Skeleton for a I2C plant device.
 * 
 * Commands:
 * - 0X01 : Opens solenoid valve.
 * - 0X02 : Closes solenoid valve.
 * - 0X03 : Retrieves the soil moisture percentage value.
 *
 */

// i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x04

// relais
#define RELAIS_PIN 2

// soil moisture sensor
#define MAX_SENSOR_VALUE 1023.0  // max value of the analog input pin
#define SOIL_MOISTURE_SENSOR_PIN A3        // sensor pin
int sensor_value = 0;     // analog pin input value
float humidity = 0;         // soil moisture in percentage

// commands
#define CMD_OPEN_SOLENOID_VALVE 0x01
#define CMD_CLOSE_SOLENOID_VALVE 0x02
#define CMD_READ_SOIL_MOISTURE 0x03
#define CMD_READ_SOLENOID_VALVE_STATE 0x05


// states
#define VALVE_CLOSED 0
#define VALVE_OPEN 1


int received_cmd = 0x00;
int valve_state = VALVE_CLOSED;

void setup() {
  pinMode(RELAIS_PIN, OUTPUT);
  Serial.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
}


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



void loop() {
  sensor_value = analogRead(SOIL_MOISTURE_SENSOR_PIN);    // read the value from the sensor
  humidity = (1.0 - ((float)sensor_value)/MAX_SENSOR_VALUE) * 100;
  
  //Serial.print("humidity: ");
  //Serial.print(humidity);
  
  //Serial.println("");
  //Serial.print("value: ");
  //Serial.print(sensor_value);
  //Serial.println("");
  
  if(received_cmd == CMD_OPEN_SOLENOID_VALVE) {
    open_solenoid_valve();
  } else if(received_cmd == CMD_CLOSE_SOLENOID_VALVE) {
    close_solenoid_valve();
  } else {
    
  }
}


void receive_data(int byteCount){ 
  while(Wire.available()) {
   received_cmd = Wire.read();
  }
}


void send_humidity_word() {
  sensor_value = analogRead(SOIL_MOISTURE_SENSOR_PIN);    // read the value from the sensor
  humidity = (1.0 - ((float)sensor_value)/MAX_SENSOR_VALUE) * 100;
  float hum = humidity;
  int8_t hum_i = (int8_t) hum;
  int8_t hum_f = (hum - hum_i) * 100;
  char data[] = { hum_i, hum_f };
  Wire.write(data, 2);
}

void send_solenoid_state_byte() {
  char data[] = { valve_state };
  Wire.write(data, 1);
}


void send_data(){
  if(received_cmd == CMD_READ_SOIL_MOISTURE) {
    send_humidity_word();
    clear_cmd();  
  } else if(CMD_READ_SOLENOID_VALVE_STATE) {
    send_solenoid_state_byte();
  }
}
