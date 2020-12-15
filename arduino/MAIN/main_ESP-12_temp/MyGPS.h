#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss (D3, D4);


class MyGPS {
 
  private:
    bool distanceCalculated = false;
    bool changedToKM = false;
    float distance; 
    float distance0 = 0;
      
  public:
    float totalDistance = 0;
    bool position0Saved = false;
    float Lat0;
    float Long0;
    float Speed = gps.speed.kmph();
    
    void gpsSetup(){
      ss.begin(9600);
    }
           
    void savePosition0(){
      smartDelay(0);
      Lat0 = gps.location.lat();
      Long0 = gps.location.lng();
      position0Saved = true;
    }
    
    void distanceCalculating(){
      smartDelay(0);
      distance = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), Lat0, Long0);
      position0Saved = false;
      distanceCalculated = true;
      distance0 += distance; 
      if (totalDistance < 1000) totalDistance += distance;
      
      else if (totalDistance >= 1000) {
        if (changedToKM == false) {
          totalDistance = totalDistance / 1000;
          changedToKM == true;
        }
        distance = distance / 1000.0;
        totalDistance += distance; 
      }
    } 

    void smartDelay(unsigned long ms) {
      unsigned long start = millis();
      do {
        while (ss.available()) 
          gps.encode(ss.read());
      } while (millis() - start < ms);
      Speed = gps.speed.kmph();
    }
};
