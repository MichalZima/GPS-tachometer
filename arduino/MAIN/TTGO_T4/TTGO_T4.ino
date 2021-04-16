#include <WiFi.h>
#include "WiFi.h"
#include <EEPROM.h>
#include "DeepSleep.h"
#include "Screens.h"

unsigned long updateTime = 0;
unsigned long Millis0 = 0;
byte passXTimes;
byte calculatingPassed = 0;
int locationUpdatedXtimes = 0;
int course0 = gps.course.deg();
int timerCount = 0;
float lastSavedDailyDistance = 0;
bool previousTrackState = false;
bool updateScreen;
byte Loop;

#define BUTTON_A_PIN  38
#define BUTTON_B_PIN  37
#define BUTTON_C_PIN  39

Screens screens;
//FTPClass FTP;
TurnOff off;

Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);
Button2 buttonC = Button2(BUTTON_C_PIN);








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  WiFi.disconnect();
  Serial.begin(74880);
  EEPROM.begin(512);
  
  myGPS.Setup();
  myTFT.Setup();
  off.Setup();
  buttonHandlerSetup();
  startup();
  if (mySD.Setup()) initialCheck(SD);
  
  else {
    myTFT.Settings(1, 12, 10);
    tft.setTextColor(0xF800, TFT_BLACK);
    tft.print("nepodarilo sa \n  nacitat kartu sd \n\n  vloz sd kartu");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    myGPS.smartDelay(2000);
  }
  
  tft.fillScreen(TFT_BLACK);
  handler.previousMillis = millis();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {  
  handler.eventA = 0;
  handler.eventB = 0;
  handler.eventC = 0;
  
  buttonA.loop();
  buttonB.loop();
  buttonC.loop();

  screenUpdate();
  
  if (handler.menuState == 0) {
    //switch to main screen
    
    if (!mySD.Setup()) {
      myTFT.Settings(1, 80, 150);
      tft.print("sd fail");
    }

    myTFT.Settings(1, 10, 150);
    handler.maxState = 4;
    
    if (handler.eventA == 1 || handler.eventB == 1) {
      updateScreen = true;
      tft.fillScreen(TFT_BLACK);
    }
 
    if (updateScreen) {
      switch (handler.state) {
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
          break;
      }
    }
  }

  else if (handler.menuState == 1) {                             //switching to menu
    handler.maxState = 5;
    menu.Cursor();
    menu.showMenu();
  }

  else if (handler.menuState == 2) {                             //select between options in menu
    menu.select();
  }

  if (menu.turnOff) {
    myGPS.dailyDistance += myGPS.trackDistance;
    mySD.saveNoTrackData(SD);
    EEPROM.put(0, myGPS.totalDistance);
    EEPROM.commit();
    esp_deep_sleep_start();
  }

//  if (menu.wifiState) {
//    if (handler.menuState == 3) {
//      FTP.FTPsetup();
//      handler.menuState = 4;
//    }
//    FTP.FTPloop();
//  }



  if (passCalculating()) {                                      //saving data
    if (myGPS.position0Saved == false) {
      myGPS.savePosition0();
      myGPS.distanceMeasurements++;
      screens.savedToSD = "count";

      //      myGPS.saveToArray();
      //      myGPS.arrayPosition++;
      
      if (menu.trackStart) trackSaving();
      
      else if (!menu.trackStart) {
        if (myGPS.dailyDistance - lastSavedDailyDistance >= 0.5) {
          lastSavedDailyDistance = myGPS.dailyDistance;
        }
      }
    }
  }
  
  else {
    screens.savedToSD = " pass";
  }

  if (millis() - handler.previousMillis > 30000) {
    tft.fillScreen(TFT_BLACK);
    pinMode (4, INPUT);
    handler.screenOff = true;
    handler.menuState = 0;
    handler.state = 1;
  }

  clearScreen();
  myGPS.smartDelay(1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool startup() {
  tft.fillScreen(TFT_BLACK);
  delay(50);
  myTFT.Settings(1, 34, 76);
  tft.println("Nacitavam...");
  tft.drawRect(10, 75, 108, 10, TFT_WHITE);
  delay(50);
  for (int16_t x=0; x < 108; x++) {
    tft.fillRect(10, 75, x, 10, TFT_WHITE);
    delay(5);
  }
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void trackSaving() {
  if (myGPS.distanceMeasurements >= 1 or courseDifference() > 5) {
    mySD.savePosition(SD);
    mySD.saveTrackData(SD);
    mySD.saveJSONTrackData(SD);
    myGPS.distanceMeasurements = 0;
    course0 = gps.course.deg();
    screens.savedToSD = " save";
  }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool passCalculating() {
byte passLoop;
  if (gps.speed.kmph() <= 0) {
    Loop = 0;
    return false;
  }
  if (0 < gps.speed.kmph() && gps.speed.kmph() < 10) passLoop = 2;
  else if (10 <= gps.speed.kmph()) passLoop = 1;
  
  if (Loop == passLoop) {
    if (myGPS.position0Saved == true) {
      if (myGPS.distanceCalculating(menu.trackStart)) {
        Loop = 0;
        return true;
      }
      else {
        mySD.saveErrorMessage(SD);
        Loop = 0;
        return false;
      }
    }
  }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void screenUpdate() {
        if (millis() < updateTime + 1000) {
          updateScreen = false;
        }
        else {
          updateTime = millis();
          updateScreen = true;
          Loop++;
        }
    }
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

int courseDifference() {
  int difference = course0 - gps.course.deg();
  return abs(difference);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void clearScreen() {
  if (handler.eventC == 1) {
    tft.fillScreen(TFT_BLACK);
    handler.eventA = 0;
    handler.eventB = 0;
    handler.eventC = 0; 
  }
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

bool initialCheck(fs::FS & fs) {
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void buttonHandlerSetup() {
  buttonA.setClickHandler(handlerA);
  buttonA.setLongClickHandler(handlerA);
  buttonA.setDoubleClickHandler(handlerA);
  buttonA.setTripleClickHandler(handlerA);
  
  buttonB.setClickHandler(handlerB);
  buttonB.setLongClickHandler(handlerB);
  buttonB.setDoubleClickHandler(handlerB);
  buttonB.setTripleClickHandler(handlerB);
  
  buttonC.setClickHandler(handlerC);
  buttonC.setLongClickHandler(handlerC);
  buttonC.setDoubleClickHandler(handlerC);
  buttonC.setTripleClickHandler(handlerC);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


void handlerA(Button2& btn) {
  handler.buttonHandlerA(btn);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void handlerB(Button2& btn) {
  handler.eventB = 0;
  handler.buttonHandlerB(btn);
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

void handlerC(Button2& btn) {
  handler.eventC = 0;
  handler.buttonHandlerC(btn);
}
