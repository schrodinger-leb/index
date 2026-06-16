// Motor pins 
#define IN1  8
#define IN2  9
#define ENA  5
#define IN3  10
#define IN4  11
#define ENB  6

// Front 
#define TRIG_F  A1
#define ECHO_F  A2

// Left
#define TRIG_L  A3
#define ECHO_L  A4

// Right
#define TRIG_R  A5
#define ECHO_R  4

const int BASE_SPEED      = 160;   // 0-255 forward (speed 160 sa una)
const int TURN_SPEED      = 70;    // speed during turns
const int WALL_FOLLOW_CM  = 15;    // desired distance from right wall
const int WALL_TOO_CLOSE  = 5;     // right wall steer left
const int WALL_TOO_FAR    = 20;    // right wall  steer right 
const int FRONT_STOP_CM   = 10;    // front wall  stop and turn 
const int SHARP_STOP_CM   = 12;    // left wall too close steer right

float ultrasonicCm(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000); // 25 ms timeout
  if (duration == 0) return 100.0;       
  return duration * 0.0343 / 2.0;
}

// Motor helpers
void motorLeft(int speed) {
  if (speed >= 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    speed = -speed;
  }
  analogWrite(ENA, constrain(speed, 0, 255));
}

void motorRight(int speed) {
  if (speed >= 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    speed = -speed;
  }
  analogWrite(ENB, constrain(speed, 0, 255));
}

void driveForward(int spd = BASE_SPEED) {
  motorLeft(spd);
  motorRight(spd);
}

void stopMotors() {
  motorLeft(0); motorRight(0);
}

void turnLeft(int ms) {
  motorLeft(-TURN_SPEED);
  motorRight(TURN_SPEED);
  delay(ms);
  stopMotors();
  delay(100);
}

void turnRight(int ms) {
  motorLeft(TURN_SPEED);
  motorRight(-TURN_SPEED);
  delay(ms);
  stopMotors();
  delay(100);
}

void steerLeft() {
  motorLeft(BASE_SPEED - 40);
  motorRight(BASE_SPEED);
}

void steerRight() {
  motorLeft(BASE_SPEED);
  motorRight(BASE_SPEED - 40);
}

void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_F, OUTPUT); pinMode(ECHO_F, INPUT);
  pinMode(TRIG_L, OUTPUT); pinMode(ECHO_L, INPUT);
  pinMode(TRIG_R, OUTPUT); pinMode(ECHO_R, INPUT);

  stopMotors();
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  float frontDist = ultrasonicCm(TRIG_F, ECHO_F);
  float leftDist  = ultrasonicCm(TRIG_L, ECHO_L);
  float rightDist = ultrasonicCm(TRIG_R, ECHO_R);

  // Debug
  Serial.print("F:"); Serial.print(frontDist);
  Serial.print(" R:"); Serial.print(rightDist);
  Serial.print(" L:"); Serial.println(leftDist);



  if (frontDist < FRONT_STOP_CM && leftDist < SHARP_STOP_CM) {
    stopMotors();
    delay(100);
    turnRight(500);
    return;
  }

  if (frontDist < FRONT_STOP_CM) {
    // Front blocked, turn right
    stopMotors();
    delay(100);
    turnRight(500);
    return;
  }

  if (rightDist < WALL_TOO_CLOSE) {
    steerLeft();
    return;
  }

  if (rightDist > WALL_TOO_FAR) {
    // Opening on the right: turn right into it
    stopMotors();
    delay(50);
    turnRight(450);
    driveForward();
    delay(150);
    return;
  }

  // keep right wall at WALL_FOLLOW_CM
  float error = rightDist - WALL_FOLLOW_CM;
  if (error > 2) {
    steerRight();
  } else if (error < -2) {
    steerLeft();
  } else {
    driveForward();
  }
}
