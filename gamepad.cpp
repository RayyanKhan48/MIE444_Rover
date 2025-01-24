#include <Servo.h>

// Create a Servo object
Servo myServo;

// Define commands
#define FORWARD 'F'
#define LEFT 'L'
#define RIGHT 'R'
#define STOP '0'
#define BACKWARD 'B'
#define CLOSE 'C'
#define OPEN 'X'
#define TRIANGLE 'T'

// Servo positions
const int defaultPosition = 180;  // Default position of the servo (open gripper)
const int activePosition = 90;    // Position to move when condition is met (close gripper)

const int ledPin = 13;

// Motor 1 pin definitions (L298)
const int motor1_pwm = 5;
const int motor1_dir1 = 6;
const int motor1_dir2 = 7;
const int motor1_encoderA = 2;

// Motor 2 pin definitions (L298)
const int motor2_pwm = 8;
const int motor2_dir1 = 9;
const int motor2_dir2 = 10;
const int motor2_encoderA = 3;

// PID Constants (adjust as needed)
float Kp = 25.0;
float Ki = 15.0;
float Kd = 0.03;

// Target speed for motors (in counts per second)
const int targetSpeed = 50;

// PID variables
long lastErrorMotor1 = 0;
long lastErrorMotor2 = 0;
long integralMotor1 = 0;
long integralMotor2 = 0;
long lastEncoderTime = 0;

// Encoder pulse counts
volatile int encoderCountMotor1 = 0;
volatile int encoderCountMotor2 = 0;

// Movement mode
enum MovementMode { FORWARD_MODE, LEFT_MODE, RIGHT_MODE, STOP_MODE };
MovementMode currentMode = STOP_MODE;
MovementMode lastMode = STOP_MODE; // To track state changes

// Encoder ISR for Motor 1
void encoderISR_Motor1() {
  encoderCountMotor1++;
}

// Encoder ISR for Motor 2
void encoderISR_Motor2() {
  encoderCountMotor2++;
}

void setup() {
  // Initialize motor control pins
  pinMode(motor1_pwm, OUTPUT);
  pinMode(motor1_dir1, OUTPUT);
  pinMode(motor1_dir2, OUTPUT);
  pinMode(motor2_pwm, OUTPUT);
  pinMode(motor2_dir1, OUTPUT);
  pinMode(motor2_dir2, OUTPUT);

  // Initialize encoder pins
  pinMode(motor1_encoderA, INPUT);
  pinMode(motor2_encoderA, INPUT);

  // Attach interrupts to encoder pins
  attachInterrupt(digitalPinToInterrupt(motor1_encoderA), encoderISR_Motor1, RISING);
  attachInterrupt(digitalPinToInterrupt(motor2_encoderA), encoderISR_Motor2, RISING);

  // Start serial communication
  Serial.begin(9600);
  Serial.println("Ready for commands (F, L, R, B, 0, C, X).");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off LED initially
  myServo.detach(); // Detach the servo to save battery
}


void loop() {
  // Check for command input
  if (Serial.available()) {
    char command = Serial.read();
    Serial.println(command); // Print received command

    if (command == FORWARD){
      moveForward();
    } else if (command == RIGHT) {
      moveRight();
    } else if (command == LEFT) {
      moveLeft(); 
    } else if (command == BACKWARD) {
      moveBackward();
    } else if (command == OPEN) {
      myServo.attach(11); // Attach the servo before moving it
      myServo.write(defaultPosition);
      delay(500); // Allow the servo to move
      myServo.detach(); // Detach the servo to save battery
    } else if (command == CLOSE) {
      myServo.attach(11); // Attach the servo before moving it
      myServo.write(activePosition);
      delay(500); // Allow the servo to move
      myServo.detach(); // Detach the servo to save battery
    } else if (command == TRIANGLE) {
      for (int i = 0; i < 7; i++) {
        digitalWrite(ledPin, HIGH);
        delay(200); // LED on for 200ms
        digitalWrite(ledPin, LOW);
        delay(200); // LED off for 200ms
      }
      //myServo.detach();
    } else if (command == STOP) {
      stopMotors();
    } 
  }

  long currentMillis = millis();
  if (currentMillis - lastEncoderTime >= 100) {
    handlePID(currentMillis);
    lastEncoderTime = currentMillis;
  }
}

// Function to handle PID control
void handlePID(long currentMillis) {
  // Calculate speed for both motors
  long speedMotor1 = (encoderCountMotor1 * 1000) / (currentMillis - lastEncoderTime);
  long speedMotor2 = (encoderCountMotor2 * 1000) / (currentMillis - lastEncoderTime);

  // Reset encoder counts
  encoderCountMotor1 = 0;
  encoderCountMotor2 = 0;

  // PID for Motor 1
  long errorMotor1 = targetSpeed - speedMotor1;
  integralMotor1 += errorMotor1;
  long derivativeMotor1 = errorMotor1 - lastErrorMotor1;
  long outputMotor1 = Kp * errorMotor1 + Ki * integralMotor1 + Kd * derivativeMotor1;
  lastErrorMotor1 = errorMotor1;

  // PID for Motor 2
  long errorMotor2 = targetSpeed - speedMotor2;
  integralMotor2 += errorMotor2;
  long derivativeMotor2 = errorMotor2 - lastErrorMotor2;
  long outputMotor2 = Kp * errorMotor2 + Ki * integralMotor2 + Kd * derivativeMotor2;
  lastErrorMotor2 = errorMotor2;

  // Apply output to motors (clamped to PWM range)
  int motor1_pwm_value = constrain(outputMotor1, 0, 255);
  int motor2_pwm_value = constrain(outputMotor2, 0, 255);

  analogWrite(motor1_pwm, motor1_pwm_value);
  analogWrite(motor2_pwm, motor2_pwm_value);
}

// Function to move both motors forward
void moveForward() {
  digitalWrite(motor1_dir1, HIGH);
  digitalWrite(motor1_dir2, LOW);
  digitalWrite(motor2_dir1, HIGH);
  digitalWrite(motor2_dir2, LOW);
  Serial.println("Moving forward.");
}

// Function to turn left
void moveLeft() {
  digitalWrite(motor1_dir1, LOW);
  digitalWrite(motor1_dir2, HIGH);
  digitalWrite(motor2_dir1, HIGH);
  digitalWrite(motor2_dir2, LOW);
  Serial.println("Turning left.");
}

void moveBackward() {
  digitalWrite(motor1_dir1, LOW);
  digitalWrite(motor1_dir2, HIGH);
  digitalWrite(motor2_dir1, LOW);
  digitalWrite(motor2_dir2, HIGH);
  Serial.println("Moving backward.");
}

// Function to turn right
void moveRight() {
  digitalWrite(motor1_dir1, HIGH);
  digitalWrite(motor1_dir2, LOW);
  digitalWrite(motor2_dir1, LOW);
  digitalWrite(motor2_dir2, HIGH);
  Serial.println("Turning right.");
}

// Function to stop all motors
void stopMotors() {
  digitalWrite(motor1_dir1, LOW);
  digitalWrite(motor1_dir2, LOW);
  digitalWrite(motor2_dir1, LOW);
  digitalWrite(motor2_dir2, LOW);
  Serial.println("Motors stopped.");
}
