/*
 * @date    19.05.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 *
 * Sketch for the I2C motor door device.
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

void receive_data(int byte_count);
void send_data();
void clear_cmd();

// current cmd that is processed
int received_cmd = 0x00;


// servo
#include <Servo.h>
#define SERVO_PIN 9 // uses timer 1
Servo servo;

#define UNLOCK_ANGLE 92
#define LOCK_ANGLE 120

// motor
#define MOTOR_PIN 11 // uses timer 2
#define MOTOR_IN1 3
#define MOTOR_IN2 4

// for unlocked / locked state
#define LED_PIN 13

// senses if the door is open
#define MAGNETIC_SENSOR_OPEN_PIN 1

// senses if the door is closed
#define MAGNETIC_SENSOR_CLOSE_PIN 0

// states
#define LOCK_STATE 1
#define UNLOCK_STATE 0

#define CLOSE_STATE 1
#define OPEN_STATE 0
#define TRANSITION_STATE 2


// commands
#define CMD_UNLOCK 0x01
#define CMD_LOCK 0x02

#define CMD_OPEN 0x03
#define CMD_CLOSE 0x04

#define CMD_READ_LOCK_STATE 0x09
#define CMD_READ_CLOSE_STATE 0x0A

// door functions
int is_in_closed_position();
int is_in_open_position();
void unlock_door();
void lock_door();
void close_door();
void open_door();
void stop_motor();

void send_lock_state_byte();
void send_close_state_byte();


// state variables
int lock_state = UNLOCK_STATE;


/*
 * Setups this device.
 */
void setup() {
  // setup servo
  servo.attach(SERVO_PIN); // servo on digital pin 10
  pinMode(LED_PIN, OUTPUT);

  // setup motor
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.begin(9600);

  // initialize i2c device as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receive_data);
  Wire.onRequest(send_data);

  // init
  unlock_door();
  //delay(2000);
}


/*
 * Returns 1 if the door is located at the close position.
 */
int is_in_closed_position() {
  return analogRead(MAGNETIC_SENSOR_CLOSE_PIN) < 700;
}


/*
 * Returns 1 if the door is located at the open position.
 */
int is_in_open_position() {
  return analogRead(MAGNETIC_SENSOR_OPEN_PIN) < 700;
}


/*
 * Unlocks the door.
 */
void unlock_door() {
  digitalWrite(LED_PIN, HIGH);
  servo.write(UNLOCK_ANGLE);
  lock_state = UNLOCK_STATE;
}


/*
 * Locks the door.
 */
void lock_door() {
  servo.write(LOCK_ANGLE);
  digitalWrite(LED_PIN, LOW);
  lock_state = LOCK_STATE;
}


/*
 * Closes the door.
 */
void close_door() {
  if(lock_state == UNLOCK_STATE) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    analogWrite(MOTOR_PIN, 255);
  }
}


/*
 * Opens the door.
 */
void open_door() {
  if(lock_state == UNLOCK_STATE) {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    analogWrite(MOTOR_PIN, 255);
  }
}


/*
 * Stops the motor of this door.
 */
void stop_motor() {
  analogWrite(MOTOR_PIN, 0);
}


/*
 * Sets the current processed cmd to zero.
 */
void clear_cmd() {
 received_cmd = 0;
}


/*
 * i2c receive callback.
 */
void receive_data(int byte_count) {
  while(Wire.available()) {
    received_cmd = Wire.read();
  }
}


/*
 * i2c send data callback.
 */
void send_data(){
  if(received_cmd == CMD_READ_LOCK_STATE) {
    send_lock_state_byte();
    clear_cmd();

  } else if(CMD_READ_CLOSE_STATE) {
    send_close_state_byte();
    clear_cmd();
  }
}


/*
 * Send lock state as a byte over i2c.
 */
void send_lock_state_byte() {
  char data[] = { lock_state };
  Wire.write(data, 1);
}


/*
 * Send close state as a byte over i2c.
 */
void send_close_state_byte() {
  char val = TRANSITION_STATE;

  if(is_in_open_position()) {
    val = OPEN_STATE;
  }

  if(is_in_closed_position()) {
    val = CLOSE_STATE;
  }

  char data[] = { val };
  Wire.write(data, 1);
}


int ignore_door_sensor = 0;
unsigned long start_time = 0;
unsigned long closing_time = 0;


/*
 * Main loop.
 */
void loop() {
  if(millis() - start_time > 1000) {
    start_time = 0;
    ignore_door_sensor = 0;
  }

  if(received_cmd == CMD_UNLOCK) {
    unlock_door();
    clear_cmd();

  } else if(received_cmd == CMD_LOCK) {
    lock_door();
    clear_cmd();

  } else if(received_cmd == CMD_OPEN) {
    ignore_door_sensor = 1;
    start_time = millis();
    open_door();
    clear_cmd();

  } else if(received_cmd == CMD_CLOSE) {
    ignore_door_sensor = 1;
    start_time = millis();
    close_door();
    clear_cmd();

  } else {

  }

  // ensure that the motor is stopped before we oversteer...
  if(is_in_open_position() && ignore_door_sensor == 0) {
   delay(10);
    stop_motor();
  }

  // ensure that the motor is stopped before we oversteer...
  if(is_in_closed_position() && ignore_door_sensor == 0) {
   delay(60);
   stop_motor();
   closing_time = millis();
  }


//  Serial.println("open pin");
//  Serial.println(analogRead(MAGNETIC_SENSOR_OPEN_PIN));
//  Serial.println("closed pin");
//  Serial.println(analogRead(MAGNETIC_SENSOR_CLOSE_PIN));
//  delay(1000);
}
