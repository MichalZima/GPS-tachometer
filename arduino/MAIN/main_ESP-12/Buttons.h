
class Pushed {

  private:
    byte buttonNextPin = A0;
    byte buttonPreviousPin = 9;
    byte buttonConfirmPin = 10;

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
      if (analogRead(buttonNextPin) >= 200) {
        Serial.println("next");
        if (!screenOff) {
          state++;
          if (state > maxState) state = 1;
        }
        previousMillis = millis();
        return true;
      }
//      else if (digitalRead(buttonPreviousPin) == HIGH) {
//        Serial.println("previous");
//        if (!screenOff) {
//          state--;
//          if (state < 1) state = maxState;
//        }
//        previousMillis = millis();
//        return true;
//      }
      else return false;
    }

    byte confirm() {
      Serial.println("confirm");
      if (digitalRead(buttonConfirmPin) == HIGH) {
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
