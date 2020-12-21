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
      char fileName[32];
      sprintf(fileName, "data/%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
      dataFile = SD.open(fileName, FILE_WRITE);
      if (dataFile) {
        dataFile.print(myGPS.realTime());
        dataFile.print("\t");
        dataFile.print(myGPS.distance);
        dataFile.print("\t");
        dataFile.print(myGPS.totalDistance);
        dataFile.print("\t");
        dataFile.print(gps.speed.kmph());
        dataFile.print("\t");
        dataFile.print(avSpeed);
        dataFile.print("\t");
        dataFile.print(gps.hdop.hdop());
        dataFile.print("\t");
        dataFile.println(avHdop);
        dataFile.close();
      }
    }
}; 
