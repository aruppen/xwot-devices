/*
 * @date    27.06.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * This program simulates a simple I2C slave device. 
 * The arduino is connected to a DHT 22 sensor and responds to two different commands.
 * 
 * Commands:
 * - 0X01 : Reads the measured temperature value.
 * - 0X02 : Reads the measured humidity value.
 *
 *
 * Dependency: https://github.com/adafruit/DHT-sensor-library
 */

 
#include <Wire.h>
#include "DHT.h"
 
#define SLAVE_ADDRESS 0x04

#define DHT_PIN 4

// use DHT11 for dht 11 sensor
#define DHT_TYPE DHT22 

#define CMD_READ_TEMPERATURE 0x01
#define CMD_READ_HUMIDITY 0x02


float humidity = 0;
float temperature = 0;

DHT dht(DHT_PIN, DHT_TYPE);

uint8_t received_cmd = 0x00;

void send_humidity();
void send_temperature();
void send_float(float f);
void send_data();
void receive_data(int byte_count);


void setup() {
  Serial.begin(9600);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
}


#define INTERVAL 2000
unsigned long time = millis();


/*
 * Main loop.
 */
void loop() {
  if(millis() - time > INTERVAL) {
    time = millis();
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
  }
}


/*
 * Received data over i2c bus.
 */
void receive_data(int byte_count){ 
  while(Wire.available()) {
    received_cmd = Wire.read();
  }
}


/*
 * Send temperature value.
 */
void send_temperature() {
  send_float(temperature);
}


/*
 * Send humidity value.
 */
void send_humidity() {
  send_float(humidity);
}


/*
 * Send data over i2c bus.
 */
void send_data(){
  if(received_cmd == CMD_READ_HUMIDITY) {
   send_humidity();
  }
  
  if(received_cmd == CMD_READ_TEMPERATURE) {
   send_temperature(); 
  }
  
  received_cmd = 0x00;
}


/*
 * Send float value over the i2c bus.
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
