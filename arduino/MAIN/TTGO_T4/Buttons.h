
#include <Button2.h>

#define BUTTON_A_PIN 38
#define BUTTON_B_PIN 37
#define BUTTON_C_PIN 39

Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);
Button2 buttonC = Button2(BUTTON_C_PIN);


class Pushed {

  private:
    byte buttonNextPin = 38;
    byte buttonPreviousPin = 37;
    byte buttonConfirmPin = 39;

  public:
    byte menuState = 0;
    byte maxState = 4;
    byte state = 1;
    unsigned long previousMillis = 0;
    bool screenOff = false;

    void buttonsSetup() {
      pinMode(buttonNextPin, INPUT);
      pinMode(buttonPreviousPin, INPUT);
      pinMode(buttonConfirmPin, INPUT);
    }

    bool nextPrevious() {
      if (digitalRead(buttonNextPin) == LOW) {
        if (!screenOff) {
          state++;
          if (state > maxState) state = 1;
        }
        previousMillis = millis();
        return true;
      }
      else if (digitalRead(buttonPreviousPin) == LOW) {        
        if (!screenOff) {
          state--;
          if (state < 1) state = maxState;
        }
        previousMillis = millis();
        return true;
      }
      else return false;
    }

    byte confirm() {
      if (digitalRead(buttonConfirmPin) == LOW) {
        if (!screenOff) {
          if (menuState == 0) {
            menuState = 1;
            state = 1;
          }
          else if (menuState == 1) {
            menuState = 2;
          }
        }
        previousMillis = millis();
        return true;
      }
      else return false;
    }


    int changeSettingValue(int MIN, int MAX, int NOW){
      int valueOfSetting = NOW;
      if (analogRead(buttonNextPin) >= 500) {
        valueOfSetting++;
        if (valueOfSetting > MAX) valueOfSetting = MIN;
      }
      else if (digitalRead(buttonPreviousPin) == HIGH) {
        valueOfSetting--;
        if (valueOfSetting < MIN) valueOfSetting = MAX;
      }
      return valueOfSetting;
    }
};
