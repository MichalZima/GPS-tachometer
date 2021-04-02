

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
        if (screenOff) {
          pinMode (4, OUTPUT);
          screenOff = false;
        }
        previousMillis = millis();
        return true;
      }
      else if (digitalRead(buttonPreviousPin) == LOW) {        
        if (!screenOff) {
          state--;
          if (state < 1) state = maxState;
        }
        if (screenOff) {
          pinMode (4, OUTPUT);
          screenOff = false;
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
        if (screenOff) {
          pinMode (4, OUTPUT);
          screenOff = false;
        }
        previousMillis = millis();
        return true;
      }
      else return false;
    }


    int changeSettingValue(int MIN, int MAX, int NOW){
      int valueOfSetting = NOW;
      if (digitalRead(buttonNextPin) == LOW) {
        valueOfSetting++;
        if (valueOfSetting > MAX) valueOfSetting = MIN;
      }
      else if (digitalRead(buttonPreviousPin) == LOW) {
        valueOfSetting--;
        if (valueOfSetting < MIN) valueOfSetting = MAX;
      }
      return valueOfSetting;
    }
};
