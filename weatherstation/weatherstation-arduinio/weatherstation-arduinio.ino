/*
 * @date    05.06.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 * Firmware for the weatherstation device.
 * 
 * Commands:
 * - 0X01 : TODO
 * - 0X02 : TODO
 * - 0X09 : TODO
 *
 */

// sensor dependencies
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_TSL2561_U.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_TCS34725.h>

// tft dependencies
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>



// tft 
#define TFT_CS 10
#define TFT_DC 9
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


// weather station

// sensors
Adafruit_BMP085 bmp;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define DHT_PIN 2
#define DHT_TYPE DHT22
DHT_Unified dht(DHT_PIN, DHT_TYPE);


#define VERSION "Weather Station v1.0.0"

// i2c
#define SLAVE_ADDRESS 0x04

// commands
#define CMD_READ_TEMPERATURE_1 0x01
#define CMD_READ_TEMPERATURE_2 0x02
#define CMD_READ_PRESSURE 0x03
#define CMD_READ_HUMIDITY 0x04
#define CMD_READ_ALTITUDE 0x05
#define CMD_READ_ILLUMINANCE 0x06
#define CMD_READ_COLOR_K 0x07
#define CMD_READ_COLOR_LUX 0x08

void send_data();
void receive_data();
void clear_cmd();



// state variables
long illuminance_value = 0;
float pressure_value = 0.0;
float temperature_value1 = 0.0;
float temperature_value2 = 0.0;
float humidity_value = 0.0;
float altitude_value = 0.0;

int received_cmd = 0x00;

uint16_t color_temp = 0;
uint16_t color_lux = 0;
uint16_t color_r = 0;
uint16_t color_g = 0;
uint16_t color_b = 0;
uint16_t color_c = 0;

void update_sensor_values();

// init functions
void init_screen();
void init_sensors();

// print functions
void print_temp();
void print_pressure();
void print_humidity();
void print_illuminance();
void print_altitude();

void print_color();

void print_version();
void print_splash();
void print_clock();


/*
 * Setups the device.
 */
void setup() {
  Serial.begin(9600);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
  
  init_screen();
  init_sensors();
}


/*
 * Setups the tft screen.
 */
void init_screen() {
  tft.begin(HX8357D);
  tft.setRotation(3); // 1
  tft.fillScreen(HX8357_BLACK);
}


/*
 * Setups the sensors.
 */
void init_sensors() {
  if (!bmp.begin()) { // i2c sensor
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
  }
  
  if(!tsl.begin()) { // i2c sensor
    Serial.println("Could not find a valid TSL2561 sensor, check wiring!");
  }
  
  if(tcs.begin()) { // i2c sensor
    Serial.println("Could not find a valid TSC34725 sensor, check wiring!");
    tcs.setInterrupt(true);  // turn off LED
  }
  
  dht.begin(); // uses one-wire protocol
}


void print_splash() {
  tft.setCursor(DISPLAY_WIDTH/6, DISPLAY_HEIGHT/3);
  tft.setTextSize(3);
  tft.println("Weather station");
  tft.setCursor(DISPLAY_WIDTH/6, DISPLAY_HEIGHT/3 + 30);
  tft.println("loading...");
  delay(3000);
  tft.fillScreen(HX8357_BLACK);
}


/*
 * Prints the current version on the tft screen.
 */
void print_version() {
  tft.setTextSize(1);
  tft.setCursor(DISPLAY_WIDTH - 150, 10);
  tft.print(VERSION);
}


/*
 * Prints the current clock on the tft screen.
 */
void print_clock(){
 tft.setTextSize(1);
 tft.setCursor(DISPLAY_WIDTH - 150, 20);
 
 float tmp = millis() / (86400.0 * 1000);
 int days = tmp;
 tmp = (tmp - days) * 24;
 int hours = tmp;
 tmp = (tmp - hours) * 60;
 int minutes = tmp;
 int seconds = (tmp - minutes) * 60;
 
 tft.print(days);
 tft.print(" days, ");
 if(hours < 10) {
  tft.print(0);
 }
 tft.print(hours);
 tft.print(":");
 if(minutes < 10) {
  tft.print(0);
 }
 tft.print(minutes);
 tft.print(":");
 if(seconds < 10) {
  tft.print(0);
 }
 tft.print(seconds); 
}


/*
 * Prints temperature 1 on the tft screen.
 */
void print_temp1() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(0, 10);
  tft.println("Temperature 1: ");
  
  tft.setCursor(0, 30);
  tft.setTextSize(3);
  tft.print(temperature_value1);
  tft.println(" C    ");
}


