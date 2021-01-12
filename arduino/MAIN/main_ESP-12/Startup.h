#include <EEPROM.h>

class Startup {
  public:
    bool initialCheck() {
      byte eepromValue = EEPROM.read(0); 
      if (eepromValue == 1) {
        EEPROM.write(0, 0);
        EEPROM.commit();
        return true;
      }
      else if (eepromValue == 0) return false;
    }
};
