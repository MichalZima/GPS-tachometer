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
  tft.fillScreen(ST77XX_BLACK);
  delay(100);
  myTFT.Settings(1, 34, 76);
  tft.print("Nacitavam...");
  tft.drawRect(10, 75, 108, 10, ST7735_WHITE);
  delay(100);
  for (int16_t x=0; x < 108; x++) {
    tft.fillRect(10, 75, x, 10, ST7735_WHITE);
    delay(10);
  }
  tft.fillScreen(ST77XX_BLACK);
}

//////////////////////////////////////////////////////////////////////////////////////

bool initialCheck() {
  File file;
  String lastLine;
  
  if (SD.exists("backup/data.txt")) {
    myTFT.Settings(1, 10, 20);
    tft.print("zariadenie sa \n  nevyplo spravne \n\n\n");
    myGPS.smartDelay(1000);
    tft.print("  nacitavam udaje zo \n  zalohy \n\n\n");
    
    String symbol;
    
    file = SD.open("backup/data.txt");
    unsigned long Position = file.size();
    while (true) {
      file.seek(Position);
      symbol = char(file.read());
      
      if (symbol == "*") { 
        while (true){
          symbol = char(file.read());
          if (symbol != ";") {  
            lastLine += symbol;
          }
          else break;
        }
        file.close();
        
        myGPS.smartDelay(1000);
        tft.print("  aktualizujem data\n\n\n");
        Serial.println();
        Serial.println(lastLine);
        
        File noTrackFile;
        noTrackFile = SD.open("denne_statistiky.txt", FILE_WRITE);
        
        if (noTrackFile) {
          noTrackFile.println(lastLine);
          noTrackFile.close();
        }
        
        myGPS.smartDelay(1000);
        tft.print("  hotovo");
        myGPS.smartDelay(1000);
        return false;
      }
      else Position--;
    }
  }
  
  else {
    myGPS.totalDistance = EEPROM.get(0, myGPS.totalDistance);
  }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(74880);
  EEPROM.begin(512);
  myGPS.gpsSetup();
  myTFT.tftSetup();
  pushed.buttonsSetup();
  SD.begin(D8);
  startup();
  if (SD.begin(D8)) initialCheck();
  else {
    myTFT.Settings(1, 10, 10);
    tft.setTextColor(ST7735_RED, ST7735_BLACK);
    tft.print("nepodarilo sa \n nacitat kartu sd \n\n vloz sd kartu");
    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    myGPS.smartDelay(3000);
  }
  tft.fillScreen(ST7735_BLACK);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop() {
  if (menu.turnOff) {
    myGPS.dailyDistance += myGPS.trackDistance;
    mySD.saveNoTrackData();
    SD.remove("backup/data.txt");
    EEPROM.put(0, myGPS.totalDistance);
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
        if (myGPS.dailyDistance - lastSavedDailyDistance >= 0.5) {
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
