//Pushbuttons connected to GPIO39
#define BUTTON_PIN_BITMASK 0x8000000000


class TurnOff {
  public:
  
  void Setup(){   
    //Configure GPIO39 as ext1 wake up source for HIGH logic level
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);
  }
};
