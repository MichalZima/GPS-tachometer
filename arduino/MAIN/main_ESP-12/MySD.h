#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

MyGPS myGPS;
File coordinatesFile;

class MySD {
  private:
    char latString[13];
    char longString[13];

  public:

    void savePosition() {
      char fileName[32];
      sprintf(fileName, "coordinates/%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
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

    void saveData(float avSpeed, float avHdop) {
      File dataFile;
      char fileName[20];
      sprintf(fileName, "data/%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
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
        dataFile.print("_");
        dataFile.print(gps.location.isUpdated());
        dataFile.print("\t");
        dataFile.print(myGPS.realTime());
        dataFile.print("_");
        dataFile.print(gps.time.age());
        dataFile.print("_");
        dataFile.print(gps.time.isUpdated());
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
        dataFile.print("_");
        dataFile.print(gps.speed.isUpdated());
        dataFile.print("\t");
        dataFile.print(gps.hdop.hdop());
        dataFile.print("_");
        dataFile.print(gps.hdop.age());
        dataFile.print("_");
        dataFile.print(gps.hdop.isUpdated());
        dataFile.print("\t");
        dataFile.print(gps.satellites.value());
        dataFile.print("sats_");
        dataFile.print(gps.satellites.age());
        dataFile.print("_");
        dataFile.print(gps.satellites.isUpdated());
        dataFile.print("\t");
        dataFile.print(gps.altitude.meters());
        dataFile.print("mnm_");
        dataFile.print(gps.altitude.age());
        dataFile.print("_");
        dataFile.print(gps.altitude.isUpdated());
        dataFile.print("\t");
        dataFile.print(gps.course.deg());
        dataFile.print("°_");
        dataFile.print(gps.course.age());
        dataFile.print("_");
        dataFile.print(gps.course.isUpdated());
        dataFile.print("\t");
        dataFile.print(gps.sentencesWithFix());
        dataFile.print(" fix\t");
        dataFile.print(gps.failedChecksum());
        dataFile.print(" fails\t");
        dataFile.print(gps.passedChecksum());
        dataFile.println(" passed \n");
        dataFile.close();
      }
    }
}; 
