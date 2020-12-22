#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss (D3, D4);


class MyGPS {
 
  private:
    bool distanceCalculated = false;
      
  public:
    float distance0;
    bool changedToKM = false; 
    float totalDistance = 0;
    bool position0Saved = false;
    float distanceLat0;
    float distanceLong0;
    float Speed = gps.speed.kmph();
    byte distanceMeasurements = 0;
    int course0 = gps.course.deg();
    float distance = 0;
    int timeZoneValue = 1; 
    char timePlusZone[10];
    
    void gpsSetup(){
      ss.begin(9600);
    }

    void smartDelay(unsigned long ms) {
      unsigned long start = millis();
      do {
        while (ss.available()) 
          gps.encode(ss.read());
      } while (millis() - start < ms);
      Speed = gps.speed.kmph();
    }
           
    void savePosition0(){
      smartDelay(0);
      distanceLat0 = gps.location.lat();
      distanceLong0 = gps.location.lng();
      position0Saved = true;
    }
    
    void distanceCalculating(){
      smartDelay(0);
      distance0 = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), distanceLat0, distanceLong0);
      position0Saved = false;
      distanceCalculated = true;
      distance += distance0; 
      
      if (changedToKM == false) totalDistance += distance0;
      
      else if (totalDistance >= 1000 && changedToKM == false) {
          totalDistance = totalDistance / 1000;
          changedToKM = true;
      }
      
      if(changedToKM == true) {
        distance0 = distance0 / 1000.0;
        totalDistance += distance0;
      } 
    } 

    char* realTime(){
      sprintf(timePlusZone, "%02d:%02d:%02d", gps.time.hour()+timeZoneValue, gps.time.minute(), gps.time.second());
      return timePlusZone;
    }
};
