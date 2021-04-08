#include <WiFi.h>
#include "WiFi.h"
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
//FTPClass FTP;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {
  WiFi.disconnect();
//  WiFi.forceSleepBegin();
  delay(1);
  Serial.begin(74880);
  EEPROM.begin(512);
  myGPS.gpsSetup();
  myTFT.tftSetup();
  
  pushed.buttonsSetup();
  startup();
  if (mySD.SDsetup()) initialCheck(SD);
  else {
    myTFT.Settings(1, 12, 10);
    tft.setTextColor(0xF800, TFT_BLACK);
    tft.print("nepodarilo sa \n  nacitat kartu sd \n\n  vloz sd kartu");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    myGPS.smartDelay(2000);
  }
  tft.fillScreen(TFT_BLACK);
  pushed.previousMillis = millis();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {
  if (pushed.menuState == 0) {                                  //switch to main screen
    if (!mySD.SDsetup()) {
      myTFT.Settings(1, 80, 150);
      tft.print("sd fail");
    }
    
    if (pushed.menuState == 0 && pushed.nextPrevious() == true) tft.fillScreen(TFT_BLACK);

 
    myTFT.Settings(1, 10, 150);
    tft.print(Loops);
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

  if (menu.turnOff) {
    myGPS.dailyDistance += myGPS.trackDistance;
    mySD.saveNoTrackData(SD);
    SD.remove("backup/data.txt");
    EEPROM.put(0, myGPS.totalDistance);
    EEPROM.commit();
    pinMode (4, INPUT);
    while(1){
      delay(1000);  
    }
  }

//  if (menu.wifiState) {
//    if (pushed.menuState == 3) {
//      FTP.FTPsetup();
//      pushed.menuState = 4;
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
          mySD.backup(SD);
        }
      }
    }
  }
  
  else {
    screens.savedToSD = " pass";
  }

//  if (millis() - pushed.previousMillis > 30000) {
//    tft.fillScreen(TFT_BLACK);
//    pinMode (4, INPUT);
//    pushed.screenOff = true;
//    pushed.menuState = 0;
//    pushed.state = 1;
//  }
  
  myGPS.smartDelay(200);
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

//////////////////////////////////////////////////////////////////////////////////////

bool initialCheck(fs::FS & fs) {
  File file;
  String lastLine;
  
  if (fs.exists("backup/data.txt")) {
    myTFT.Settings(1, 10, 20);
    tft.print("zariadenie sa \n  nevyplo spravne \n\n\n");
    myGPS.smartDelay(1000);
    tft.print("  nacitavam udaje zo \n  zalohy \n\n\n");
    
    String symbol;
    
    file = fs.open("backup/data.txt");
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
        noTrackFile = fs.open("denne_statistiky.txt", FILE_WRITE);
        
        if (noTrackFile) {
          noTrackFile.println(lastLine);
          noTrackFile.close();
        }
        fs.remove("backup/data.txt");
        
        myGPS.smartDelay(1000);
        tft.print("  hotovo");
        myGPS.smartDelay(1000);
        myGPS.totalDistance = EEPROM.get(0, myGPS.totalDistance);
        Serial.println(myGPS.totalDistance);
        return false;
      }
      else Position--;
    }
  }
  
  else {
    myGPS.totalDistance = EEPROM.get(0, myGPS.totalDistance);
    Serial.println(myGPS.totalDistance);
  }
}


//////////////////////////////////////////////////////////////////////////////////////


void trackSaving() {
  if (myGPS.distanceMeasurements >= 1 or courseDifference() > 5) {
    mySD.savePosition(SD);
    mySD.saveTrackData(SD);
    myGPS.distanceMeasurements = 0;
    course0 = gps.course.deg();
    screens.savedToSD = " save";
  }
}

//////////////////////////////////////////////////////////////////////////////////////

bool passCalculating() {
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
        if (menu.trackStart) mySD.saveErrorMessage(true, SD);
        if (!menu.trackStart) mySD.saveErrorMessage(false, SD);
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
  if (pushed.confirm() == true) tft.fillScreen(TFT_BLACK);
}
