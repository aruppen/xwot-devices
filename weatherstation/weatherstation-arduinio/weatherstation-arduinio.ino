
// sensor dependencies
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_TSL2561_U.h>
#include <DHT.h>
#include <DHT_U.h>

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


// sensors
Adafruit_BMP085 bmp;
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define DHT_PIN            2
#define DHT_TYPE            DHT22
DHT_Unified dht(DHT_PIN, DHT_TYPE);


#define VERSION "Weather Station v1.0.0"

// state variables
float illuminance_value = 0.0;
float pressure_value = 0.0;
float temperature_value1 = 0.0;
float temperature_value2 = 0.0;
float humidity_value = 0.0;
float altitude_value = 0.0;

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

void print_version();
void print_splash();
void print_clock();

void setup() {
  Serial.begin(9600);
  init_screen();
  init_sensors();
}


void init_screen() {
  tft.begin(HX8357D);
  tft.setRotation(3); // 1
  tft.fillScreen(HX8357_BLACK);
  print_version();
}

void init_sensors() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
  }
  
  if(!tsl.begin()) {
    Serial.println("Could not find a valid TSL2561 sensor, check wiring!");
  }
  
  dht.begin();
}


void print_splash() {
  
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
  tft.println(" C");
}


/*
 * Prints temperature 2 on the tft screen.
 */
void print_temp2() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(DISPLAY_WIDTH/2 + 30, 70);
  tft.println("Temperature 2: ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 30, 90);
  tft.setTextSize(3);
  tft.print(temperature_value2);
  tft.println(" C");
}


/*
 * Prints average temperature on the tft screen.
 */
void print_temp_avg() {
  tft.setTextSize(2);
  tft.setTextColor(HX8357_WHITE, HX8357_BLACK);
  tft.setCursor(DISPLAY_WIDTH/2 + 30, 130);
  tft.println("Temperature avg: ");
  
  tft.setCursor(DISPLAY_WIDTH/2 + 30, 150);
  tft.setTextSize(3);
  tft.print((temperature_value1 + temperature_value2)/2.0);
  tft.println(" C");
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
  tft.println(" %");
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
  tft.println(" pa");
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
  tft.println(" lx");
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
  tft.println(" m");
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
  
}


/*
 * main loop.
 */
void loop() { 
 print_temp1();
 print_temp2();
 print_temp_avg();
 print_humidity();
 print_pressure();
 print_illuminance();
 print_altitude();
 print_clock();
 update_sensor_values();
 delay(1000);
}

