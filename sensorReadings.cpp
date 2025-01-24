#include <Wire.h>
#include <MechaQMC5883.h>
#include <math.h>

MechaQMC5883 qmc;

#define TRIG_FRONT 2
#define ECHO_FRONT 3

#define TRIG_LEFT1 4
#define ECHO_LEFT1 5

#define TRIG_LEFT2 6
#define ECHO_LEFT2 7

#define TRIG_RIGHT1 8
#define ECHO_RIGHT1 9

#define TRIG_RIGHT2 42
#define ECHO_RIGHT2 44

#define TRIG_REAR 24
#define ECHO_REAR 26

#define TRIG_BOTTOM 32
#define ECHO_BOTTOM 34

float getDistanceInInches(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.0133 / 2; // Distance in inches
}

String getCompassHeading() {
  int x, y, z;
  qmc.read(&x, &y, &z);

  // Calculate heading
  float heading = atan2(y, x) * 180.0 / M_PI;
  if (heading < 0) heading += 360;

  // Determine compass direction
  String direction;
  if (heading >= 337.5 || heading < 22.5) direction = "North";
  else if (heading >= 22.5 && heading < 67.5) direction = "Northeast";
  else if (heading >= 67.5 && heading < 112.5) direction = "East";
  else if (heading >= 112.5 && heading < 157.5) direction = "Southeast";
  else if (heading >= 157.5 && heading < 202.5) direction = "South";
  else if (heading >= 202.5 && heading < 247.5) direction = "Southwest";
  else if (heading >= 247.5 && heading < 292.5) direction = "West";
  else if (heading >= 292.5 && heading < 337.5) direction = "Northwest";

  return String(heading, 1) + "° - " + direction; // Return heading with 1 decimal point
}


void setup() {
  Serial.begin(9600); // For communication with Python via HM10

  Wire.begin();
  qmc.init();

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);

  pinMode(TRIG_LEFT1, OUTPUT);
  pinMode(ECHO_LEFT1, INPUT);

  pinMode(TRIG_LEFT2, OUTPUT);
  pinMode(ECHO_LEFT2, INPUT);

  pinMode(TRIG_RIGHT1, OUTPUT);
  pinMode(ECHO_RIGHT1, INPUT);

  pinMode(TRIG_RIGHT2, OUTPUT);
  pinMode(ECHO_RIGHT2, INPUT);

  pinMode(TRIG_REAR, OUTPUT);
  pinMode(ECHO_REAR, INPUT);

  pinMode(TRIG_BOTTOM, OUTPUT);
  pinMode(ECHO_BOTTOM, INPUT);

}

void loop() {
  float front = getDistanceInInches(TRIG_FRONT, ECHO_FRONT);
  float left1 = getDistanceInInches(TRIG_LEFT1, ECHO_LEFT1);
  float left2 = getDistanceInInches(TRIG_LEFT2, ECHO_LEFT2);
  float right1 = getDistanceInInches(TRIG_RIGHT1, ECHO_RIGHT1);
  float right2 = getDistanceInInches(TRIG_RIGHT2, ECHO_RIGHT2);
  float rear = getDistanceInInches(TRIG_REAR, ECHO_REAR);
  float bottom = getDistanceInInches(TRIG_BOTTOM, ECHO_BOTTOM);
  String compass = getCompassHeading();

  String data = String(front) + "," + left1 + "," + left2 + "," + right1 + "," + right2 + "," + rear + "," + bottom + "," + compass;
  Serial.println(data); // Send data to HM10
  delay(1000); // 3 seconds delay
}
