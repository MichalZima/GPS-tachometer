#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define TFT_CS        D1
#define TFT_RST       D0 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        D2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


class MyTFT {
  
  private:
    char convertString[10];
    
  public:  
    void tftSetup() {
      tft.initR(INITR_BLACKTAB);
      tft.fillScreen(ST7735_BLACK);
      tft.setFont();
      printColor(ST7735_WHITE);
    }
    
    void Settings(byte TEXTSIZE, byte Y, byte X) {
      tft.setTextSize(TEXTSIZE);
      tft.setCursor(Y, X);
    }

    void Print(float VARIABLE, byte NUMBERSIZE, byte DECIMALPLACES) {
      dtostrf(VARIABLE, NUMBERSIZE, DECIMALPLACES, convertString);
      tft.print(convertString);
    }

    void printColor (const uint16_t TEXTCOLOR) {
//    const uint16_t  blackColor        = ST7735_BLACK;
//    const uint16_t  redColor          = 0xF800;
//    const uint16_t  greenColor        = 0x07E0;
//    const uint16_t  yellowColor       = 0xFFE0;
//    const uint16_t  whiteColor        = ST7735_WHITE;
      tft.setTextColor(TEXTCOLOR, ST7735_BLACK);
    }

    void showMenu(){
      myTFT.Settings(1, 20, 10);
      tft.print("session");

      myTFT.Settings(1, 20, 25);
      tft.print("zobrazenie");

      myTFT.Settings(1, 20, 40);
      tft.print("ukladanie");

      myTFT.Settings(1, 20, 55);
      tft.print("statistiky");

      myTFT.Settings(1, 20, 70);
      tft.print("wifi");

      myTFT.Settings(1, 20, 85);
      tft.print("casove pasmo");

      myTFT.Settings(1, 20, 100);
      tft.print("pamat");

      myTFT.Settings(1, 20, 115);
      tft.print("reset");

      myTFT.Settings(1, 20, 130);
      tft.print("exit");
    } 

};