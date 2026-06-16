/* Commands: F=Forward, B=Backward, L=Left, R=Right, S=Stop
   A=Autonomous mode (triggered by bridge.py when BLE signal is weak)
   M=Manual mode (triggered by bridge.py when BLE signal is strong)
   Autonomous mode uses IR sensors to stay in ring and
   ultrasonic sensor to avoid obstacles while returning to owner */

#define ENA 5
#define IN1 8
#define IN2 9
#define ENB 6
#define IN3 10
#define IN4 11

#define SPEED      140
#define AUTO_SPEED 80

#define IR_LEFT   3
#define IR_RIGHT  4

#define TRIG_PIN A1
#define ECHO_PIN A2
 
#define OBSTACLE_DIST 20
bool autonomous = false;

void setup() {
    Serial.begin(9600);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(IR_LEFT,  INPUT);
    pinMode(IR_RIGHT, INPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    stopMotors();
    Serial.println("Ready");
}

void loop() {
    // Check for incoming command from HM-10
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        switch (cmd) {
            case 'A':
                autonomous = true;
                stopMotors();
                break;
            case 'M':
                autonomous = false;
                stopMotors();
                break;
            case 'F': if (!autonomous) forward();    break;
            case 'B': if (!autonomous) backward();   break;
            case 'L': if (!autonomous) turnLeft();   break;
            case 'R': if (!autonomous) turnRight();  break;
            case 'S': if (!autonomous) stopMotors(); break;
        }
    }

    if (autonomous) {
        navigateAutonomous();
    }
}

void navigateAutonomous() {
    int leftIR  = digitalRead(IR_LEFT);
    int rightIR = digitalRead(IR_RIGHT);
    long dist   = getDistance();

    // Priority 1 — Stay inside ring using IR
    // IR LOW = detected edge/white line
    if (leftIR == LOW && rightIR == LOW) {
        backward();
        delay(400);
        turnRight();
        delay(600);

    } else if (leftIR == LOW) {
        backward();
        delay(300);
        turnRight();
        delay(400);

    } else if (rightIR == LOW) {
        backward();
        delay(300);
        turnLeft();
        delay(400);

    // Priority 2 — Avoid obstacles using ultrasonic
    } else if (dist > 0 && dist < OBSTACLE_DIST) {
        stopMotors();
        delay(100);
        turnRight();
        delay(400);

    // Priority 3 — Move forward toward owner
    } else {
        forward();
    }
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    if (duration == 0) return 999;
    return duration * 0.034 / 2;
}

void forward() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
}

void backward() {
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    analogWrite(ENA, AUTO_SPEED); analogWrite(ENB, AUTO_SPEED);
}

void turnLeft() {
    digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENA, AUTO_SPEED); analogWrite(ENB, AUTO_SPEED);
}

void turnRight() {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
    analogWrite(ENA, AUTO_SPEED); analogWrite(ENB, AUTO_SPEED);
}

void stopMotors() {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);    analogWrite(ENB, 0);
}
