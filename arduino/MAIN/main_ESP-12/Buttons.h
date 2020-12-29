
class Pushed {

  private:
    byte buttonNextPin = A0;
    byte buttonPreviousPin = 9;
    byte buttonConfirmPin = 10;

  public:
    byte menuState = 0;
    byte maxState = 4;
    byte state = 1;

    void buttonsSetup() {
      pinMode(buttonNextPin, INPUT);
      pinMode(buttonPreviousPin, INPUT);
      pinMode(buttonConfirmPin, INPUT);
    }

    bool nextPrevious() {
      if (analogRead(buttonNextPin) >= 500) {
        state++;
        if (state > maxState) state = 1;
        return true;
      }
      else if (digitalRead(buttonPreviousPin) == HIGH) {
        state--;
        if (state < 1) state = maxState;
        return true;
      }
      else return false;
    }

    byte confirm() {
      if (digitalRead(buttonConfirmPin) == HIGH) {
        if (menuState == 0) {
          menuState = 1;
          state = 1;
        }
        else if (menuState == 1) {
          menuState = 2;
        }
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