/*
 * Prints temperature 2 on the tft screen.
 */
void print_temp2() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 70);
  tft.println("Temperature 2: ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 90);
  tft.setTextSize(3);
  tft.print(temperature_value2);
  tft.println(" C    ");
}


/*
 * Prints average temperature on the tft screen.
 */
void print_temp_avg() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 130);
  tft.println("Temperature avg:");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 150);
  tft.setTextSize(3);
  tft.print((temperature_value1 + temperature_value2)/2.0);
  tft.println(" C    ");
}

/*
 * Prints average temperature on the tft screen.
 */
void print_color() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 190);
  tft.println("Color: ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 210);
  tft.setTextSize(3);
  tft.print(color_lux);
  tft.println(" lx    ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 240);
  tft.setTextSize(3);
  tft.print(color_temp);
  tft.println(" K    ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 270);
  
  float r = 0;
  float g = 0;
  float b = 0;
  float sum = color_c;
  r = (color_r / sum) * 256;
  g = (color_g / sum) * 256;
  b = (color_b / sum) * 256;
  
  
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  Serial.print("C: "); Serial.print(color_c); Serial.print(" ");
  Serial.println(" ");
  
  if(g > 255 || g < 0) {
   g = 0; 
  }
  
  if(r > 255 || r < 0) {
   r = 0; 
  }
  
  if(b > 255 || b < 0) {
   b = 0; 
  }
  
  tft.setTextSize(2);
  tft.print("R: ");
  tft.print((int)r);
  tft.print(" ");
  tft.print("G: ");
  tft.print((int)g);
  tft.print("    ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 40, 290);
  tft.print("B: ");
  tft.print((int)b);
  tft.print("    ");
}


/*
 * Prints humidity on the tft screen.
 */
void print_humidity() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(0, 70);
  tft.println("Humidity: ");
  
  tft.setCursor(0, 90);
  tft.setTextSize(3);
  tft.print(humidity_value);
  tft.println(" %    ");
}


/*
 * Prints pressure on the tft screen.
 */
void print_pressure() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(0, 130);
  tft.println("Pressure: ");
  
  tft.setCursor(0, 150);
  tft.setTextSize(3);
  tft.print(pressure_value);
  tft.println(" pa   ");
}


/*
 * Prints illuminance on the tft screen.
 */
void print_illuminance() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(0, 190);
  tft.println("Illuminance: ");
  
  tft.setCursor(0, 210);
  tft.setTextSize(3);
  tft.print(illuminance_value);
  tft.println(" lx   ");
}


/*
 * Prints altitude on the tft screen.
 */
void print_altitude() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(0, 250);
  tft.println("Altitude: ");
  
  tft.setCursor(0, 270);
  tft.setTextSize(3);
  tft.print(altitude_value);
  tft.println(" m    ");
}

void update_sensor_values() {
  temperature_value1 = bmp.readTemperature();
  pressure_value = bmp.readPressure();
  altitude_value = bmp.readAltitude();
  
  sensors_event_t event;
  tsl.getEvent(&event);
  illuminance_value = event.light;
  
  dht.humidity().getEvent(&event);
  humidity_value = event.relative_humidity;
  dht.temperature().getEvent(&event);
  temperature_value2 = event.temperature;
  
  
  
  tcs.getRawData(&color_r, &color_g, &color_b, &color_c);
  color_temp = tcs.calculateColorTemperature(color_r, color_g, color_b);
  color_lux = tcs.calculateLux(color_r, color_g, color_b);
  
}


/*
 * main loop.
 */
int once = 1;
long last_update = millis();
#define INTERVAL 2000

void loop() { 
  if(once) {
    update_sensor_values();
    print_splash();
    once = 0;
  }
  
  print_version();
  print_clock();
  
  print_temp1();
  print_temp2();
  print_color();
  print_temp_avg();
  print_humidity();
  print_pressure();
  print_illuminance();
  print_altitude();
  
  if(millis() - last_update > INTERVAL) {
    update_sensor_values();
    last_update = millis();
    Serial.println("update!");
  }
  
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0; 
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
 * i2c send data.
 */
void send_data(){
  if(received_cmd == CMD_READ_TEMPERATURE_1) {
    // todo
    clear_cmd();  
  } else if(received_cmd == CMD_READ_TEMPERATURE_2) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_PRESSURE) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_HUMIDITY) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_ALTITUDE) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_COLOR_K) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_COLOR_LUX) {
    // todo
    clear_cmd();
  } else if(received_cmd == CMD_READ_ILLUMINANCE) {
    // todo
    clear_cmd();
  }
}
