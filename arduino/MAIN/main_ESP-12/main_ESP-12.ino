#include "Screens.h"

byte Loops = 0;
unsigned long Millis0 = 0;
byte passXTimes;
byte calculatingPassed = 0;
int locationUpdatedXtimes = 0;
int course0 = gps.course.deg();

Screens screens;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
  SD.begin(D8);
  Serial.begin(74880);
  myGPS.gpsSetup();
  myTFT.tftSetup();
  pushed.buttonsSetup();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() {  
  myGPS.saveDataTemporarily();
  
  if (pushed.menuState == 0) {                                  //switch to main screen
    
    if (!SD.begin(D8)) {
      myTFT.Settings(1, 80, 150);
      tft.print("sd fail");
    }

    if (pushed.menuState == 0 && pushed.nextPrevious() == true) {
      tft.fillScreen(ST7735_BLACK);
    } 

    pushed.maxState = 4;

    switch (pushed.state) {
      case 1:
        screens.First();
        break;
      case 2:
        screens.Second();
        break;
      case 3:
        screens.Third();
        break;
      case 4:
        screens.Fourth();
        break;
      default:
        myTFT.Settings(3, 10, 10);
        tft.print("default");
        break;
    }
    clearScreen();
  }


  else if (pushed.menuState == 1) {                             //switching to menu
    pushed.maxState = 8;
    menu.Cursor();
    menu.showMenu();
    clearScreen();
  }


  else if (pushed.menuState == 2) {                             //select between options in menu
    menu.select();
  }

  

  if (gps.location.isUpdated() and locationUpdatedXtimes >= 2) {                     //saving data
    myTFT.Settings(1, 10, 150);
    tft.print(locationUpdatedXtimes);
    locationUpdatedXtimes = 0;
      
    if (passCalculating() == true and myGPS.position0Saved == false) {
      myGPS.savePosition0();
      myGPS.distanceMeasurements++;
      screens.savedToSD = "count";

      myGPS.saveToArray();
      myGPS.arrayPosition++;

      if (myGPS.distanceMeasurements >= 1 or course0 + 30 < gps.course.deg() or course0 - 30 > gps.course.deg()) {
        mySD.savePosition();
        mySD.saveData();
        myGPS.distanceMeasurements = 0;
        course0 = gps.course.deg();
        screens.savedToSD = " save";
      }
    }  
  }
  else {
    myTFT.Settings(1, 10, 150);
    tft.print(locationUpdatedXtimes);
    locationUpdatedXtimes++;
    screens.savedToSD = " pass";
  }

}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool passCalculating() {
  if (gps.speed.kmph() <= 3) return false;
  else if (3 < gps.speed.kmph() && gps.speed.kmph() < 5) passXTimes = 3;
  else if (5 <= gps.speed.kmph() && gps.speed.kmph() <= 10) passXTimes = 2;
  else if (gps.speed.kmph() > 10) {
    if (myGPS.position0Saved == true) {
      myGPS.distanceCalculating();
      calculatingPassed = 0;
      return true;
    }
  }
  calculatingPassed++;
  if (calculatingPassed == passXTimes) {
    if (myGPS.position0Saved == true) {
      myGPS.distanceCalculating();
      calculatingPassed = 0;
      return true;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void clearScreen() {
  if (pushed.confirm() == true) {
    tft.fillScreen(ST7735_BLACK);
  }
}
