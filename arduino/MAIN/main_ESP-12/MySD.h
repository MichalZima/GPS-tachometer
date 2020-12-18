#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

File file;
MyGPS myGPS;

class MySD {
  private:
    char latString[9];
    char longString[9];

  public:

    void savePosition() {
      char fileName[32];
      sprintf(fileName, "coordinates/%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
      file = SD.open(fileName, FILE_WRITE);
      if (file) {
        dtostrf(gps.location.lat(), 9, 6, latString);
        dtostrf(gps.location.lng(), 9, 6, longString);
        file.print("[");
        file.print(longString);
        file.print(", ");
        file.print(latString);
        file.println("], ");
        file.close();
      }
    }

    void saveData(float avSpeed, float avHdop) {
      char fileName[32];
      sprintf(fileName, "data/%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
      file = SD.open(fileName, FILE_WRITE);
      if (file) {
        file.print(gps.time.value());
        file.print("\t");
        file.print(myGPS.distance);
        file.print("\t");
        file.print(myGPS.totalDistance);
        file.print("\t");
        file.print(gps.speed.kmph());
        file.print("\t");
        file.print(avSpeed);
        file.print("\t");
        file.print(gps.hdop.hdop());
        file.print("\t");
        file.print(avHdop);
        file.close();
      }
    }

};
