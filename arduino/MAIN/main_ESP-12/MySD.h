#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

MyGPS myGPS;

class MySD {
  
  private:
    char latString[13];
    char longString[13];
    String fileName;

//////////////////////////////////////////////////////////////////////////////////////

  public:

    void savePosition() {
      File coordinatesFile;
      if (myGPS.realDate()) {
        fileName = "trasy/mapy/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        coordinatesFile = SD.open(fileName, FILE_WRITE);
        if (coordinatesFile) {
          dtostrf(gps.location.lat(), 9, 6, latString);
          dtostrf(gps.location.lng(), 9, 6, longString);
          coordinatesFile.print("[");
          coordinatesFile.print(longString);
          coordinatesFile.print(", ");
          coordinatesFile.print(latString);
          coordinatesFile.println("], ");
          coordinatesFile.close();
        }
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    void saveTrackData() {
      File dataFile;
      if (myGPS.realDate()) {
        fileName = "trasy/data/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        dataFile = SD.open(fileName, FILE_WRITE);
        if (dataFile) {
          dtostrf(gps.location.lat(), 12, 9, latString);
          dtostrf(gps.location.lng(), 12, 9, longString);
          dataFile.print("[");
          dataFile.print(longString);
          dataFile.print(", ");
          dataFile.print(latString);
          dataFile.print("]_");
          dataFile.print(gps.location.age());
          dataFile.print("\t");
          dataFile.print(myGPS.realTime());
          dataFile.print("_");
          dataFile.print(gps.time.age());
          dataFile.print("\t");
          dataFile.print(myGPS.distance0);
          if(myGPS.changedToKM == false) dataFile.print("m ");
          else if(myGPS.changedToKM == true) dataFile.print("km ");
          dataFile.print(myGPS.totalDistance);
          if(myGPS.changedToKM == false) dataFile.print("m\t");
          else if(myGPS.changedToKM == true) dataFile.print("km\t");
          dataFile.print(gps.speed.kmph());
          dataFile.print("kmph_");
          dataFile.print(gps.speed.age());
          dataFile.print("\t");
          dataFile.print(gps.hdop.hdop());
          dataFile.print("_");
          dataFile.print(gps.hdop.age());
          dataFile.print("\t");
          dataFile.print(gps.satellites.value());
          dataFile.print("sats_");
          dataFile.print(gps.satellites.age());
          dataFile.print("\t");
          dataFile.print(gps.altitude.meters());
          dataFile.print("mnm_");
          dataFile.print(gps.altitude.age());
          dataFile.print("\t");
          dataFile.print(gps.course.deg());
          dataFile.print("°_");
          dataFile.print(gps.course.age());
          dataFile.println("\n");
          dataFile.close();
        } 
      }     
    }

//////////////////////////////////////////////////////////////////////////////////////

    void saveNoTrackData() {
      File noTrackFile;
      if (myGPS.realDate()) {
        fileName = "denne_statistiky/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        noTrackFile = SD.open(fileName, FILE_WRITE);
        if (noTrackFile) {
          noTrackFile.print(myGPS.totalDistance);
          noTrackFile.print("/");
          noTrackFile.println(myGPS.dailyDistance); 
          noTrackFile.close();
        } 
      }     
    }
    

//////////////////////////////////////////////////////////////////////////////////////

    void saveErrorMessage(bool TRACK) {
      File dataFile;
      if (myGPS.realDate()) {
        if (TRACK) fileName = "trasy/data/";
        else if (!TRACK) fileName = "denne_statistiky/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        dataFile = SD.open(fileName, FILE_WRITE);
        if (dataFile) {
          dataFile.print(myGPS.errorMessage);
          dataFile.println("\n");
          dataFile.close();
        }
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    long backup () {
      File backupFile;
      long fileSize;
      backupFile = SD.open("backup/data.txt", FILE_WRITE);
        if (backupFile) {
          backupFile.print("*");
          backupFile.print(myGPS.convertedGPSdate);
          backupFile.print(";");
          backupFile.print(myGPS.dailyDistance);
          backupFile.print(",");
          backupFile.print(myGPS.totalDistance);
          backupFile.print(".\n");
          fileSize = backupFile.size();
          backupFile.close();
        }
      return fileSize;
    }
    
}; 
