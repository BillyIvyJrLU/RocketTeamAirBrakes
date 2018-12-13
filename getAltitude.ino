/*********************************************************************
*     File: SensorRead
*   Author: Steven Proctor
*  Created: 4/20/2018
* 
* This program takes an input from the sensor and if that value 
* is within the atkDist value, the robot moves forward
* This program utilizes code taken from Arduino Playground
*********************************************************************/

//reads sensor data and outputs the cm value


float getAltitude()
{
  float altitude = 0.000;
  for (int i=0; i < dataPoints; i++)
  {
    long realPressure = ms5611.readPressure();
    float relativeAltitude = ms5611.getAltitude(realPressure,referencePressure);
    relativeAltitude = relativeAltitude * 3.28084;
    //Serial.println(relativeAltitude);
    altitude = relativeAltitude+altitude;
    delay(5);
  }
  altitude = altitude/((float)(dataPoints));
  return(altitude);
}
