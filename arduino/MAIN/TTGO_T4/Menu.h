#include "MyTFT.h"
#include "MultiHandler.h"

MyTFT myTFT;
MultiHandler handler;


class Menu {
  private:
    byte cursorPosition0 = 1;
    byte cursorPosition = 1;
    bool cursorBlink = true;
    byte countdown = 0;
    bool countdownStarted = false;

  public:
    bool trackStart = true;
    bool turnOff = false;
    bool wifiState = false;

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    void showMenu() {
      myTFT.Settings(2, 20, 30);
      tft.print("TRASA");

      myTFT.Settings(2, 20, 50);
      tft.print("UTC");

      myTFT.Settings(2, 20, 70);
      tft.print("WIFI");

      myTFT.Settings(2, 20, 90);
      tft.print("VYPNUT");

      myTFT.Settings(2, 20, 110);
      tft.print("EXIT");

    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void Cursor() {
      if (handler.eventA > 0 || handler.eventB > 0) {
        cursorPosition0 = cursorPosition;
      }
      myTFT.Settings(2, 6, cursorPosition0);
      tft.print(" ");
      cursorPosition = handler.state * 20 + 10;
      myTFT.Settings(2, 6, cursorPosition);
      tft.print(">");
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void select() {
      switch (handler.state) {
        case 1:
          track();
          break;
        case 2:
          setTimeZone();
          break;
        case 3:
          wifi();
          break;
        case 4:
          powerOff();
          break;
        case 5:
          Exit();
          break;
        default:
          break;
      }
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void track() {
      if (!trackStart) {
        Serial.println("im here");
        
        if (!countdownStarted){
          myTFT.Settings(2, tft.width()/2 - 35, tft.height()/2 - 7);
          tft.print("START?");
        }
        
        if (handler.eventB > 0) countdownStarted = true;
             
        if (countdownStarted) { 
          tft.fillScreen(TFT_BLACK); 
          myTFT.Settings(2, tft.width()/2 - 53, tft.height()/2 - 7);
        
          tft.print("PRIPRAVIT");
          smartDelay(1000);

          tft.fillScreen(TFT_BLACK);
          tft.print("  POZOR  ");
          smartDelay(1000);
          
        
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.fillScreen(TFT_BLACK);
          tft.print("  START  ");
          smartDelay(1000);
          
          countdownStarted = false;
          trackStart = true;
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.fillScreen(TFT_BLACK);
          handler.menuState = 0;
        }
      }
      
      else if (trackStart) {
        myTFT.Settings(2, tft.width()/2 - 29, tft.height()/2 - 7);
        tft.print("STOP?");
        if (handler.eventB == 1) {
          trackStart = false;
          countdownStarted = false;
          tft.fillScreen(TFT_BLACK);
          handler.menuState = 0;
        }
      }
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void Stats() {
      myTFT.Settings(2, 10, 15);
      tft.print("stats");
      if (handler.eventC > 0) {
        tft.fillScreen(TFT_BLACK);
        handler.menuState = 1;
      }
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void setTimeZone() {
      char Array[3];
      myTFT.Settings(1, 10, 15);
      tft.print("time zone    ");
      dtostrf(myGPS.timeZoneValue, 3, 0, Array);
      tft.print(Array);  
      myGPS.timeZoneValue = handler.changeSettingValue(-12, 12, myGPS.timeZoneValue);
      if (handler.eventC > 0 ) {
        tft.fillScreen(TFT_BLACK);
        handler.menuState = 1;
      }    
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

  
    void wifi(){
      myTFT.Settings(2, 20, tft.height()/2 - 7);
      tft.println("WIFI");
      if (handler.eventC > 0) {
        tft.fillScreen(TFT_BLACK);
        wifiState = true;
        handler.menuState = 3;
      }
    }
    

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


    void powerOff() {
      myTFT.Settings(2, 20, tft.height()/2 - 7);
      tft.println("VYPNUT?");
      if (handler.eventC == 1) {
        tft.fillScreen(TFT_BLACK);
        turnOff = true;
      }
    }
    
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void Exit() {
      handler.menuState = 0;
      handler.state = 1;
      tft.fillScreen(TFT_BLACK);
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    void smartDelay(unsigned long ms) {
      unsigned long start = millis();
      while (millis() - start < ms) {
        }
    }
};
