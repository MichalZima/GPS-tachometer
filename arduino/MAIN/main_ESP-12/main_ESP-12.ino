#include "Screens.h"

byte Loops = 0;
unsigned long Millis0 = 0;
bool timePassed = true;
float Hdop = 0;
float speeds[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float hdops[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float averageSpeed = 0;
float averageHdop = 0;
byte arrayPosition = 0;
byte passXTimes;
byte calculatingPassed = 0;

int previousPassedChecksum = 0;
int previousFails = 0;

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




void loop() {
  while (ss.available() > 0)
    gps.encode(ss.read());

  
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




  if (gps.speed.isUpdated() && previousFails == gps.failedChecksum()) {                     //saving data

      if (myGPS.position0Saved == false && passCalculating() == true) {
        myGPS.savePosition0();
        myGPS.distanceMeasurements++;
        screens.savedToSD = "count";

        saveToArray();
        arrayPosition++;

        if (myGPS.distanceMeasurements >= 1 || myGPS.course0 + 30 < gps.course.deg() || myGPS.course0 - 30 > gps.course.deg()) {
          calculateAverage();
          mySD.savePosition();
          mySD.saveData(averageSpeed, averageHdop);
          myGPS.distanceMeasurements = 0;
          myGPS.course0 = gps.course.deg();
          myGPS.distance = 0;
          arrayPosition = 0;
          resetArray();
          screens.savedToSD = "saved";
        }
      }  
  }
  else {
    previousFails = gps.failedChecksum();
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

void clearScreen() {
  if (pushed.confirm() == true) {
    tft.fillScreen(ST7735_BLACK);
  }
}

void calculateAverage() {
  float speedsTogether = 0;
  float hdopsTogether = 0;
  for (int i = 0; i < 11; i++) {
    if (speeds[i] != 0 && hdops[i] != 0) {
      speedsTogether = speedsTogether + speeds[i];
      hdopsTogether = hdopsTogether + hdops[i];
    }
    else i = 15;
  }
  averageSpeed = speedsTogether / myGPS.distanceMeasurements;
  averageHdop = hdopsTogether / myGPS.distanceMeasurements;
}


void saveToArray() {
  speeds[arrayPosition] = gps.speed.kmph();
  hdops[arrayPosition] = gps.hdop.hdop();
}

void resetArray(){
  for(int i = 0; i < 11; i++){
    speeds[i] = 0.0;
    hdops[i] = 0.0;
  }
}
