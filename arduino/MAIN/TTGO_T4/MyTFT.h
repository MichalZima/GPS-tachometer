



class MyTFT {

  private:
    char convertString[12];

  public:
    void Setup() {
      tft.init();
      tft.setRotation(0);
      tft.fillScreen(TFT_BLACK);
    }

    void Settings(byte TEXTSIZE, int Y, int X) {
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
