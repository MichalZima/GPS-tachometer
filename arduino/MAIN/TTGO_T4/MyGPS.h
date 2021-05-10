#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial ss (21, 1);


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
    int timeZoneValue = 2; 
    byte arrayPosition = 0;
    float distanceLat0 = gps.location.lat();
    float distanceLong0 = gps.location.lng();
    char convertedGPSdate[11];
    char GPSdate[11];
    
    float distance0;
    float dailyDistance = 0;
    float trackDistance = 0;
    float totalDistance = 0; 
    float pace = 0.00;
    float speedsTogether;
    float avgSpeed;
    uint16_t trackSaves = 0;;

    String dailyTotalTime = "***";
    String dailyRideTime = "***";
    float dailyAvgSpeed = 0;
    float dailyAvgPace = 0;
    int dailyAltitudeDifference = 0;

    String trackTotalTime = "***";
    String trackRideTime = "***";
    String trackRestTime = "***";
    float trackAvgSpeed = 0;
    float trackAvgPace = 0;
    int trackAltitudeDifference = 0;
    

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    void Setup(){
      ss.begin(9600);
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void smartDelay(unsigned long ms) {
      unsigned long start = millis();
      do {
        while (ss.available()) 
          gps.encode(ss.read());
      } while (millis() - start < ms);
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
           
    void savePosition0(){
      distanceLat0 = gps.location.lat();
      distanceLong0 = gps.location.lng();
      position0Saved = true;
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
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

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    char* realTime(){
      sprintf(timePlusZone, "%02d:%02d:%02d", gps.time.hour()+timeZoneValue, gps.time.minute(), gps.time.second());
      return timePlusZone;
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    bool realDate(){
      if (gps.hdop.hdop() < 50) {
        if (newDate == gps.date.value() and dateChecked and gps.date.isValid()) {
          sprintf(convertedGPSdate, "%02d.%02d.%04d", gps.date.day(), gps.date.month(), gps.date.year());
          sprintf(GPSdate, "%04d.%02d.%02d", gps.date.year(), gps.date.month(), gps.date.day() );
          return dateChecked;
        }
        else if (newDate == gps.date.value() and !dateChecked and gps.date.isValid()) {
          confirmDate++;
          if (confirmDate >= 5) dateChecked = true;
          else dateChecked = false;
          return dateChecked;
        }
        else if (newDate != gps.date.value() and dateChecked and gps.date.isValid()) {
          newDate = gps.date.value();
          confirmDate = 0; 
          dateChecked = false;
          return dateChecked;
        }
      } 
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void calculateAverage() {
//      float speedsTogether = 0;
//      float hdopsTogether = 0;
//      for (int i = 0; i < 11; i++) {
//        if (speeds[i] != 0 && hdops[i] != 0) {
//          speedsTogether = speedsTogether + speeds[i];
//          hdopsTogether = hdopsTogether + hdops[i];
//        }
//        else i = 15;
//      }
//      averageSpeed = speedsTogether / distanceMeasurements;
//      averageHdop = hdopsTogether / distanceMeasurements;
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    bool errorCheck() {
      errorMessage = "*";
      //check signal strength and age
      if (gps.hdop.hdop() > 10) errorMessage += "HIGH HDOP, ";
      if (gps.hdop.age() > 10000) errorMessage += "OLD HDOP, ";
      if (gps.satellites.value() < 1) errorMessage += "LACK SATS, "; 
      if (gps.satellites.age() > 10000) errorMessage += "OLD SATS, ";
      if (gps.location.lat() == distanceLat0 and gps.location.lng() == distanceLong0) errorMessage += "LOCATION NOT UPDATED, ";
      if (!gps.location.isValid()) errorMessage += "NOT VALID COORDINATES, ";
      //check if speed was updated and is reliable
//      else if (gps.speed.kmph() < 3) errorMessage += "NOT MOVING, ";
      //else if (gps.speed.kmph() > 2*averageSpeed) errorMessage += "BIG ACCELERATION, ";
      //check coordinates difference
      if (gps.speed.kmph() < 15 && distance0 > 500)  errorMessage += "NOT VALID COORDINATES 1, ";
      if (gps.speed.kmph() > 15 && gps.speed.kmph() < 50 && distance0 > 500)  errorMessage += "NOT VALID COORDINATES 2, ";
      if (gps.speed.kmph() > 50 && distance0 > 1000)  errorMessage += "NOT VALID COORDINATES 3, ";
      //check if any error occured
      if (errorMessage == "*") return false;
      else if (errorMessage != "*") return true;
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    
    
};
