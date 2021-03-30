#include <SPI.h>
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();


class MyTFT {

  private:
    char convertString[12];

  public:
    void tftSetup() {
      tft.init();
      tft.begin();
      tft.setRotation(0);
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }

    void Settings(byte TEXTSIZE, int Y, int X) {
      Serial.println("2.2.1");
      tft.setTextSize(TEXTSIZE);
      tft.setCursor(Y, X);
    }

    void Print(float VARIABLE, byte NUMBERSIZE, byte DECIMALPLACES) {
      dtostrf(VARIABLE, NUMBERSIZE, DECIMALPLACES, convertString);
      tft.print(convertString);
    }

      //    const uint16_t  blackColor        = TFT_BLACK;
      //    const uint16_t  redColor          = 0xF800;
      //    const uint16_t  greenColor        = 0x07E0;
      //    const uint16_t  yellowColor       = 0xFFE0;
      //    const uint16_t  whiteColor        = TFT_WHITE;

};
