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
String savedToSD;


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

    
    if (!SD.begin(D8)) {
      myTFT.Settings(1, 80, 150);
      tft.print("sd fail");
    }

    if (pushed.menuState == 0 && pushed.nextPrevious() == true) {
      tft.fillScreen(ST7735_BLACK);
    } 

    pushed.maxState = 4;

    printValuesForObservation();

    switch (pushed.state) {

      case 1:

        //if (Speed < 3 || Hdop > 12) Speed = 0;
        myTFT.Settings(2, 10, 10);
        myTFT.Print(gps.speed.kmph(), 4, 1);
        tft.setTextSize(1);
        tft.print(" km/h ");
        myTFT.Print(gps.speed.age(), 5, 0);
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
    myGPS.smartDelay(10);
    if (gps.hdop.hdop() < 15 && gps.speed.kmph() > 3 && gps.location.lat() != 0.0000000 && gps.location.lng() != 0.000000) {                                              //saving data
      passTime();

      if (myGPS.position0Saved == false && timePassed == true) {
        myGPS.savePosition0();
        myGPS.distanceMeasurements++;
        savedToSD = "count";

        if (gps.speed.kmph() > 6) {
          Millis0 = millis() + 2000;
        }

        else if (gps.speed.kmph() > 3 && gps.speed.kmph() < 6)  {
          Millis0 = millis() + 4000;
        }

        saveToArray();
        arrayPosition++;

        if ( myGPS.course0 + 30 < gps.course.deg() || myGPS.course0 - 30 > gps.course.deg()) {
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

  myGPS.smartDelay(200);
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

void printValuesForObservation(){
  myGPS.smartDelay(10);
  myTFT.Settings(1, 10, 40);
  myTFT.Print(gps.satellites.value(), 2, 0);
  tft.print(" sats ");
  myTFT.Print(gps.satellites.age(), 6, 0);
  myTFT.Settings(1, 10, 50);
  myTFT.Print(gps.hdop.hdop(), 4, 1);
  tft.print("hdop ");
  myTFT.Print(gps.hdop.age(), 6, 0);
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
  myTFT.Settings(1, 10, 100);
  myTFT.Print(gps.sentencesWithFix(), 5, 0);
  tft.print(" fixed");
  myTFT.Settings(1, 10, 110);
  myTFT.Print(gps.failedChecksum(), 5, 0);
  tft.print(" failed");
  myTFT.Settings(1, 10, 120);
  myTFT.Print(myGPS.distanceMeasurements, 2, 0);
  myTFT.Settings(1, 10, 130);
  tft.print(savedToSD);
}
