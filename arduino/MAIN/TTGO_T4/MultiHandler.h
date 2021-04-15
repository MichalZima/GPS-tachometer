
#include "Button2.h"


class MultiHandler {


  public:
    byte menuState = 0;
    byte maxState = 4;
    byte state = 1;
    unsigned long previousMillis = 0;
    bool screenOff = false;
    byte eventA;
    byte eventB;
    byte eventC;

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/    

    void buttonHandlerA(Button2& btn) {
      switch (btn.getClickType()) {
        case SINGLE_CLICK:
            if (menuState < 2) {
              if (!screenOff) {
                state++;
                if (state > maxState) state = 1;
              }
            }
            
            if (screenOff) {
              pinMode (4, OUTPUT);
              screenOff = false;
            }
            
            previousMillis = millis();
            eventA = 1;
            break;
            
        case DOUBLE_CLICK:
            eventA = 2;
            break;
        case TRIPLE_CLICK:
            eventA = 3;
            break;
        case LONG_CLICK:
            eventA = 4;
            break;
      }
    }
    
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    void buttonHandlerB(Button2& btn) {
      switch (btn.getClickType()) {
        case SINGLE_CLICK:
            if (menuState < 2) {
              if (!screenOff) {
                state--;
                if (state < 1) state = maxState;
              }
            }
            
            if (screenOff) {
              pinMode (4, OUTPUT);
              screenOff = false;
            }
            
            previousMillis = millis();
            eventB = 1;
            break;
            
        case DOUBLE_CLICK:
            eventB = 2;
            break;
        case TRIPLE_CLICK:
            eventB = 3;
            break;
        case LONG_CLICK:
            eventA = 4;
            break;
            
      }
    }
    
/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
    void buttonHandlerC(Button2& btn) {
      switch (btn.getClickType()) {
        case SINGLE_CLICK:
          if (menuState < 2) {
            if (!screenOff) {
              if (menuState == 0) {
                menuState = 1;
                state = 1;
              }
              
              else if (menuState == 1) {
                menuState = 2;
              }
            }
          }
          
          
          if (screenOff) {
            pinMode (4, OUTPUT);
            screenOff = false;
          }
          
          previousMillis = millis();
          eventC = 1;
          break;
        
       case DOUBLE_CLICK:
            eventC = 2;
            break;
        
        case TRIPLE_CLICK:
            eventC = 3;
            break;
        case LONG_CLICK:
            eventC = 4;
            break;
      }
    }

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    int changeSettingValue(int MIN, int MAX, int NOW){
      int valueOfSetting = NOW;
      if (eventA > 0) {
        valueOfSetting++;
        if (valueOfSetting > MAX) valueOfSetting = MIN;
      }
      else if (eventB > 0) {
        valueOfSetting--;
        if (valueOfSetting < MIN) valueOfSetting = MAX;
      }
      return valueOfSetting;
    }

};
