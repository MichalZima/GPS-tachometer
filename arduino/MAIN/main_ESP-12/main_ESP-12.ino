//version with error check and loops
//v.3
//graphic stuff on track iteem in menu almost done

#include "Screens.h"

byte Loops = 0;
byte passLoops = 5;
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
    pushed.maxState = 4;
    menu.Cursor();
    menu.showMenu();
    clearScreen();
  }

  else if (pushed.menuState == 2) {                             //select between options in menu
    menu.select();
  }

  

  if (passCalculating()) {                                      //saving data
    if (myGPS.position0Saved == false) {
      myGPS.savePosition0();
      myGPS.distanceMeasurements++;
      screens.savedToSD = "count";

//      myGPS.saveToArray();
//      myGPS.arrayPosition++;

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
    screens.savedToSD = " pass";
  }
myGPS.smartDelay(200);
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool passCalculating() {
  myTFT.Settings(1, 10, 150);
  tft.print(Loops);
  if (gps.speed.kmph() <= 3) {
    Loops = 0;
    return false;
  }
  else if (3 < gps.speed.kmph() && gps.speed.kmph() < 10) passLoops = 10;
  else if (gps.speed.kmph() >= 10) passLoops = 5;
  Loops++;
  if (Loops >= passLoops) {
    if (myGPS.position0Saved == true) {
      if (myGPS.distanceCalculating()) {
        Loops = 0;
        return true;
      }
      else {
        mySD.saveErrorMessage();
        Loops = 0;
        return false;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void clearScreen() {
  if (pushed.confirm() == true) {
    tft.fillScreen(ST7735_BLACK);
  }
}
