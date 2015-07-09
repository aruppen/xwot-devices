/*
 * @date    8.07.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Sketch for the I2C servo door device.
 * 
 * Commands:
 * - 0X01 : Unlocks the door.
 * - 0X02 : Locks the door.
 * - 0x03 : Opens the door.
 * - 0x04 : Closes the door.
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
#define DOOR_SERVO_PIN 10
#define LOCK_SERVO_PIN 9

#define CLOSE_ANGLE (35)
#define OPEN_ANGLE (35+105)
#define LOCK_ANGLE (90)
#define UNLOCK_ANGLE (140)

Servo door_servo;
Servo lock_servo;

// magnetic sensor
#define MAGNETIC_SENSOR_PIN 4
#define CLOSED 1
#define OPEN 0


// door functions

int is_door_closed();
int is_door_opened();
int is_door_locked();
int is_door_unlocked();

void init_door();
void open_door();
void close_door();

void lock_door();
void unlock_door();

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
  door_servo.attach(DOOR_SERVO_PIN);
  lock_servo.attach(LOCK_SERVO_PIN);
  
  // setup magnetic sensor pin
  pinMode(MAGNETIC_SENSOR_PIN, INPUT);
  
  // initialize i2c device as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
  
  init_door();
}


/*
 * Main loop.
 */
void loop() {
 if(received_cmd == CMD_OPEN) {
   open_door();
   clear_cmd();
 } else if(received_cmd == CMD_CLOSE) {
   close_door();
   clear_cmd();
 } else if(received_cmd == CMD_LOCK) {
   lock_door();
   clear_cmd();
 } else if(received_cmd == CMD_UNLOCK) {
   unlock_door();
   clear_cmd();
 } 
}


/*
 * i2c send data callback.
 */
void send_data() {
  if(received_cmd == CMD_READ_CLOSE_STATE) {
    char closed_state = is_door_closed();
    send_byte(closed_state);
    clear_cmd();
  } else if(received_cmd == CMD_READ_LOCK_STATE) {
    char locked_state = is_door_locked();
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
 * Sets the door into the initial state.
 */
void init_door() {
  unlock_door();
  close_door();
}


/*
 * Closes the door.
 */
void close_door() {
  door_servo.write(CLOSE_ANGLE);
}


/*
 * Opens the door.
 */
void open_door() {
  door_servo.write(OPEN_ANGLE);
}


/*
 * Checks if the door is closed.
 */
int is_door_closed() {
  return digitalRead(MAGNETIC_SENSOR_PIN) == CLOSED;
}


/*
 * Checks if the door is opened.
 */
int is_door_open() {
  return digitalRead(MAGNETIC_SENSOR_PIN) == OPEN;
}


/*
 * Checks if the door is locked.
 */
int is_door_locked() {
  return locked_state == LOCKED_STATE;
}


/*
 * Checks if the door is unlocked.
 */
int is_door_unlocked() {
  return locked_state == UNLOCKED_STATE;
}


/*
 * Locks the window.
 */
void lock_door() {
  lock_servo.write(LOCK_ANGLE);
  locked_state = LOCKED_STATE;
}


/*
 * Unlocks the window.
 */
void unlock_door() {
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
