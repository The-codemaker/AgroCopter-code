#include <Wire.h>
#include <Servo.h>

int soundPin = 8;
int motorPin1 = 9;
int motorPin2 = 10;
int servoPin1 = 11;
int servoPin2 = 12;
int speed = 500;  // Initial speed

const int MPU_ADDR = 0x68;       // MPU6050 I2C address
const int ACCEL_XOUT_H = 0x3B;   // X-axis acceleration high byte register
const int PWR_MGMT_1 = 0x6B;     // Power management register

int16_t accelX;
float gX;

float threshold = 0.3;  // Define a threshold for meaningful movement
Servo myServo;          // Servo object
int servoPos = 90;      // Start at neutral position (90 degrees)


Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();          // Initialize I2C communication
  pinMode(soundPin, INPUT);
  motorESC.attach(motorPin);  // Attach the ESC signal wire to the motor object
  motorESC.writeMicroseconds(speed);  // Send initial speed signal to ESC
  servo.attach(servoPin1);
  servo.attach(servoPin2);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(PWR_MGMT_1);  
  Wire.write(0);          // Wake up the MPU6050
  Wire.endTransmission(true);
  delay(2000);  // Delay to allow ESC to initialize
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(ACCEL_XOUT_H);  // Start reading at X-axis high byte
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 2, true);  // Request 2 bytes (high and low byte for X-axis)

  // Combine high and low byte for X-axis
  accelX = Wire.read() << 8 | Wire.read();
  gX = accelX / 16384.0;  // Convert raw data to g's (±2g sensitivity)

  if (soundPin == HIGH) {
    delay(100);
    if (soundPin == LOW){
      delay(1000);
      if (soundPin == HIGH){
        motorESC.writeMicroseconds(1000);
        delay(1000)
        motorESC.writeMicroseconds(500);
      }
    }
  }
  if (gX > threshold) {
    // Move right: Increase servo angle
    servoPos += 5;
    if (servoPos > 180) servoPos = 180;  // Keep within bounds
    myServo.write(servoPos);
  } 
  else if (gX < -threshold) {
    // Move left: Decrease servo angle
    servoPos -= 5;
    if (servoPos < 0) servoPos = 0;  // Keep within bounds
    myServo.write(servoPos);
  } 
  }

  delay(200);  // Small delay to prevent rapid movements
}
}
