//version with error check and loops
//v.3
//graphic stuff on track iteem in menu almost done
//made blinking cursor in menu

#include "Screens.h"
#include <EEPROM.h>

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

//////////////////////////////////////////////////////////////////////////////////////

bool startup() {
  while (true) {
    tft.fillScreen(ST77XX_BLACK);
    delay(100);
    tft.drawRect(10, 80, 108, 10, ST7735_WHITE);
    delay(100);
    for (int16_t x=0; x < 108; x++) {
      tft.fillRect(10, 80, x, 10, ST7735_WHITE);
      delay(10);
    }
    if (!initialCheck()) {
    }
    else return true;
  }
}

//////////////////////////////////////////////////////////////////////////////////////

bool initialCheck() {
  if (EEPROM.read(0) == 1) {
    EEPROM.write(0, 0);
    EEPROM.commit();
    tft.fillScreen(ST77XX_BLACK);
    return true;
  }
  else {
    tft.fillScreen(ST7735_BLACK);
    myTFT.Settings(1, 10, 20);
    tft.print("zariadenie sa \n  nevyplo spravne \n\n\n");
    tft.print("  nacitavam udaje zo \n  zalohy \n\n\n");
    File file;
    file = SD.open("backup/data.txt");
    unsigned long fileSize = file.size();
    int i = 0;
    while (true) {
      file.seek(fileSize - i);
      if (file.peek() == "*") {
        String readString = file.read();
        Serial.println(readString);
        String  DATE = readString.substring(readString.indexOf("/") + 1 ,  readString.lastIndexOf("/")) ;
        String  DAILYDISTANCE = readString.substring(readString.indexOf(",") + 1 ,  readString.lastIndexOf(",")) ;
        String  TOTALDISTANCE = readString.substring(readString.indexOf("-") + 1 ,  readString.lastIndexOf("-")) ;
        Serial.println(DATE);
        Serial.println(DAILYDISTANCE);
        Serial.println(TOTALDISTANCE);
        delay(10000);
      }
      else i++;
    }
    String readString = mySD.;
    String SUBSTRING = readString.substring(readString.indexOf("") + 1 ,  readString.lastIndexOf("\t")) ;
    Serial.println(SUBSTRING);
    delay(3300);
    tft.fillScreen(ST7735_BLACK);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  SD.begin(D8);
  Serial.begin(74880);
  EEPROM.begin(512);
  myGPS.gpsSetup();
  myTFT.tftSetup();
  pushed.buttonsSetup();
  startup();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() {
  if (menu.turnOff) {
    myGPS.dailyDistance += myGPS.trackDistance;
    mySD.saveNoTrackData();
    SD.remove("backup/data.txt");
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
        if (myGPS.dailyDistance - lastSavedDailyDistance >= 0.05) {
          lastSavedDailyDistance = myGPS.dailyDistance;
          myTFT.Settings(1, 50, 130);
          myTFT.Print(mySD.backup(), 8, 0);
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
  if (gps.speed.kmph() <= 1) {
    Loops = 0;
    return false;
  }
  else if (1 < gps.speed.kmph() && gps.speed.kmph() < 10) passLoops = 10;
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
