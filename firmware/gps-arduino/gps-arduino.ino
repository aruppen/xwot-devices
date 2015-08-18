/*
 * @date    18.08.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * This sketch simulates a simple I2C gps slave device
 * and is heavily based on the adafruit gps example program 'parsing.ino'.
 *
 * The arduino MCU is connected to a gps device (Adafruit Ultimate GPS Breakout).
 * 
 * Commands:
 * - 0X01 : Reads the gps state.
 * - 0X02 : Reads the longitude value.
 * - 0X03 : Reads the latitude value.
 * - 0X04 : Reads the altitude value.
 *
 * Dependency: https://github.com/adafruit/Adafruit-GPS-Library
 */

 
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
 
//i2c
#define SLAVE_ADDRESS 0x05

// Commands
#define CMD_READ_GPS_STATE 0x01
#define CMD_READ_LONGITUDE 0x02
#define CMD_READ_LATITUDE 0x03
#define CMD_READ_ALTITUDE 0x4


// Setup gps device
#define GPS_TX_SERIAL 4
#define GPS_RX_SERIAL 3

SoftwareSerial gps_serial(GPS_TX_SERIAL, GPS_RX_SERIAL);
Adafruit_GPS GPS(&gps_serial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences. 
#define GPSECHO  true

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy


// state variables
char gps_state = 0;
float latitude = -1;
float longitude = -1;
float altitude = 0;

int received_cmd = 0x00;

void send_byte(char b);
void send_float(float f);
void send_data();
void receive_data(int byte_count);
void clear_cmd();


void setup() {
  Serial.begin(9600);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
  
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  // the nice thing about this code is you can have a timer0 interrupt go off
  // every 1 millisecond, and read data from the GPS for you. that makes the
  // loop code a heck of a lot easier!
  useInterrupt(true);
  
  delay(1000);
  // Ask for firmware version
  gps_serial.println(PMTK_Q_RELEASE);
}


// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;  
    // writing direct to UDR0 is much much faster than Serial.print 
    // but only one character can be written at a time. 
#endif
}


void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}


#define INTERVAL 2000
unsigned long time = millis();


/*
 * Main loop.
 */
void loop() {
  // in case you are not using the interrupt above, you'll
  // need to 'hand query' the GPS, not suggested :(
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c) Serial.print(c);
  }
  
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
  
  if(millis() - time > INTERVAL) {
    time = millis();
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
    
    if (GPS.fix) {
      gps_state = 1;
      latitude = GPS.latitudeDegrees;
      longitude = GPS.longitudeDegrees;
      altitude = GPS.altitude;
      Serial.print("Location (in degrees): ");
      Serial.print(GPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(GPS.longitudeDegrees, 4);
      
      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    } else {
     gps_state = 0; 
    }
  }
  
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0; 
}


/*
 * Received data over i2c bus.
 */
void receive_data(int byte_count){ 
  while(Wire.available()) {
    received_cmd = Wire.read();
  }
}

#define CMD_READ_GPS_STATE 0x01
#define CMD_READ_LONGITUDE 0x02
#define CMD_READ_LATITUDE 0x03
#define CMD_READ_ALTITUDE 0x4
/*
 * Send data over i2c bus.
 */
void send_data(){
  if(received_cmd == CMD_READ_GPS_STATE) {
   send_byte(gps_state);
   clear_cmd();
  }
  
  if(received_cmd == CMD_READ_LONGITUDE) {
   send_float(longitude);
   clear_cmd();
  }
  
  if(received_cmd == CMD_READ_LATITUDE) {
   send_float(latitude);
   clear_cmd();
  }
  
  if(received_cmd == CMD_READ_ALTITUDE) {
   send_float(altitude);
   clear_cmd();
  }
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


/*
 * Send float value over the i2c bus.
 */
void send_byte(char b) {
  char data[1] = {b};
  Wire.write(data, 1);
}
