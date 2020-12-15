#include "mySD.h"
#include "Menu.h"

unsigned long Millis0 = 0;
bool timePassed = true;
float Hdop = 0;

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
  myGPS.smartDelay(200);
  
  switch(pushed.menuState){
   
    case 0:
      clearScreen(4);
      Hdop = gps.hdop.hdop();
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
          myTFT.Settings(3, 0, 80);
          tft.print("screen 4");
          break;
        default:
          clearScreen(4);
          myTFT.Settings(3, 10, 10);
          tft.print("default");
          break;
      }
      break;


    case 1:
      clearScreen(9);
      menu.showMenu();
      pushed.nextPrevious();
      menu.Cursor();  
      break;

    default:
      break;
  }
  
  
  if (Hdop < 12){  
    passTime();
    if (myGPS.position0Saved == false && timePassed == true){
      Serial.print("     saved");
      myGPS.savePosition0();
      mySD.save(myGPS.Lat0, myGPS.Long0);
      if (myGPS.Speed > 40 );
      else if (myGPS.Speed > 25 && myGPS.Speed < 40) {
       Millis0 = millis() + 1000;          
      }
      else if (myGPS.Speed > 10 && myGPS.Speed < 25) {
       Millis0 = millis() + 1500;          
      }
      else if (myGPS.Speed > 4 && myGPS.Speed < 10) {
       Millis0 = millis() + 2000;          
      }
    }
  }

  if(!SD.begin(D8)) {
      myTFT.Settings(1, 10, 90);
      tft.print("sd fail");
    }
  else {
    myTFT.Settings(1, 10, 90);
    tft.print("       ");
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void passTime(){
  if (Millis0 <= millis()) {
    if (myGPS.position0Saved == true){
      myGPS.distanceCalculating();
      timePassed = true; 
    }
  }
  else timePassed = false; 
}

void clearScreen(int MAX){
  if(pushed.confirm() == true) tft.fillScreen(ST7735_BLACK);
  if(pushed.menuState == 0){
    if(pushed.nextPrevious() == true) tft.fillScreen(ST7735_BLACK);
    pushed.maxState = MAX;
  }
  else{
    if (pushed.confirm() == true){
      tft.fillScreen(ST7735_BLACK);
      pushed.maxState = MAX;
      pushed.state = 1;
    }
  }
}
