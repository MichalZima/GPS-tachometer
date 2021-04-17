
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
 
  private:
    char latString[13];
    char longString[13];
    String JSONfileName;
    String fileName;
    byte fileNumber = 0;
    String values[7] = {"\"lat\":", "\"lon\":", "\"time\":", "\"speed\":", "\"pace\":", "\"alt\":"};
    String values2[7] = {"\"total_time\":", "\"ride_time\":", "\"distance\":", "\"avg_speed\":", "\"avg_pace\":", "\"altitude_difference\":"};
    String values3[7] = {"\"date\":", "\"daily_distance\":", "\"daily_ride_time\":", "\"daily_avg_speed\":", "\"daily_avg_pace\":", "\"daily_alt_diff\":"};
    bool fileMade = false;
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  public:
  
    bool Setup() {
      sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS, sdSPI)) return false;
        else return true;
    }


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void makeFile() {
      File file;
      bool newFile = false;
      
      if (myGPS.realDate()) {
        
        while (!newFile) {
          fileName = "/";
          fileName += myGPS.convertedGPSdate;
          fileName += "_";
          fileName += fileNumber;
          fileName += ".txt";
          file = fs.open(fileName);
          
          if (file) {
            file.close();
            fileNumber++;
          }
          
          if (!file) {
            JSONfileName = "/";
            JSONfileName += myGPS.convertedGPSdate;
            JSONfileName += "_";
            JSONfileName += fileNumber;
            JSONfileName += ".txt";
            
            file = fs.open(fileName, FILE_WRITE);
            file.close();
            file = fs.open(JSONfileName, FILE_WRITE);
            file.print("{\n\"points\":[");
            file.close();
            newFile = true;
            fileMade = true;
          }
        }
      }
    }


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void finishTrack(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(JSONfileName);
         
        if (dataFile) {
          dataFile = fs.open(JSONfileName, FILE_APPEND);
          char DATA[200];
          
          sprintf(DATA, "{%s%s, %s%s, %s%.1f, %s%.2f, %s%f},", values2[0], latString, values2[1], longString, values2[2], myGPS.realTime(), values2[3], gps.speed.kmph(), values2[4], myGPS.pace, values2[5], gps.altitude.meters());
          
          dataFile.print("\n]\n}");
          dataFile.close();
        } 
      }  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void savePosition(fs::FS & fs) {
      File coordinatesFile;
      
      if (fileMade) {
        coordinatesFile = fs.open(fileName);
        
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
      
      if (fileMade) {
        dataFile = fs.open(fileName);
        
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

    void saveJSONTrackData(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(JSONfileName);
        
        if (dataFile) {
          dataFile = fs.open(JSONfileName, FILE_APPEND);
          char DATA[200];
          
          dtostrf(gps.location.lat(), 12, 9, latString);
          dtostrf(gps.location.lng(), 12, 9, longString);
          
          sprintf(DATA, "{%s%s, %s%s, %s%.1f, %s%.2f, %s%f},", values[0], latString, values[1], longString, values[2], myGPS.realTime(), values[3], gps.speed.kmph(), values[4], myGPS.pace, values[5], gps.altitude.meters());

          dataFile.println(DATA);
          dataFile.close();
        } 
      }  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveJSONDailyStats(fs::FS & fs) {
      File dataFile;
      
      if (myGPS.realDate()) {
        dataFile = fs.open("/DAILY-STATS.txt");
        
        if (!dataFile) dataFile = fs.open("/DAILY-STATS.txt", FILE_WRITE);
        
        if (dataFile) {
          dataFile = fs.open("/DAILY-STATS.txt", FILE_APPEND);
          char DATA[200];
          
          sprintf(DATA, "\n %s, %s, %.1f, %.2f, %f", myGPS.realDate(), myGPS.totalDistance, myGPS.dailyDistance, myGPS.rideTime, myGPS.avgSpeed, myGPS.avgPace, myGPS.elevationGain);

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
