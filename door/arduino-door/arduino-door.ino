/*
 * @date    19.05.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Sketch for the I2C door device.
 * 
 * Commands:
 * - 0X01 : Unlocks the door.
 * - 0X02 : Locks the door.
 * - 0x03 : Opens the door.
 * - 0x04 : Closes the door.
 * - 0x05 : Retrieves the state.
 *
 */

// i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x04

// servo
#include <Servo.h>
#define SERVO_PIN 9 // uses timer 1
Servo servo;

#define UNLOCK_ANGLE 80
#define LOCK_ANGLE 110

// motor
#define MOTOR_PIN 11 // uses timer 2
#define MOTOR_IN1 3
#define MOTOR_IN2 4

// for unlocked / locked state
#define LED_PIN 13

// senses if the door is open
#define MAGNETIC_SENSOR_OPEN_PIN A0

// senses if the door is closed
#define MAGNETIC_SENSOR_CLOSE_PIN A2

// states
#define DOOR_LOCK_STATE 0
#define DOOR_UNLOCK_STATE 1

#define DOOR_CLOSE_STATE 0
#define DOOR_OPEN_STATE 1


// commands
#define CMD_UNLOCK_DOOR 0x01
#define CMD_LOCK_DOOR 0x02

#define CMD_OPEN_DOOR 0x03
#define CMD_CLOSE_DOOR 0x04


// state variables
int lock_state = DOOR_UNLOCK_STATE;
int close_state = DOOR_OPEN_STATE;

// current cmd that is processed
int received_cmd = 0x00;

void setup()
{
  // setup servo
  servo.attach(SERVO_PIN); // servo on digital pin 10
  pinMode(LED_PIN, OUTPUT);
  
  // setup motor
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  
  Serial.begin(9600);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);
  
  // init
  unlock_door();
  delay(2000);
  close_state = !is_closed();
}


/*
 * Door close / open behaviour.
 */

/*
 * Unlocks the door.
 */
void unlock_door() {
  digitalWrite(LED_PIN, HIGH);
  servo.write(UNLOCK_ANGLE);
  lock_state = DOOR_UNLOCK_STATE;
}


/*
 * Locks the door.
 */
void lock_door() {
  servo.write(LOCK_ANGLE);
  digitalWrite(LED_PIN, LOW);
  lock_state = DOOR_LOCK_STATE;
}


/*
 * Closes the door.
 */
void close_door() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_PIN, 180);
}


/*
 * Opens the door.
 */
void open_door() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  analogWrite(MOTOR_PIN, 180);
}


void stop_motor() {
  analogWrite(MOTOR_PIN, 0);
}


void clear_cmd() {
 received_cmd = 0; 
}


/*
 * I2C 
 */
void receive_data(int byteCount){ 
   while(Wire.available()) {
    received_cmd = Wire.read();
  }
}


void send_data(){
  
}


int is_closed() {
  int val = analogRead(MAGNETIC_SENSOR_CLOSE_PIN) > 1017;
  return val; 
}


int is_open() {
  int val = analogRead(MAGNETIC_SENSOR_OPEN_PIN) > 1017;
  return val; 
}



/*
 * Main loop.
 */

void loop() {
  
  if(received_cmd == CMD_UNLOCK_DOOR){
    unlock_door();
    clear_cmd();
    
  } else if(received_cmd == CMD_LOCK_DOOR) {
    lock_door();
    clear_cmd();
    
  } else if(received_cmd == CMD_OPEN_DOOR) {
    open_door();
    clear_cmd();
    
  } else if(received_cmd == CMD_CLOSE_DOOR) {
    close_door();
    clear_cmd();
    
  } else {
    
  }
  
  // ensure that the motor is stopped before we oversteer...  
  if(is_open()){
   stop_motor(); 
   close_state = DOOR_OPEN_STATE;
  }
  
  // ensure that the motor is stopped before we oversteer...
  if(is_closed()){
   stop_motor();
   close_state = DOOR_CLOSE_STATE;
  }
  
  delay(300);
}


