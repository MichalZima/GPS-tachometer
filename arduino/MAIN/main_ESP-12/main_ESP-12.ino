//version with error check and loops
//v.3
//graphic stuff on track iteem in menu almost done
//made blinking cursor in menu

#include "Screens.h"
#include "Startup.h"

byte Loops = 0;
byte passLoops = 5;
unsigned long Millis0 = 0;
byte passXTimes;
byte calculatingPassed = 0;
int locationUpdatedXtimes = 0;
int course0 = gps.course.deg();
int timerCount = 0;
float lastSavedDailyDistance = 0;
bool previousTrackState = false;

Screens screens;
Startup startup;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void setup() {
  SD.begin(D8);
  Serial.begin(74880);
  myGPS.gpsSetup();
  myTFT.tftSetup();
  pushed.buttonsSetup();
  if (!startup.initialCheck()) Serial.println("initial error");
  else if (startup.initialCheck()) Serial.println("initialCheck passed");
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() {
  if (menu.turnOff) {
    myGPS.dailyDistance += myGPS.trackDistance;
    mySD.saveNoTrackData();
    EEPROM.write(0, 1);
    EEPROM.commit();
  }

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
    pushed.maxState = 5;
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
      
      if (menu.trackStart) trackSaving();
      else if (!menu.trackStart) {
        if (myGPS.dailyDistance - lastSavedDailyDistance >= 500) {
          lastSavedDailyDistance = myGPS.dailyDistance;
          mySD.backup();
        }
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



//////////////////////////////////////////////////////////////////////////////////////

void trackSaving() {
  if (myGPS.distanceMeasurements >= 5 or courseDifference() > 5) {
    mySD.savePosition();
    mySD.saveTrackData();
    myGPS.distanceMeasurements = 0;
    course0 = gps.course.deg();
    screens.savedToSD = " save";
  }
}

//////////////////////////////////////////////////////////////////////////////////////

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
      if (myGPS.distanceCalculating(menu.trackStart)) {
        Loops = 0;
        return true;
      }
      else {
        if (menu.trackStart) mySD.saveErrorMessage(true);
        if (!menu.trackStart) mySD.saveErrorMessage(false);
        Loops = 0;
        return false;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////

int courseDifference() {
  int difference = course0 - gps.course.deg();
  return abs(difference);
}

//////////////////////////////////////////////////////////////////////////////////////

void clearScreen() {
  if (pushed.confirm() == true) {
    tft.fillScreen(ST7735_BLACK);
  }
}
