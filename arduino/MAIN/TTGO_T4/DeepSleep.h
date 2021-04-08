//Pushbuttons connected to GPIO39
#define BUTTON_PIN_BITMASK 0x8000000000

RTC_DATA_ATTR int bootCount = 0;


class TurnOff {
  public:
  
  void offSetup(){  
    //Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
  
    //Print the wakeup reason for ESP32
    print_wakeup_reason();
  
    //Configure GPIO39 as ext1 wake up source for HIGH logic level
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ALL_LOW);
  }
  
  //Function that prints the reason by which ESP32 has been awaken from sleep
  void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch(wakeup_reason)
    {
      case 1  : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
      case 2  : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
      case 3  : Serial.println("Wakeup caused by timer"); break;
      case 4  : Serial.println("Wakeup caused by touchpad"); break;
      case 5  : Serial.println("Wakeup caused by ULP program"); break;
      default : Serial.println("Wakeup was not caused by deep sleep"); break;
    }
  }
};
