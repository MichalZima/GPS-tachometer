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
String savedToSD;

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

  if (gps.time.isUpdated()) {
    if (gps.hdop.hdop() < 50 && gps.speed.kmph() > 3) {                                              //saving data
      passTime();

      if (myGPS.position0Saved == false && timePassed == true) {
        myGPS.savePosition0();
        myGPS.distanceMeasurements++;
        savedToSD = "count";

//        if (gps.speed.kmph() > 6) {
//          Millis0 = millis() + 2000;
//        }
//
//        else if (gps.speed.kmph() > 3 && gps.speed.kmph() < 6)  {
//          Millis0 = millis() + 4000;
//        }

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
          savedToSD = "saved";
        }
      }
    }
  }

  //myGPS.smartDelay(495);
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void passTime() {
  if (Millis0 <= millis()) {
    if (myGPS.position0Saved == true) {
      myGPS.distanceCalculating();
      timePassed = true;
    }
  }
  else timePassed = false;
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
