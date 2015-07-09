/*
 * @date    6.07.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Sketch for the I2C window device.
 * 
 * Commands:
 * - 0X01 : Unlocks the window.
 * - 0X02 : Locks the window.
 * - 0x03 : Opens the window.
 * - 0x04 : Closes the window.
 * - 0x09 : Retrieves the locked / unlocked state.
 * - 0x0A : Retrieves the open / closed state.
 *
 */
 
 
// i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x04

void send_data();
void received_data(int byte_count);
void clear_cmd();
void send_byte(char b);

// current cmd that is processed
int received_cmd = 0x00;

 
// servo 
#include <Servo.h>
#define LEFT_SERVO_PIN 10
#define RIGHT_SERVO_PIN 9
#define LOCK_SERVO_PIN 8

Servo left_servo;
Servo right_servo;
Servo lock_servo;


// magnetic sensor
#define MAGNETIC_SENSOR_PIN 4
#define CLOSED 1
#define OPEN 0


// window angles
#define LEFT_WINDOW_OPEN_ANGLE 150
#define LEFT_WINDOW_CLOSE_ANGLE 25

#define RIGHT_WINDOW_OPEN_ANGLE 40
#define RIGHT_WINDOW_CLOSE_ANGLE 158

#define LOCK_ANGLE 90
#define UNLOCK_ANGLE 135


// window functions

void close_right_window();
void close_left_window();

void open_right_window();
void open_left_window();

int is_window_closed();
int is_window_opened();
int is_window_locked();
int is_window_unlocked();

void init_window();
void open_window();
void close_window();

void lock_window();
void unlock_window();

// commands
#define CMD_UNLOCK 0x01
#define CMD_LOCK 0x02

#define CMD_OPEN 0x03
#define CMD_CLOSE 0x04

#define CMD_READ_LOCK_STATE 0x09
#define CMD_READ_CLOSE_STATE 0x0A


// state variables
#define UNLOCKED_STATE 0
#define LOCKED_STATE 1

int locked_state = UNLOCKED_STATE;


/*
 * Setups this device.
 */
void setup() {
  Serial.begin(9600);
  
  // setup servos
  left_servo.attach(LEFT_SERVO_PIN);
  right_servo.attach(RIGHT_SERVO_PIN);
  lock_servo.attach(LOCK_SERVO_PIN);
  
  // setup magnetic sensor pin
  pinMode(MAGNETIC_SENSOR_PIN, INPUT);
  
  // initialize i2c device as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
  
  init_window();
}


/*
 * Main loop.
 */
void loop() {
 if(received_cmd == CMD_OPEN) {
   open_window();
   clear_cmd();
 } else if(received_cmd == CMD_CLOSE) {
   close_window();
   clear_cmd();
 } else if(received_cmd == CMD_LOCK) {
   lock_window();
   clear_cmd();
 } else if(received_cmd == CMD_UNLOCK) {
   unlock_window();
   clear_cmd();
 } 
}


/*
 * i2c send data callback.
 */
void send_data() {
  if(received_cmd == CMD_READ_CLOSE_STATE) {
    char closed_state = is_window_closed();
    send_byte(closed_state);
    clear_cmd();
  } else if(received_cmd == CMD_READ_LOCK_STATE) {
    char locked_state = is_window_locked();
    send_byte(locked_state);
    clear_cmd();
  }
}


/*
 * i2c receive data callback.
 */
void receive_data(int byte_count) {
  while(Wire.available()) {
    received_cmd = Wire.read();
  }
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0;
}
 

/*
 * Sets the windows into the initial state.
 */
void init_window() {
  close_left_window();
  close_right_window();
  unlock_window();
}


/*
 * Opens the left and right window.
 */
void open_window() {
  open_left_window();
  open_right_window();
}


/*
 * Closes the left and right window.
 */
void close_window() {
  close_left_window();
  close_right_window(); 
}


/*
 * Closes the right window.
 */
void close_right_window() {
  right_servo.write(RIGHT_WINDOW_CLOSE_ANGLE);
}


/*
 * Opens the right window.
 */
void open_right_window() {
  right_servo.write(RIGHT_WINDOW_OPEN_ANGLE);
}


/*
 * Closes the left window.
 */
void close_left_window() {
  left_servo.write(LEFT_WINDOW_CLOSE_ANGLE);
}


/*
 * Opens the left window.
 */
void open_left_window() {
  left_servo.write(LEFT_WINDOW_OPEN_ANGLE);
}


/*
 * Checks if the window is closed.
 */
int is_window_closed() {
  return digitalRead(MAGNETIC_SENSOR_PIN) == CLOSED;
}


/*
 * Checks if the window is opened.
 */
int is_window_open() {
  return digitalRead(MAGNETIC_SENSOR_PIN) == OPEN;
}


/*
 * Checks if the window is locked.
 */
int is_window_locked() {
  return locked_state == LOCKED_STATE;
}


/*
 * Checks if the window is unlocked.
 */
int is_window_unlocked() {
  return locked_state == UNLOCKED_STATE;
}


/*
 * Locks the window.
 */
void lock_window() {
  lock_servo.write(LOCK_ANGLE);
  locked_state = LOCKED_STATE; 
}


/*
 * Unlocks the window.
 */
void unlock_window() {
  lock_servo.write(UNLOCK_ANGLE);
  locked_state = UNLOCKED_STATE;
}


/*
 * Sends a byte over the i2c bus.
 */
void send_byte(char b) {
  char data[] = { b };
  Wire.write(data, 1);
}
