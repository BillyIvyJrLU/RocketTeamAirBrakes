/*
  MS5611 Barometric Pressure & Temperature Sensor. Simple Example
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/czujnik-cisnienia-i-temperatury-ms5611.html
  GIT: https://github.com/jarzebski/Arduino-MS5611
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include "MS5611.h"
#define   dataPoints 20
#include <Servo.h> 
 
Servo servo;
MS5611 ms5611;

int targetAlt = 30;    //in feet
int dataAlt = 25;
int servoPin = 11;
double referencePressure = 0;
float seaAlt = 0;
//int angle = 0;        //angle servo moves to at target atltitude
//int intAngle = 0;   //initial angle for the servo
float currentAlt = 0;
bool deployed = false;
bool bIntServo = false;
bool endInput = false;
float altitudeData[100] = {};
int timeData[100] = {};
double pressureData[100] = {};
int i;

void setup() 
{
  Serial.begin(9600);
  servo.attach(servoPin);

  // Initialize MS5611 sensor
  //Serial.println("Initialize MS5611 Sensor");
  while(!ms5611.begin())
  {
    //Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }
  
  servo.write(0);

  referencePressure = 0;
  currentAlt = getAltitude();
  delay(500);
  for (int i=0; i < dataPoints; i++)
  {
    long pressureData = ms5611.readPressure();
    referencePressure = pressureData+referencePressure;
    delay(10);
  }
  referencePressure = referencePressure/((float)(dataPoints));
  //Serial.println(referencePressure);  
}

void checkSettings()
{
  //Serial.print("Oversampling: ");
  //Serial.println(ms5611.getOversampling());
}

void loop()
{
  
  //Serial.println("--");
  //Serial.print("RelativeAltitude = ");
  currentAlt = getAltitude();
  //Serial.print(currentAlt);   
  //Serial.println(" ft");
  
  if((currentAlt >= targetAlt) and (deployed == false)){
    deployed = true;
    servo.write(270);
  }  

  if((currentAlt >= dataAlt) and (endInput == false)){
    altitudeData[i] = currentAlt;
    timeData[i] = millis();
    pressureData[i] = ms5611.readPressure();
    i ++;
    delay(100);
  }
  if((Serial.available()) and (endInput == false)){
      char myChar = Serial.read();
      //if the new input is CR/LF toggle flag 
      if((myChar == 10) or (myChar == 13)){
          Serial.println("\r\n");
          for(int j = 0; j < i; j ++){
            Serial.print("Altitude: ");
            Serial.print(altitudeData[j]);
            
            Serial.print(" at time: ");
            Serial.print(timeData[j]);
            Serial.print(" with pressure: ");
            Serial.println(pressureData[j]);
          }
          endInput = true;
      }
  }
}
