#include "mySD.h"
#include "Menu.h"

byte Loops = 0;
unsigned long Millis0 = 0;
bool timePassed = true;
float Hdop = 0;
float speeds[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float hdops[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float averageSpeed = 0;
float averageHdop = 0;
byte arrayPosition = 0;

MySD mySD;
Menu menu;


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

  if (pushed.menuState == 0) {                                  //switch to main screen

    if (pushed.menuState == 0 && pushed.nextPrevious() == true) {
      tft.fillScreen(ST7735_BLACK);
    } 
    
    pushed.maxState = 4;
    myTFT.Settings(1, 40, 40);
    myTFT.Print(Hdop, 4, 1);

    switch (pushed.state) {

      case 1:

        //if (Speed < 3 || Hdop > 12) Speed = 0;
        myTFT.Settings(2, 10, 10);
        myTFT.Print(myGPS.Speed, 4, 1);
        tft.setTextSize(1);
        tft.print(" km/h");
        break;

      case 2:

        if (myGPS.totalDistance < 1000) {
          myTFT.Settings(2, 10, 10);
          myTFT.Print(myGPS.totalDistance, 3, 0);
          tft.setTextSize(1);
          tft.print(" m");
        }

        else if (myGPS.totalDistance >= 1000) {
          myTFT.Settings(2, 10, 10);
          myTFT.Print(myGPS.totalDistance, 7, 2);
          tft.setTextSize(1);
          tft.print(" km");
        }
        break;

      case 3:

        myTFT.Settings(3, 10, 10);
        tft.print("screen 3");
        break;

      case 4:

        myTFT.Settings(2, 10, 10);
        tft.print(myGPS.realTime());
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

  if (Loops >= 5) {

    if (Hdop < 30) {                                              //saving data
      passTime();

      if (myGPS.position0Saved == false && timePassed == true) {
        Serial.print("     saved");
        myGPS.savePosition0();
        myGPS.distanceMeasurements++;

        if (myGPS.Speed > 6) {
          Millis0 = millis() + 1000;
        }

        else if (myGPS.Speed > 3 && myGPS.Speed < 6)  {
          Millis0 = millis() + 2000;
        }

        saveToArray();
        arrayPosition++;

        if (myGPS.distanceMeasurements >= 10 || myGPS.course0 + 30 >= gps.course.deg() || myGPS.course0 - 30 <= gps.course.deg()) {
          calculateAverage();
          mySD.savePosition();
          mySD.saveData(averageSpeed, averageHdop);
          myGPS.distanceMeasurements = 0;
          myGPS.course0 = gps.course.deg();
          myGPS.distance = 0;
          arrayPosition = 0;
          resetArray();
        }
      }
    }
  }


  if (!SD.begin(D8)) {
    myTFT.Settings(1, 10, 90);
    tft.print("sd fail");
  }

  myGPS.smartDelay(200);
  Hdop = gps.hdop.hdop();
  Loops++;
  
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
