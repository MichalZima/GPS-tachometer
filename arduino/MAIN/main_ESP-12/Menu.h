#include "MyTFT.h"
#include "Buttons.h"

MyTFT myTFT;
Pushed pushed;


class Menu {
  private:
    byte cursorPosition0 = 1;
    byte cursorPosition = 1;
    bool cursorBlink = true;

  public:
    bool trackStart = false;

//////////////////////////////////////////////////////////////////////////////////////
    
    void showMenu() {
      myTFT.Settings(2, 20, 30);
      tft.print("TRASA");

      myTFT.Settings(2, 20, 50);
      tft.print("DATA");

      myTFT.Settings(2, 20, 70);
      tft.print("UTC");

      myTFT.Settings(2, 20, 90);
      tft.print("EXIT");

    }

//////////////////////////////////////////////////////////////////////////////////////

    void Cursor() {
      if (pushed.nextPrevious()) {
        cursorPosition0 = cursorPosition;
        cursorBlink = true;
      }
      myTFT.Settings(2, 6, cursorPosition0);
      tft.print(" ");
      cursorPosition = pushed.state * 20 + 10;
      myTFT.Settings(2, 6, cursorPosition);
      if (cursorBlink) tft.print(">");
      if (!cursorBlink) tft.print(" ");
      cursorBlink = !cursorBlink;
    }

//////////////////////////////////////////////////////////////////////////////////////

    void select() {
      switch (pushed.state) {
        case 1:
          track();
          break;
        case 2:
          Stats();
          break;
        case 3:
          setTimeZone();
          break;
        case 4:
          Exit();
          break;
        default:
          break;
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    void track() {
      if (!trackStart) {
        myTFT.Settings(2, tft.width()/2 - 29, tft.height()/2 - 7);
        tft.print("START");
        if (pushed.confirm() == true) {
          trackStart = true;
          tft.fillScreen(ST7735_BLACK);
          pushed.menuState = 1;
        }
      }
      else {
        myTFT.Settings(2, tft.width()/2 - 23, tft.height()/2 - 7);
        tft.print("STOP");
        if (pushed.confirm() == true) {
          trackStart = false;
          tft.fillScreen(ST7735_BLACK);
          pushed.menuState = 1;
        }
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    void Stats() {
      myTFT.Settings(2, 10, 15);
      tft.print("stats");
      if (pushed.confirm() == true) {
        tft.fillScreen(ST7735_BLACK);
        pushed.menuState = 1;
      }
    }

//////////////////////////////////////////////////////////////////////////////////////

    void setTimeZone() {
      char Array[3];
      myTFT.Settings(1, 10, 15);
      tft.print("time zone    ");
      dtostrf(myGPS.timeZoneValue, 3, 0, Array);
      tft.print(Array);  
      myGPS.timeZoneValue = pushed.changeSettingValue(-12, 12, myGPS.timeZoneValue);
      if (pushed.confirm() == true) {
        tft.fillScreen(ST7735_BLACK);
        pushed.menuState = 1;
      }    
    }

//////////////////////////////////////////////////////////////////////////////////////

    void Exit() {
      pushed.menuState = 0;
      pushed.state = 1;
    }
};
