
#include <SPI.h>
#include <SD.h>
#include "MyGPS.h"


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
    String trackPath;
    String values[8] = {"\"lat\":", "\"lon\":", "\"time\":", "\"speed\":", "\"pace\":", "\"alt\":"};
    String values2[8] = {"\"track_total_time\":", "\"track_ride_time\":", "\"track_rest_time\":", "\"track_distance\":", "\"track_avg_speed\":", "\"track_avg_pace\":", "\"track_altitude_difference\":"};
    String values3[8] = {"\"date\":", "\"daily_ride_time\":", "\"daily_distance\":", "\"daily_avg_speed\":", "\"daily_avg_pace\":", "\"daily_alt_diff\":"};
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  public:
    int trackNumber = 0;
    bool fileMade = false;

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  
    bool Setup() {
      sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS, sdSPI)) return false;
        else return true;
    }


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void makeFile(fs::FS & fs) {
      File file;
      trackNumber = 0;
      
      if (myGPS.realDate()) {
        while (!fileMade) {
          fileName = "/";
          fileName += myGPS.convertedGPSdate;
          fileName += "_";
          fileName += String(trackNumber);
          fileName += ".txt";
          file = fs.open(fileName);
          
          if (!file) {
            JSONfileName = "/";
            JSONfileName += myGPS.GPSdate;
            JSONfileName += "_";
            JSONfileName += String(trackNumber);
            JSONfileName += ".txt";
            
            trackPath = "/tracks";
            trackPath += fileName;
            
            file = fs.open(fileName, FILE_WRITE);
            file.close();
            file = fs.open(trackPath, FILE_WRITE);
            file.close();
            file = fs.open(JSONfileName, FILE_WRITE);
            file.println("{\n\"points\":[");
            file.close();
            fileMade = true;
          }

          if (file) {
            file.close();
            trackNumber++;
          }
        }
      }
      else fileMade = false;
    }


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void finishTrack(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(JSONfileName);
         
        if (dataFile) {
          dataFile = fs.open(JSONfileName, FILE_APPEND);
//          char DATA[200];
          
//          sprintf(DATA, "{%s%s, %s%s, %s%s, %s%.1f, %s%.2f, %s%d}, ", values2[0], myGPS.trackTotalTime, values2[1], myGPS.trackRideTime, values2[2], myGPS.trackRestTime, values2[3], myGPS.trackDistance, values2[4], myGPS.trackAvgSpeed, values2[5], myGPS.trackAvgPace, values2[6], myGPS.trackAltitudeDifference);

          dataFile.print(values2[0]);
          dataFile.print(myGPS.trackTotalTime);
          dataFile.print(values2[1]);
          dataFile.print(myGPS.trackRideTime);
          dataFile.print(values2[2]);
          dataFile.print(myGPS.trackRestTime);
          dataFile.print(values2[3]);
          dataFile.print(myGPS.trackDistance);
          dataFile.print(values2[4]);
          dataFile.print(myGPS.trackAvgSpeed);
          dataFile.print(values2[5]);
          dataFile.print(myGPS.trackAvgPace);
          dataFile.print(values2[6]);
          dataFile.println(myGPS.trackAltitudeDifference);
//          dataFile.println(DATA);
          dataFile.print("\n]\n}");
          dataFile.close();
          trackNumber++;
        } 
      }
      else makeFile(SD);  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void savePosition(fs::FS & fs) {
      File coordinatesFile;

      if (fileMade) {
        coordinatesFile = fs.open(trackPath);
        
        if (coordinatesFile) {
          coordinatesFile = fs.open(trackPath, FILE_APPEND);
//          char DATA[40];
          
          dtostrf(gps.location.lat(), 11, 9, latString);
          dtostrf(gps.location.lng(), 11, 9, longString);
          
//          sprintf(DATA, "[%s, %s], ", longString, latString);

          coordinatesFile.print("[");
          coordinatesFile.print(longString);
          coordinatesFile.print(", ");
          coordinatesFile.print(latString);
          coordinatesFile.print("]\n");
          coordinatesFile.close();
        }
      }
      else makeFile(SD);
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveTrackData(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(fileName);
        
        if (dataFile) {
          dataFile = fs.open(fileName, FILE_APPEND);
          char DATA[200];
          
          dtostrf(gps.location.lat(), 11, 8, latString);
          dtostrf(gps.location.lng(), 11, 8, longString);
          
//          sprintf(DATA, "\n[%s, %s]/%d\t%s/%d\t%.2fm\t%.3fkm\t%.1f/%d\t%.1f/%d\t%d/%d\t%.1f/%d\t%.1f/%d", longString, latString, gps.location.age(), myGPS.realTime(), gps.time.age(), myGPS.distance0, myGPS.totalDistance, gps.speed.kmph(), gps.speed.age(), gps.hdop.hdop(), gps.hdop.age(), gps.satellites.value(), gps.satellites.age(), gps.altitude.meters(), gps.altitude.age(), gps.course.deg(), gps.course.age());

          dataFile.print("[");
          dataFile.print(longString);
          dataFile.print(", ");
          dataFile.print(latString);
          dataFile.print("]_");
          dataFile.print(gps.location.age());
          dataFile.print("/");
          dataFile.print(gps.location.isUpdated ());
          dataFile.print("   ");
          dataFile.print(myGPS.realTime());
          dataFile.print("_");
          dataFile.print(gps.time.age());
          dataFile.print("/");
          dataFile.print(gps.time.isUpdated ());
          dataFile.print("   ");
          dataFile.print(myGPS.distance0);
          dataFile.print("m ");
          dataFile.print(myGPS.totalDistance);
          dataFile.print("km   ");
          dataFile.print(gps.speed.kmph());
          dataFile.print("kmph_");
          dataFile.print(gps.speed.age());
          dataFile.print("/");
          dataFile.print(gps.speed.isUpdated ());
          dataFile.print("   ");
          dataFile.print(gps.hdop.hdop());
          dataFile.print("_");
          dataFile.print(gps.hdop.age());
          dataFile.print("/");
          dataFile.print(gps.hdop.isUpdated ());
          dataFile.print("   ");
          dataFile.print(gps.satellites.value());
          dataFile.print("sats_");
          dataFile.print(gps.satellites.age());
          dataFile.print("/");
          dataFile.print(gps.satellites.isUpdated ());
          dataFile.print("   ");
          dataFile.print(gps.altitude.meters());
          dataFile.print("mnm_");
          dataFile.print(gps.altitude.age());
          dataFile.print("/");
          dataFile.print(gps.altitude.isUpdated ());
          dataFile.print("   ");
          dataFile.print(gps.course.deg());
          dataFile.print("°_");
          dataFile.print(gps.course.age());
          dataFile.print("/");
          dataFile.print(gps.course.isUpdated ());
          dataFile.print("\n");
//          dataFile.println(DATA);
          dataFile.close();
        } 
      }
      else makeFile(SD);  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveJSONTrackData(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(JSONfileName);
        
        if (dataFile) {
          dataFile = fs.open(JSONfileName, FILE_APPEND);
//          char DATA[200];
          
          dtostrf(gps.location.lat(), 12, 9, latString);
          dtostrf(gps.location.lng(), 12, 9, longString);
          
//          sprintf(DATA, "{%s%s, %s%s, %s%s, %s%.1f, %s%.2f, %s%d, },", values[0], latString, values[1], longString, values[2], myGPS.realTime(), values[3], gps.speed.kmph(), values[4], myGPS.pace, values[5], gps.altitude.meters());

          dataFile.print("\n { ");
          dataFile.print(values[0]);
          dataFile.print(latString);
          dataFile.print(", ");
          dataFile.print(values[1]);
          dataFile.print(longString);
          dataFile.print(", ");
          dataFile.print(values[2]);
          dataFile.print(myGPS.realTime());
          dataFile.print(", ");
          dataFile.print(values[3]);
          dataFile.print(gps.speed.kmph());
          dataFile.print(", ");
          dataFile.print(values[4]);
          dataFile.print(myGPS.pace);
          dataFile.print(", ");
          dataFile.print(values[5]);
          dataFile.print(gps.altitude.meters());
          dataFile.print(" },");
          
//          dataFile.println(DATA);
          dataFile.close();
        } 
      }
      else makeFile(SD);  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveJSONDailyStats(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open("/DAILY-STATS.txt");
        
        if (!dataFile) {
          dataFile = fs.open("/DAILY-STATS.txt", FILE_WRITE);
          dataFile.println("[");
          dataFile.close();
        }
        
        if (dataFile) {
          dataFile = fs.open("/DAILY-STATS.txt", FILE_APPEND);
          char DATA[200];
          
          sprintf(DATA, "{ %s%s, \n %s%s, \n %s%.2f, \n %s%.1f, \n %s%.1f, \n %s%d},", values3[0], myGPS.GPSdate, values3[1], myGPS.dailyRideTime, values3[2], myGPS.dailyDistance, values3[3], myGPS.dailyAvgSpeed, values3[4], myGPS.dailyAvgPace, values3[5], myGPS.dailyAltitudeDifference);

          dataFile.println(DATA);
          dataFile.close();
        } 
      }
      else makeFile(SD);  
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    void saveNoTrackData(fs::FS & fs) {
      File noTrackFile;
      
      if (fileMade) {
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
      else makeFile(SD);     
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void saveErrorMessage(fs::FS & fs) {
      File dataFile;
      
      if (fileMade) {
        dataFile = fs.open(fileName);

        if (dataFile) {
          dataFile = fs.open(fileName, FILE_APPEND);
          dataFile.print("(");
          dataFile.print(myGPS.errorMessage);
          dataFile.println(")");
          dataFile.close();
        }
      }
      else makeFile(SD);
    }
    
}; 
