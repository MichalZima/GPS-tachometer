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
      if (myGPS.realDate()) {
        myTFT.Settings(1, 10, 10);
        tft.print(myGPS.convertedGPSdate); 
      }
      printValuesForObservation();
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
      myTFT.Settings(1, 10, 50);
      myTFT.Print(gps.hdop.hdop(), 5, 1);
      tft.print("hdop ");
      myTFT.Print(gps.hdop.age(), 7, 0);
      if (gps.hdop.hdop() < 101){
        myTFT.Settings(1, 10, 60);
        myTFT.Print(myGPS.distance0, 6, 2);
        tft.print("m ");
        myTFT.Settings(1, 10, 70);
        myTFT.Print(myGPS.trackDistance, 6, 2);
        tft.print("km");
        myTFT.Settings(1, 10, 80);
        myTFT.Print(myGPS.dailyDistance, 6, 2);
        tft.print("km");
        myTFT.Settings(1, 10, 90);
        myTFT.Print(myGPS.totalDistance, 6, 2);
        tft.print("km");
        myTFT.Settings(1, 10, 100);
        myTFT.Print(gps.altitude.meters(), 3, 0);
        tft.print("mnm ");
        myTFT.Settings(1, 10, 110);
        myTFT.Print(gps.course.deg(), 3, 0);
        tft.print("deg ");
        myTFT.Print(gps.course.age(), 8, 0);
      }
//      myTFT.Settings(1, 10, 100);
//      myTFT.Print(gps.sentencesWithFix(), 5, 0);
//      tft.print(" fixed");
//      myTFT.Settings(1, 10, 110);
//      myTFT.Print(gps.failedChecksum(), 5, 0);
//      tft.print(" failed");
//      myTFT.Settings(1, 10, 120);
//      myTFT.Print(gps.passedChecksum(), 5, 0);
//      tft.print(" passed");
//      myTFT.Settings(1, 10, 130);
//      myTFT.Print(myGPS.distanceMeasurements, 2, 0);
      myTFT.Settings(1, 10, 130);
      tft.print(savedToSD);
    }
};
