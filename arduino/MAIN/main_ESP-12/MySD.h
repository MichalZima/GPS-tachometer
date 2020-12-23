#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

MyGPS myGPS;
File coordinatesFile;

class MySD {
  private:
    char latString[11];
    char longString[11];

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
        dataFile.print(myGPS.realTime());
        dataFile.print(" ");
        dataFile.print(gps.time.age());
        dataFile.print("\t");
        dataFile.print(myGPS.distance);
        if(myGPS.changedToKM == false) dataFile.print("m");
        if(myGPS.changedToKM == true) dataFile.print("km");
        dataFile.print(gps.location.age());
        dataFile.print("\t");
        dataFile.print(myGPS.totalDistance);
        if(myGPS.changedToKM == false) dataFile.print("m\t");
        if(myGPS.changedToKM == true) dataFile.print("km\t");
        dataFile.print(gps.speed.kmph());
        dataFile.print("km/h");
        dataFile.print(gps.speed.age());
        dataFile.print(" ");
        dataFile.print(avSpeed);
        dataFile.print("km/h\t");
        dataFile.print(gps.hdop.hdop());
        dataFile.print(" ");
        dataFile.print(gps.hdop.age());
        dataFile.print("\t");
        dataFile.print(gps.satellites.value());
        dataFile.print("sats");
        dataFile.print(gps.satellites.age());
        dataFile.print("\t");
        dataFile.print(gps.altitude.meters());
        dataFile.print("m.n.m.");
        dataFile.print(gps.altitude.age());
        dataFile.print("\t");
        dataFile.print(gps.course.deg());
        dataFile.print("° ");
        dataFile.print(gps.time.age());
        dataFile.print("\t");
        dataFile.print(gps.sentencesWithFix());
        dataFile.print(" fix\t");
        dataFile.print(gps.failedChecksum());
        dataFile.println(" fails");
        dataFile.close();
      }
    }
}; 
