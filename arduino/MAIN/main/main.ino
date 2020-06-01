#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include "RTClib.h"

Adafruit_SSD1306 display(128, 64);
RTC_DS1307 realTimeClock;

const unsigned long ZeroTimeout = 500000;
volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;

int totalPulses = 0;

byte confirmButtonValue = 0;
byte wait = 0;
byte wait2 = 0;
const byte buttonUPpin = 6;
const byte buttonDOWNpin = 5;
int scrollValue = 0;
bool confirm = false;
short int wheelDiameter = 702;


///////////////////////////////////////////////////////////////////////////////////////

void scrollFunction(int minValue, int maxValue) {
  if (digitalRead(buttonUPpin) == HIGH) {
    if (wait == 0) {
      scrollValue++;
      wait = 1;
    }
  }
  else {
    wait = 0;
  }

  if (scrollValue > maxValue) {
    scrollValue = minValue;
  }

  if (digitalRead(buttonDOWNpin) == HIGH) {
    if (wait2 == 0) {
      scrollValue--;
      wait2 = 1;
    }
  }
  else {
    wait2 = 0;
  }

  if (scrollValue < minValue) {
    scrollValue = maxValue;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class KMHClass {
  private:
    const byte PulsesPerRevolution = 1;
    const byte numReadings = 2;
    unsigned long FrequencyRaw;
    unsigned long FrequencyReal;
    double KMH;
    unsigned long RPM;
    unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
    unsigned int ZeroDebouncingExtra;
    unsigned long CurrentMicros = micros();
    const unsigned long ZeroTimeout = 500000;

  public:
    void KMHCalculation() {
      LastTimeCycleMeasure = LastTimeWeMeasured;
      CurrentMicros = micros();
      if (CurrentMicros < LastTimeCycleMeasure)
      {
        LastTimeCycleMeasure = CurrentMicros;
      }
      FrequencyRaw = 10000000000 / PeriodAverage;  // Calculate the frequency using the period between pulses.

      if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra)
      {
        FrequencyRaw = 0;
        ZeroDebouncingExtra = 2000;
      }
      else
      {
        ZeroDebouncingExtra = 0;
      }
      FrequencyReal = FrequencyRaw / 10000;

      RPM = FrequencyRaw / PulsesPerRevolution * 60;
      RPM = RPM / 10000;
      KMH = RPM * wheelDiameter * 3.142 * 60;
      KMH = KMH / 1000;
    }



    void PrintKMH() {
      KMHCalculation();
      char string[6];
      dtostrf(KMH, 4, 1, string);

      display.clearDisplay();
      display.setTextSize(3);
      display.setCursor(0, 25);
      display.print(string);
      display.display();
      Serial.println(wheelDiameter);
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class RTCClass {
  private:
    DateTime now = realTimeClock.now();
    int Year = now.year();
    byte Month = now.month();
    byte Day = now.day();
    byte Hour = now.hour();
    byte Minute = now.minute();
    byte Second = now.second();
  
  public:
  
    void printTime() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 25);

      display.print(now.hour(), DEC);
      display.print(':');
      if (now.minute() < 10) {
        display.print("0");
        display.print(now.minute(), DEC);
      }
      else {
        display.print(now.minute(), DEC);
      }
      display.print(':');
      if (now.second() < 10) {
        display.print("0");
        display.print(now.second(), DEC);
      }
      else {
        display.print(now.second(), DEC);
      }

      display.display();
    }


    void printDate() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 25);

      display.print(now.day(), DEC);
      display.print('.');
      display.print(now.month(), DEC);
      display.print('.');
      display.print(now.year(), DEC);

      display.display();
    }


    void printSettingTime() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 5);

      display.print(Hour);
      display.print(":");
      if (Minute < 10) {
        display.print("0");
        display.print(Minute);
      }
      else {
        display.print(Minute);
      }
      display.print(":");
      if (Second < 10) {
        display.print("0");
        display.print(Second);
      }
      else {
        display.print(Second);
      }
      display.display();
    }


    void printSettingDate() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 5);

      display.print(Day);
      display.print(".");
      display.print(Month);
      display.print(".");
      display.print(Year);
      display.display();
    }


    void settingTime() {
      byte i = 0;
      scrollValue = Hour;

      while (i < 3) {
        printSettingTime();

        switch (i) {
          case 0:
            scrollFunction(0, 23);
            Hour = scrollValue;
            if (confirm == true) {
              scrollValue = Minute;
              confirm = false;
              i = 1;
              break;
            }
            break;

          case 1:
            scrollFunction(0, 59);
            Minute = scrollValue;
            if (confirm == true) {
              scrollValue = Second;
              confirm = false;
              i = 2;
              break;
            }
            break;

          case 2:
            scrollFunction(0, 59);
            Second = scrollValue;
            if (confirm == true) {
              confirm = false;
              i = 3;
              break;
            }
            break;

          default:
            ;
        }
      }
      realTimeClock.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
      confirmButtonValue = 8;
    }


    void settingDate() {
      byte i = 0;
      scrollValue = Day;

      while (i < 3) {
        printSettingDate();

        switch (i) {
          case 0:
            scrollFunction(0, 31);
            Day = scrollValue;
            if (confirm == true) {
              scrollValue = Month;
              confirm = false;
              i = 1;
              break;
            }
            else {
              ;
            }
            break;

          case 1:
            scrollFunction(0, 12);
            Month = scrollValue;
            if (confirm == true) {
              scrollValue = Year;
              confirm = false;
              i = 2;
              break;
            }
            else {
              ;
            }
            break;

          case 2:
            scrollFunction(2019, 2060);
            Year = scrollValue;
            if (confirm == true) {
              confirm = false;
              i = 3;
              break;
            }
            else {
              ;
            }
            break;

          default:
            ;
        }
      }
      realTimeClock.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
      confirmButtonValue = 8;
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class MenuClass {
  public:
    void printMenu(){
      display.clearDisplay();
      display.setTextSize(1);

      moveCursor(4);
      
      display.setCursor(20, 0);
      display.print("statistiky");
      display.setCursor(20, 10);
      display.print("stav baterie");
      display.setCursor(20, 20);
      display.print("mod");
      display.setCursor(20, 30);
      display.print("nastavenia");
      display.setCursor(20, 40);
      display.print("spat");
      
      display.display();

      confirmInMenu();
    }

    void confirmInMenu() {
      if(confirm == true) {
        confirm = false;
        switch(scrollValue) {
          case 0:
            Serial.println("statistiky");
            //confirmButtonValue = 5;
            break;
            
          case 1:
            Serial.println("stav baterie");
            //confirmButtonValue = 6;
            break;
  
          case 2:
            Serial.println("mod");
            //confirmButtonValue = 7;
            break;
  
          case 3:
            scrollValue = 0;
            confirmButtonValue = 8;
            break; 
  
          case 4:
            confirmButtonValue = 0;
            break; 
        }
      }
    }
  
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class SettingsClass {
  public:
    void printSettings(){
      display.clearDisplay();
      display.setTextSize(1);

      confirmInSettings();
      moveCursor(3);
      
      display.setCursor(20, 0);
      display.print("cas");
      display.setCursor(20, 10);
      display.print("datum");
      display.setCursor(20, 20);
      display.print("priemer kolesa");
      display.setCursor(20, 30);
      display.print("spat");
      
      display.display();
    }

    void confirmInSettings() {
      if(confirm == true) {
        confirm = false;      
        switch(scrollValue) {
          case 0:
            confirmButtonValue = 10;
            break;
            
          case 1:
            confirmButtonValue = 11;
            break;
  
          case 2:
            scrollValue = 0;
            confirmButtonValue = 12;
            break;
  
          case 3:
            scrollValue = 3;
            confirmButtonValue = 1;
            break;  
        }
      }
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class SDClass {
  public:
    void saveStatistic() {
      // na ukladanie roznych statistik, maximalnych aj celkovych
    }  

    void loadStatistic() {
      //na nacitavanie roznych statistik, maximalnych aj celkovych 
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CompareClass {
  public:
    void 
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void totalDistance(){
  float distance;
  distance = totalPulses * 2.234;
  distance = distance / 1000; 

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(distance);
  display.display();
}


void setWheelDiameter() {
  KMHClass kmh;
  char diameterTagsList[6][10] = {"26x2.3 ", "26x3.0 ", "27.5x2.3 ", "27.5x3.0 ", "29x2.3 ", "29x3.0 "};
  int diameters[6] = {677, 702, 702, 740, 740, 778};
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  
  scrollFunction (0, 5);
  display.print(diameterTagsList[scrollValue]);

  if(confirm == true){
    confirm = false;
    wheelDiameter = diameters[scrollValue];
    scrollValue = -1;
    confirmButtonValue = 8;
  }
  
  display.display();
}


//##################################################################################################################################################################
//##################################################################################################################################################################




void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), Pulse_Event, RISING);
  attachInterrupt(digitalPinToInterrupt(3), confirmFunction, RISING);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);

}

KMHClass kmh;

void loop() {
  RTCClass rtc;
  MenuClass menu;
  SettingsClass settings;
  
  switch (confirmButtonValue) {
    case 0:
      Main();
      if (confirm == true) {
        confirm = false;
        scrollValue = 0;
        confirmButtonValue = 1;
        break;
      }
      break;

    case 1:
      menu.printMenu();
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      break;

    case 8:
      settings.printSettings();
      break;

    case 10:
      rtc.settingTime();
      break;

    case 11:
      rtc.settingDate();

    case 12:
      setWheelDiameter();
      break;
  }
}




//##################################################################################################################################################################
//##################################################################################################################################################################


void Main(){
  RTCClass rtc;
  scrollFunction(0, 3);
  switch(scrollValue){
    case 0:
      kmh.PrintKMH();
      break;
    case 1:
      totalDistance();
      break;
    case 2:
      rtc.printTime();
      break;
    case 3:
      rtc.printDate();
      break;
  }
}


void moveCursor(byte maxCursorValue) {
  scrollFunction(0, maxCursorValue);
  
  switch(scrollValue) {
    case 0:
      display.setCursor(5, 0);
      display.write(">");
      break;
      
    case 1:
      display.setCursor(5, 10);
      display.write(">");
      break;

    case 2:
      display.setCursor(5, 20);
      display.write(">");
      break;

    case 3:
      display.setCursor(5, 30);
      display.write(">");
      break; 

    case 4:
      display.setCursor(5, 40);
      display.write(">");
      break;
  }
}


void Pulse_Event() {
  totalPulses++;
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();

  if (PulseCounter >= AmountOfReadings)
  {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;

    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);

    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  }
  else
  {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void confirmFunction() {
  confirm = true;
}
