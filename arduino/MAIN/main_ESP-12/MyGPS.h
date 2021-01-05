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
    float tempSavedLat;
    float tempSavedLong;
    float tempSavedSpeed;
    float tempSavedHdop;
    int tempSavedSats;
    char tempSavedDate[11];
    String tempSavedTime;
    int tempSavedAlt;
    int tempSavedCourse;

    int tempSavedLocationAge;
    int tempSavedSpeedAge;
    int tempSavedHdopAge;
    int tempSavedSatsAge;
    int tempSavedDateAge;
    int tempSavedTimeAge;
    int tempSavedAltAge;
    int tempSavedCourseAge;    

    int tempSavedPassedChecksum;
    int tempSavedFailedChecksum;
    int tempSavedFixSentences;

    String aftermath = " ";
  
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

           if (gps.location.isUpdated()){
         tempSavedLong = gps.location.lng();
         tempSavedLat = gps.location.lat(); 
         tempSavedLocationAge = gps.location.age();
         aftermath += "loc, ";
        }
        if (gps.speed.isUpdated()){
         tempSavedSpeed = gps.speed.kmph();
         tempSavedLocationAge = gps.speed.age();
         aftermath += "speed, ";
        }
        if (gps.hdop.isUpdated()){
         tempSavedHdop = gps.hdop.hdop(); 
         tempSavedLocationAge = gps.hdop.age();
         aftermath += "hdop, ";
        }
        if (gps.satellites.isUpdated()){
         tempSavedSats = gps.satellites.value(); 
         tempSavedLocationAge = gps.satellites.age();
         aftermath += "sats, ";
        }
        if (gps.date.isUpdated()){
         sprintf(tempSavedDate, "%02d.%02d.%04d", gps.date.day(), gps.date.month(), gps.date.year());
         tempSavedLocationAge = gps.date.age();
         aftermath += "date, ";
        }
        if (gps.time.isUpdated()){
         tempSavedTime = realTime(); 
         tempSavedLocationAge = gps.time.age();
         aftermath += "time, ";
        }
        if (gps.altitude.isUpdated()){
         tempSavedAlt = gps.altitude.meters(); 
         tempSavedLocationAge = gps.altitude.age();
         aftermath += "alt, ";
        }
        if (gps.course.isUpdated()){
         tempSavedCourse = gps.course.deg();
         tempSavedLocationAge = gps.course.age();
         aftermath += "course, ";
        }
        
      } while (millis() - start < ms);
    }

//////////////////////////////////////////////////////////////////////////////////////

    bool saveDataTemporarily() {
      while (ss.available()){ 
        gps.encode(ss.read());

       
      }
    }

//////////////////////////////////////////////////////////////////////////////////////
           
    void savePosition0(){
      distanceLat0 = gps.location.lat();
      distanceLong0 = gps.location.lng();
      position0Saved = true;
    }

//////////////////////////////////////////////////////////////////////////////////////
    
    void distanceCalculating(){
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

//////////////////////////////////////////////////////////////////////////////////////

    char* realTime(){
      sprintf(timePlusZone, "%02d:%02d:%02d", gps.time.hour()+timeZoneValue, gps.time.minute(), gps.time.second());
      return timePlusZone;
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
    
};
