
#include <Servo.h>

#define LEFT_SERVO_PIN 10
#define RIGHT_SERVO_PIN 9
#define MAGNETIC_SENSOR_PIN 4

// windows functions

void close_right_window();
void close_left_window();

void open_right_window();
void open_left_window();

int is_closed();

void init_windows();
void open_windows();
void close_windows();

void lock_window();
void unlock_window();


Servo left_servo;
Servo right_servo;


void setup() {
  Serial.begin(9600);
  left_servo.attach(LEFT_SERVO_PIN);
  right_servo.attach(RIGHT_SERVO_PIN);
  pinMode(MAGNETIC_SENSOR_PIN, INPUT);
  init_windows();
}


void loop() {
 delay(1000);
 open_windows();
 delay(5000);
 Serial.println("is_closed():");
 Serial.println(is_closed());
 delay(1000);
 close_windows();
 delay(5000);
 Serial.println("is_closed():");
 Serial.println(is_closed());
}

/*
 * Sets the windows into the initial state.
 */
void init_windows() {
  close_left_window();
  close_right_window();
}


/*
 * Opens the left and right window.
 */
void open_windows() {
  open_left_window();
  open_right_window();
}


/*
 * Closes the left and right window.
 */
void close_windows() {
  close_left_window();
  close_right_window(); 
}


/*
 * Closes the right window.
 */
void close_right_window() {
  right_servo.write(158);
}


/*
 * Opens the right window.
 */
void open_right_window() {
  right_servo.write(40);
}


/*
 * Closes the left window.
 */
void close_left_window() {
  left_servo.write(25);
}


/*
 * Opens the left window.
 */
void open_left_window() {
  left_servo.write(150);
}


/*
 * Checks if the window is closed.
 */
int is_closed() {
  return digitalRead(MAGNETIC_SENSOR_PIN);
}


/*
 * Locks the window.
 */
void lock_window() {
  // todo  
}


/*
 * Unlocks the window.
 */
void unlock_window() {
  // todo
}
