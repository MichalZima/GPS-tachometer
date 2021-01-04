#include "mySD.h"
#include "Menu.h"




MySD mySD;
Menu menu;


class Screens {
  
  public:
    String savedToSD;
  
    void First() {
      myTFT.Settings(2, 10, 10);
      myTFT.Print(gps.speed.kmph(), 4, 1);
      tft.setTextSize(1);
      tft.print(" km/h ");
      myTFT.Print(gps.speed.age(), 5, 0);
      printValuesForObservation();
    }

    void Second() {
      if (myGPS.changedToKM == false) {
        myTFT.Settings(2, 10, 10);
        myTFT.Print(myGPS.totalDistance, 3, 0);
        tft.setTextSize(1);
        tft.print(" m");
      }

      else if (myGPS.changedToKM == true) {
        myTFT.Settings(2, 10, 10);
        myTFT.Print(myGPS.totalDistance, 7, 2);
        tft.setTextSize(1);
        tft.print(" km");
      }
      printValuesForObservation();
    }

    void Third() {
      myTFT.Settings(1, 10, 10);
      tft.print(myGPS.tempSavedDate);
      printTempValuesForObservation();
    }

    void Fourth() {
      myTFT.Settings(2, 10, 10);
      tft.print(myGPS.realTime());
      printValuesForObservation();
    }


  private:
    void printValuesForObservation(){
      myTFT.Settings(1, 10, 40);
      myTFT.Print(gps.satellites.value(), 2, 0);
      tft.print(" sats ");
      myTFT.Print(gps.satellites.age(), 6, 0);
      myTFT.Settings(1, 10, 50);
      myTFT.Print(gps.hdop.hdop(), 4, 1);
      tft.print("hdop ");
      myTFT.Print(gps.hdop.age(), 6, 0);
      if (gps.hdop.hdop() < 90){
        myTFT.Settings(1, 10, 60);
        myTFT.Print(myGPS.distance0, 6, 2);
        tft.print("m ");
        myTFT.Print(gps.location.age(), 6, 0);
        myTFT.Settings(1, 10, 70);
        myTFT.Print(myGPS.totalDistance, 4, 1);
        tft.print("m/km");
        myTFT.Settings(1, 10, 80);
        myTFT.Print(gps.altitude.meters(), 3, 0);
        tft.print("mnm ");
        myTFT.Print(gps.altitude.age(), 6, 0);
        myTFT.Settings(1, 10, 90);
        myTFT.Print(gps.course.deg(), 3, 0);
        tft.print("deg ");
        myTFT.Print(gps.course.age(), 6, 0);
      }
      myTFT.Settings(1, 10, 100);
      myTFT.Print(gps.sentencesWithFix(), 5, 0);
      tft.print(" fixed");
      myTFT.Settings(1, 10, 110);
      myTFT.Print(gps.failedChecksum(), 5, 0);
      tft.print(" failed");
      myTFT.Settings(1, 10, 120);
      myTFT.Print(gps.passedChecksum(), 5, 0);
      tft.print(" passed");
      myTFT.Settings(1, 10, 130);
      myTFT.Print(myGPS.distanceMeasurements, 2, 0);
      myTFT.Settings(1, 10, 130);
      tft.print(savedToSD);
    }


    void printTempValuesForObservation(){
      myTFT.Settings(1, 10, 30);
      myTFT.Print(myGPS.tempSavedSpeed, 4, 1);
      tft.print(" kmph");
      myTFT.Settings(1, 10, 40);
      myTFT.Print(myGPS.tempSavedSats, 2, 0);
      tft.print(" sats ");
      myTFT.Print(myGPS.tempSavedSatsAge, 6, 0);
      myTFT.Settings(1, 10, 50);
      myTFT.Print(myGPS.tempSavedHdop, 4, 1);
      tft.print("hdop ");
      myTFT.Print(myGPS.tempSavedHdopAge, 6, 0);
      myTFT.Settings(1, 10, 60);
      myTFT.Print(myGPS.distance0, 6, 2);
      tft.print("m ");
      myTFT.Print(myGPS.tempSavedLocationAge, 6, 0);
      myTFT.Settings(1, 10, 70);
      myTFT.Print(myGPS.totalDistance, 4, 1);
      tft.print("m/km");
      myTFT.Settings(1, 10, 80);
      myTFT.Print(myGPS.tempSavedAlt, 3, 0);
      tft.print("mnm ");
      myTFT.Print(myGPS.tempSavedAltAge, 6, 0);
      myTFT.Settings(1, 10, 90);
      myTFT.Print(myGPS.tempSavedCourse, 3, 0);
      tft.print("deg ");
      myTFT.Print(myGPS.tempSavedCourseAge, 6, 0);
      myTFT.Settings(1, 10, 100);
      myTFT.Print(myGPS.tempSavedFixSentences, 5, 0);
      tft.print(" fixed");
      myTFT.Settings(1, 10, 110);
      myTFT.Print(myGPS.tempSavedFailedChecksum, 5, 0);
      tft.print(" failed");
      myTFT.Settings(1, 10, 120);
      myTFT.Print(myGPS.tempSavedPassedChecksum, 5, 0);
      tft.print(" passed");
      myTFT.Settings(1, 10, 130);
      myTFT.Print(myGPS.distanceMeasurements, 2, 0);
      myTFT.Settings(1, 10, 130);
      tft.print(savedToSD);
    }
};
