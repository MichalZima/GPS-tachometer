#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

File file;
MyGPS myGPS;

class MySD {
  
  public:
    
    bool save(float Lat, float Long) { 
      char fileName[32]; 
      sprintf(fileName, "%02d.%02d.%02d.txt", gps.date.day(), gps.date.month(), gps.date.year());
      file = SD.open(fileName, FILE_WRITE);
      if (file) { 
        char latString[9];
        char longString[9];
        dtostrf(Lat, 9, 6, latString);
        dtostrf(Long, 9, 6, longString);
        file.print("[");
        file.print(longString); 
        file.print(", ");     
        file.print(latString);    
        file.println("], ");   
        file.close();    
        return true;
      }
      else return false;
    }    
};
