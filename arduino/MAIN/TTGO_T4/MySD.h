
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

//////////////////////////////////////////////////////////////////////////////////////

  public:
  
    bool SDsetup() {
      sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS, sdSPI)) return false;
        else return true;
    }


    void readFile(fs::FS &fs, const char * path){
      Serial.printf("Reading file: %s\n", path);
  
      File file = fs.open(path);
      if(!file){
          Serial.println("Failed to open file for reading");
          return;
      }
  
      Serial.print("Read from file: ");
      while(file.available()){
          Serial.write(file.read());
      }
      file.close();
    }
    

    void savePosition(fs::FS & fs) {
      File coordinatesFile;
      //if (myGPS.realDate()) {
        fileName = "/";
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
      //}
    }

//////////////////////////////////////////////////////////////////////////////////////

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
          
          sprintf(DATA, "\n[%s, %s]/%d\t%s/%d\t%.2fm\t%.3fkm\t%.1f/%d\t%.1f/%d\t%d/%d\t%.1f/%d\t%.1f/%d\t ", longString, latString, gps.location.age(), myGPS.realTime(), gps.time.age(), myGPS.distance0, myGPS.totalDistance, gps.speed.kmph(), gps.speed.age(), gps.hdop.hdop(), gps.hdop.age(), gps.satellites.value(), gps.satellites.age(), gps.altitude.meters(), gps.altitude.age(), gps.course.deg(), gps.course.age());

          dataFile.println(DATA);
          dataFile.close();
        } 
      }  
//      int str_len = fileName.length() + 1;
//      char path[str_len];
//      fileName.toCharArray(path, str_len);
//      readFile(SD, path);   
    }

//////////////////////////////////////////////////////////////////////////////////////

    void saveNoTrackData(fs::FS & fs) {
      File noTrackFile;
      if (myGPS.realDate()) {
        noTrackFile = fs.open("denne_statistiky.txt", FILE_WRITE);
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

//////////////////////////////////////////////////////////////////////////////////////

    void saveErrorMessage(bool TRACK, fs::FS & fs) {
      File dataFile;
      if (myGPS.realDate()) {
        if (TRACK) {
          fileName = "trasy-bn-180/data/";
          fileName += myGPS.convertedGPSdate;
          fileName += ".txt";
        }
        else if (!TRACK) fileName = "backup/data.txt";
        
        dataFile = fs.open(fileName, FILE_WRITE);
        if (dataFile) {
          dataFile.print("(");
          dataFile.print(myGPS.errorMessage);
          dataFile.println(")");
          dataFile.close();
        }
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    long backup (fs::FS & fs) {
      File backupFile;
      long fileSize;
      backupFile = fs.open("backup/data.txt", FILE_WRITE);
        if (backupFile) {
          backupFile.print("*");
          backupFile.print(myGPS.convertedGPSdate);
          backupFile.print(" ");
          backupFile.print(myGPS.totalDistance);
          backupFile.print("/");
          backupFile.print(myGPS.dailyDistance);
          backupFile.print(";\n"); 
          fileSize = backupFile.size();
          backupFile.close();
        }
      return fileSize;
    }
    
}; 
