#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss (D3, D4);


class MyGPS {
 
  private:
    bool distanceCalculated = false;
    float speeds[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float hdops[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char timePlusZone[10];
      
  public:

    String errorMessage = "";
    float distance0;
    bool changedToKM = false; 
    float totalDistance = 0;
    bool position0Saved = false;
    byte distanceMeasurements = 0;
    float distance = 0;
    int timeZoneValue = 1; 
    float averageSpeed = 0;
    float averageHdop = 0;  
    byte arrayPosition = 0;
    float distanceLat0 = gps.location.lat();
    float distanceLong0 = gps.location.lng();

//////////////////////////////////////////////////////////////////////////////////////
    
    void gpsSetup(){
      ss.begin(9600);
    }

//////////////////////////////////////////////////////////////////////////////////////

    void smartDelay(unsigned long ms) {
      unsigned long start = millis();
      do {
        while (ss.available()) 
          gps.encode(ss.read());
      } while (millis() - start < ms);
    }

//////////////////////////////////////////////////////////////////////////////////////
           
    void savePosition0(){
      distanceLat0 = gps.location.lat();
      distanceLong0 = gps.location.lng();
      position0Saved = true;
    }

//////////////////////////////////////////////////////////////////////////////////////
    
    bool distanceCalculating(){
      distance0 = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), distanceLat0, distanceLong0);
      if (errorCheck() == false) {
        position0Saved = false;
        distanceCalculated = true;
        distance += distance0; 
        
        if (changedToKM == false) totalDistance += distance0;
        
  //      else if (totalDistance >= 1000 && changedToKM == false) {
  //          totalDistance = totalDistance / 1000;
  //          changedToKM = true;
  //      }
  //      
  //      if(changedToKM == true) {
  //        distance0 = distance0 / 1000;
  //        totalDistance += distance0;
  //      } 
      return true;
      }
      else return false;
    } 

//////////////////////////////////////////////////////////////////////////////////////

    char* realTime(){
      sprintf(timePlusZone, "%02d:%02d:%02d", gps.time.hour()+timeZoneValue, gps.time.minute(), gps.time.second());
      return timePlusZone;
    }

//////////////////////////////////////////////////////////////////////////////////////

    char* realDate(){
      char convertedGPSdate[11];
      sprintf(convertedGPSdate, "%02d.%02d.%04d", gps.date.day(), gps.date.month(), gps.date.year());
      return convertedGPSdate;
    }

//////////////////////////////////////////////////////////////////////////////////////

    void calculateAverage() {
      float speedsTogether = 0;
      float hdopsTogether = 0;
      for (int i = 0; i < 11; i++) {
        if (speeds[i] != 0 && hdops[i] != 0) {
          speedsTogether = speedsTogether + speeds[i];
          hdopsTogether = hdopsTogether + hdops[i];
        }
        else i = 15;
      }
      averageSpeed = speedsTogether / distanceMeasurements;
      averageHdop = hdopsTogether / distanceMeasurements;
    }

//////////////////////////////////////////////////////////////////////////////////////
    
    void saveToArray() {
      speeds[arrayPosition] = gps.speed.kmph();
      hdops[arrayPosition] = gps.hdop.hdop();
    }

//////////////////////////////////////////////////////////////////////////////////////
    
    void resetArray(){
      for(int i = 0; i < 11; i++){
        speeds[i] = 0.0;
        hdops[i] = 0.0;
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    bool errorCheck() {
      errorMessage = "";
      //check signal strength and age
      if (gps.hdop.hdop() > 40) errorMessage += "HIGH HDOP, ";
      if (gps.hdop.age() > 5000) errorMessage += "OLD HDOP, ";
      if (gps.satellites.value() < 3) errorMessage += "LACK SATS, "; 
      if (gps.satellites.age() > 10) errorMessage += "OLD SATS, ";
      //check if speed was updated and is reliable
      //if (gps.speed.kmph() > 2*averageSpeed) errorMessage += "BIG ACCELERATION, ";
      //check coordinates difference
      if (averageSpeed < 10 && distance0 > 100)  errorMessage += "NOT VALID COORDINATES 1, ";
      if (averageSpeed > 10 && averageSpeed < 30 && distance0 > 300)  errorMessage += "NOT VALID COORDINATES 2, ";
      if (averageSpeed > 30 && distance0 > 500)  errorMessage += "NOT VALID COORDINATES 3, ";
      //check if any error occured
      if (errorMessage != "") return true;
      else return false;
    }

//////////////////////////////////////////////////////////////////////////////////////

    
};
