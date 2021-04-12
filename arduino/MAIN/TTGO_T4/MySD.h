
#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"

#include "TFT_eSPI.h"
TFT_eSPI tft = TFT_eSPI();

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13

SPIClass sdSPI(VSPI);

MyGPS myGPS;

class MySD {
  
  private:
    char latString[13];
    char longString[13];
    String fileName;

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  public:
  
    bool Setup() {
      sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS, sdSPI)) return false;
        else return true;
    }


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    

    void savePosition(fs::FS & fs) {
      File coordinatesFile;
      
      if (myGPS.realDate()) {
        fileName = "/tracks/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        coordinatesFile = fs.open(fileName);
        
        if (!coordinatesFile) coordinatesFile = fs.open(fileName, FILE_WRITE);
        
        if (coordinatesFile) {
          coordinatesFile = fs.open(fileName, FILE_APPEND);
          char DATA[40];
          dtostrf(gps.location.lat(), 11, 9, latString);
          dtostrf(gps.location.lng(), 11, 9, longString);
          sprintf(DATA, "[%s, %s], ", longString, latString);
          coordinatesFile.println(DATA);
          coordinatesFile.close();
        }
      }
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveTrackData(fs::FS & fs) {
      File dataFile;
      
      if (myGPS.realDate()) {
        fileName = "/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        dataFile = fs.open(fileName);
        
        if (!dataFile) dataFile = fs.open(fileName, FILE_WRITE);
        
        if (dataFile) {
          dataFile = fs.open(fileName, FILE_APPEND);
          char DATA[200];
          
          dtostrf(gps.location.lat(), 12, 9, latString);
          dtostrf(gps.location.lng(), 12, 9, longString);
          
          sprintf(DATA, "\n[%s, %s]/%d\t%s/%d\t%.2fm\t%.3fkm\t%.1f/%d\t%.1f/%d\t%d/%d\t%.1f/%d\t%.1f/%d", longString, latString, gps.location.age(), myGPS.realTime(), gps.time.age(), myGPS.distance0, myGPS.totalDistance, gps.speed.kmph(), gps.speed.age(), gps.hdop.hdop(), gps.hdop.age(), gps.satellites.value(), gps.satellites.age(), gps.altitude.meters(), gps.altitude.age(), gps.course.deg(), gps.course.age());

          dataFile.println(DATA);
          dataFile.close();
        } 
      }  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveNoTrackData(fs::FS & fs) {
      File noTrackFile;
      
      if (myGPS.realDate()) {
        noTrackFile = fs.open("/DAILY-STATS.txt");
        
        if (!noTrackFile) noTrackFile = fs.open("/DAILY-STATS.txt", FILE_WRITE);
        
        if (noTrackFile) {
          noTrackFile.print(myGPS.convertedGPSdate);
          noTrackFile.print(" ");
          noTrackFile.print(myGPS.totalDistance);
          noTrackFile.print("/");
          noTrackFile.println(myGPS.dailyDistance); 
          noTrackFile.close();
        } 
      }     
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveErrorMessage(fs::FS & fs) {
      File dataFile;
      
      if (myGPS.realDate()) {
        fileName = "/";
        fileName += myGPS.convertedGPSdate;
        fileName += ".txt";
        dataFile = fs.open(fileName);
        
        if (!dataFile) fs.open(fileName, FILE_WRITE);

        if (dataFile) {
          dataFile = fs.open(fileName, FILE_APPEND);
          dataFile.print("(");
          dataFile.print(myGPS.errorMessage);
          dataFile.println(")");
          dataFile.close();
        }
      }
    }
    
}; 
