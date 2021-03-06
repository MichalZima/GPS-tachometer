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
    byte confirmDate = 0;
    int newDate;
    bool dateChecked = true;
      
  public:

    String errorMessage = " ";
    bool changedToKM = true; 
    bool position0Saved = false;
    byte distanceMeasurements = 0;
    int timeZoneValue = 1; 
    float averageSpeed = 0;
    float averageHdop = 0;  
    byte arrayPosition = 0;
    float distanceLat0 = gps.location.lat();
    float distanceLong0 = gps.location.lng();
    char convertedGPSdate[11];
    
    float distance0;
    float dailyDistance = 0;
    float trackDistance = 0;
    float totalDistance = 0; 

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
    
    bool distanceCalculating(bool TRACKON){
      distance0 = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), distanceLat0, distanceLong0);
      position0Saved = false;
      distanceCalculated = true;
      float distance0InKM = distance0/1000; 
      if (!errorCheck()) {
        if (TRACKON) trackDistance += distance0InKM;
        dailyDistance += distance0InKM;
        totalDistance += distance0InKM;
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

    bool realDate(){
      if (gps.hdop.hdop() < 100) {
        if (newDate == gps.date.value() and dateChecked) {
          sprintf(convertedGPSdate, "%02d.%02d.%04d", gps.date.day(), gps.date.month(), gps.date.year());
          return dateChecked;
        }
        else if (newDate == gps.date.value() and !dateChecked) {
          confirmDate++;
          if (confirmDate >= 5) dateChecked = true;
          else dateChecked = false;
          return dateChecked;
        }
        else if (newDate != gps.date.value() and dateChecked) {
          newDate = gps.date.value();
          confirmDate = 0; 
          dateChecked = false;
          return dateChecked;
        }
      } 
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
      errorMessage = " ";
      //check signal strength and age
      if (gps.hdop.hdop() > 10) errorMessage += "HIGH HDOP, ";
      if (gps.hdop.age() > 10000) errorMessage += "OLD HDOP, ";
      if (gps.satellites.value() < 1) errorMessage += "LACK SATS, "; 
      if (gps.satellites.age() > 10000) errorMessage += "OLD SATS, ";
      //check if speed was updated and is reliable
//      else if (gps.speed.kmph() < 3) errorMessage += "NOT MOVING, ";
      //else if (gps.speed.kmph() > 2*averageSpeed) errorMessage += "BIG ACCELERATION, ";
      //check coordinates difference
      if (gps.speed.kmph() < 15 && distance0 > 500)  errorMessage += "NOT VALID COORDINATES 1, ";
      if (gps.speed.kmph() > 15 && gps.speed.kmph() < 50 && distance0 > 500)  errorMessage += "NOT VALID COORDINATES 2, ";
      if (gps.speed.kmph() > 50 && distance0 > 1000)  errorMessage += "NOT VALID COORDINATES 3, ";
      //check if any error occured
      if (errorMessage == " ") return false;
      else if (errorMessage != " ") return true;
    }

//////////////////////////////////////////////////////////////////////////////////////

    
};
