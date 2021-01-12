#include <EEPROM.h>

class Startup {
  public:
    bool initialCheck() {
      if (EEPROM.read(0) == 1) {
        EEPROM.update(0, 0);
        EEPROM.commit();
        return true;
      }
      else return false;
    }
};
