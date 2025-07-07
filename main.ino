#include <Servo.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>


#define Echo A0
#define Trig A1
#define motor 10
#define Speed 170
#define spoint 103


char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;


TinyGPSPlus gps;
SoftwareSerial gps_ss(7, 8); // RX, TX
Servo servo;


AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);


void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(motor,OUTPUT);
  servo.attach(motor);
  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
  gps_ss.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(4, OUTPUT);
}


void loop() {
  distance=ultrasonic();
  Bluetoothcontrol();
  Obstacle();
  if (distance<=60)
  {
    digitalWrite(4, LOW);
    digitalWrite(12,LOW);
  }
  else
  {
    digitalWrite(4,HIGH);
    digitalWrite(12,HIGH);
    
  }
   
}


void Bluetoothcontrol() {
  
  if (Serial.available() > 0) {
    value = Serial.read();
    Serial.println(value);
  }
  if (value == 'F') {
    forward();
  } 
  else if (value == 'B') {
    backward();
  }
  else if (value == 'L') {
    left();
  } 
  else if (value == 'R') {
    right();
  } 
  else if (value == 'S') {
    Stop();
  }
  else if(value=='V'){
    gpsLocation();
    value='S';
  }
}


void Obstacle() {
  
  if (distance <= 12) {
    Stop();
    backward();
    delay(100);
    Stop();
    L = leftsee();
    servo.write(spoint);
    delay(800);
    R = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
      value='S';
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
      value='S';
    }
  }
  light(distance);
}


void light(int distance){
  if (distance<=60)
  {  
    
  	digitalWrite(4, LOW);
    digitalWrite(12,LOW);
  }
  else
  {
    digitalWrite(4,HIGH);
    digitalWrite(12,HIGH);
    
  }
}


int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2; 
  return cm;
}


void gpsLocation()
{
  Serial.println("Attempting to read GPS data...");

  gps_ss.listen();  
  bool receivedData = false;

  while (gps_ss.available() > 0) 
  {
    char c = gps_ss.read();
    gps.encode(c);
    receivedData = true;
  }

  if (receivedData) 
  {
    Serial.println("\nGPS data received.");
  } 
  else
  {
    Serial.println("No GPS data available.");
  }

  if (gps.location.isValid()) {
    gps.encode(gps_ss.read());
    Serial.print("Latitude= ");
    Serial.println(gps.location.lat()); 
    Serial.print("Longitude= ");
    Serial.println(gps.location.lng()); 
  } 
  else
  {
    Serial.println("Location not valid.");
  } 
}


void forward() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}


void backward() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}


void right() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}


void left() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}


void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}


int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}


int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}
