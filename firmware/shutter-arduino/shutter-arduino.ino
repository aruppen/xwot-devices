/*
 * @date    16.07.2015
 * @author  Alexander Rüedlinger <a.rueedlinger@gmail.com>
 *
 * !!!PLEASE USE FOR THE MOTOR A 5 VOLT POWER SUPPLY, 
 * OTHERWISE THE TIMINGS ARE INCORRECT!!!
 *
 * FOR EXAMPLE USE THE 5 VOLT PIN ON THE TRINKET PRO (5V/16MHZ) 
 * TO POWER THE L293D IC (MOTOR POWER SUPPLY).
 * 
 * FOR POWERING THE TRINKET PRO USE A 9 VOLT BATTERY AND CONNECT IT
 * TO THE BAT PIN. 
 *
 *
 * Sketch for the I2C shutter device.
 *
 * Commands:
 * - 0X01 : Moves the shutter upwards.
 * - 0X02 : Moves the shutter downwards.
 * - 0X03 : Stops the shutter motor.
 * - 0x09 : Retrieves the state of the shutter.
 *
 */
 
 
// i2c
#include <Wire.h>
#define SLAVE_ADDRESS 0x05


void receive_data(int byte_count);
void send_data();
void clear_cmd();

// current cmd that is processed
int received_cmd = 0x00;

// motor
#define MOTOR_PIN 11 // uses timer 2
#define MOTOR_IN1 3
#define MOTOR_IN2 4

#define MOTOR_STATE_STOP 0
#define MOTOR_STATE_UP 1
#define MOTOR_STATE_DOWN 2

#define MOTOR_SPEED_UP 150
#define MOTOR_SPEED_DOWN 100

// senses if the shutter is the top position
#define MAGNETIC_SENSOR_TOP_PIN 0

// senses if the shutter is the bottom position
#define MAGNETIC_SENSOR_BOTTOM_PIN 1


// states
#define TOP_STATE 1
#define BOTTOM_STATE 0
#define TRANSITION_STATE 2

// commands
#define CMD_UP 0x01
#define CMD_DOWN 0x02
#define CMD_STOP 0x03

#define CMD_READ_STATE 0x09

// shutter functions
int is_at_top_position();
int is_at_bottom_position();
void move_shutter_upwards();
void move_shutter_downwards();
void stop_motor();


void send_state_byte();


/*
 * Setups this device.
 */
void setup() {

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
}



/*
 * Returns 1 if the shutter is located at the top position.
 */
int is_at_top_position() {
  return analogRead(MAGNETIC_SENSOR_TOP_PIN) > 700;
}


/*
 * Returns 1 if the shutter is located at the bottom position.
 */
int is_at_bottom_position() {
  return analogRead(MAGNETIC_SENSOR_BOTTOM_PIN) > 5;
}


/*
 * Moves the shutter upwards.
 */
void move_shutter_upwards() {
  Serial.println("up");
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_PIN, MOTOR_SPEED_UP);
}


/*
 * Moves the shutter downwards.
 */
void move_shutter_downwards() {
  Serial.println("down");
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  analogWrite(MOTOR_PIN, MOTOR_SPEED_DOWN);
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
  if(received_cmd == CMD_READ_STATE) {
    send_state_byte();
    clear_cmd();

  } 
}


/*
 * Send close state as a byte over i2c.
 */
void send_state_byte() {
  char val = TRANSITION_STATE;

  if(is_at_top_position()) {
    val = TOP_STATE;
  }

  if(is_at_bottom_position()) {
    val = BOTTOM_STATE;
  }

  char data[] = { val };
  Wire.write(data, 1);
}


int ignore_sensor = 0;
unsigned long start_time = 0;
unsigned long closing_time = 0;
int run_once = 0;


/*
 * Main loop.
 */
void loop() {
  if(millis() - start_time > 1500) {
    start_time = 0;
    ignore_sensor = 0;
  }

  if(received_cmd == CMD_UP) {
    if(!is_at_top_position()) {
      ignore_sensor = 1;
      start_time = millis();
      stop_motor();
      move_shutter_upwards();
      clear_cmd();
    }

  } else if(received_cmd == CMD_DOWN) {
    if(!is_at_bottom_position()) {
      ignore_sensor = 1;
      start_time = millis();
      stop_motor();
      move_shutter_downwards();
      clear_cmd();
    }

  } else if(received_cmd == CMD_STOP) {
    stop_motor();
    clear_cmd(); 
  } else {

  }

  // ensure that the motor is stopped before we oversteer...
  if(is_at_top_position() && ignore_sensor == 0) {
    delay(100);
    stop_motor();
  }

  // ensure that the motor is stopped before we oversteer...
  if(is_at_bottom_position() && ignore_sensor == 0) {
    stop_motor();
  }
}
