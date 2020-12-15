#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define TFT_CS        D1
#define TFT_RST       D0 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC        D2

static const int RXPin = D4, TXPin = D3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss (RXPin, TXPin);
File file;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// char array for printing on the screen
int buttonValue = 0;
float Speed = 0;
int Satelites = 0;
float Hdop;
unsigned int Fail;
unsigned int lowSignal = 0;
unsigned int veryLowSignal = 0;
unsigned int signalLost = 0;
unsigned long Millis0 = 0;
bool position0Saved = false;
bool distanceCalculated = false;
bool timePassed = true;
bool changedToKM = false;
char convertString[10];
float Lat0;
float Long0;
float distance; 
float totalDistance = 0;
float distance0 = 0;


//.....................................................................................................................................................................
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available()) 
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

class DistanceClass {    
  public:
    void savePosition0(){
      smartDelay(0);
      Lat0 = gps.location.lat();
      Long0 = gps.location.lng();
      position0Saved = true;
    }
    
    void distanceCalculating(){
      distance = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), Lat0, Long0);
      position0Saved = false;
      distanceCalculated = true;
      distance0 += distance; 
      if (totalDistance < 1000) totalDistance += distance;
      
      else if (totalDistance >= 1000) {
        if (changedToKM == false) {
          totalDistance = totalDistance / 1000;
          tft.fillScreen(ST7735_BLACK);
          changedToKM == true;
        }
        distance = distance / 1000.0;
        totalDistance += distance; 
      }
      
      smartDelay(0);
    }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

class PrintingClass {
  public:
    
    void Settings(byte TEXTSIZE, byte Y, byte X) {
      tft.setTextSize(TEXTSIZE);
      tft.setCursor(Y, X);
    }

    void Value(float VARIABLE, byte NUMBERSIZE, byte DECIMALPLACES) {
      dtostrf(VARIABLE, NUMBERSIZE, DECIMALPLACES, convertString);
      tft.print(convertString);
    }
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(74880);
  ss.begin(GPSBaud);
  SD.begin(D8);
  Serial.println("\n\n\n");
//  pinMode(2, INPUT);
//  pinMode(A4, INPUT);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setFont();

  printColor(ST7735_WHITE);  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PrintingClass Print;
DistanceClass Distance;

void loop() { 
  if(!SD.begin(D8)) {
      Print.Settings(1, 10, 90);
      tft.print("sd fail");
    }
    else {
      Print.Settings(1, 10, 90);
      tft.print("       ");
    }
   
  smartDelay(0); 
  signalStrength();

  Hdop = gps.hdop.hdop();
  Speed = gps.speed.kmph();

  Print.Settings(1, 40, 40);
  Print.Value(Hdop, 4, 1);
  
  if (Hdop < 12){  
    passTime();
  
    if (position0Saved == false && timePassed == true){
      
      Distance.savePosition0();
      saveToSD();
      
      if (Speed > 40 );
      
      else if (Speed > 25 && Speed < 40) {
       Millis0 = millis() + 1000;          
      }
      
      else if (Speed > 10 && Speed < 25) {
       Millis0 = millis() + 1500;          
      }
  
      else if (Speed > 4 && Speed < 10) {
       Millis0 = millis() + 2000;          
      }
    }
  }
  next();
  
  switch (buttonValue) {
    //printing speed
    case 0:
      //if (Speed < 4 || Hdop > 12) Speed = 0; 
      Print.Settings(2, 10, 10);
      Print.Value(Speed, 4, 1);
      tft.setTextSize(1);
      tft.print(" km/h");
      break;

    //printing distance
    case 1:
      if (totalDistance < 1000) {
        Print.Settings(2, 10, 10);
        Print.Value(totalDistance, 3, 0);
        tft.setTextSize(1);
        tft.print(" m");  
      }
      else if (totalDistance >= 1000) {
        Print.Settings(2, 10, 10);
        Print.Value(totalDistance, 7, 2);
        tft.setTextSize(1);
        tft.print(" km");
      }
      break;


    //printing signal strength
    case 2:
      Fail = gps.failedChecksum();
      Print.Settings(1, 10, 115);
      Print.Value(Fail, 4, 0);
      
      Print.Settings(1, 50, 115);
      Print.Value(lowSignal, 5, 0);
      
      Print.Settings(1, 10, 125);
      Print.Value(veryLowSignal, 5, 0);
  
      Print.Settings(1, 50, 125);
      Print.Value(signalLost, 4, 0);
      break;
    } 
  smartDelay(500);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//.....................................................................................................................................................................

void printColor (const uint16_t TEXTCOLOR) {
//  const uint16_t  blackColor        = ST7735_BLACK;
//  const uint16_t  redColor          = 0xF800;
//  const uint16_t  greenColor        = 0x07E0;
//  const uint16_t  yellowColor       = 0xFFE0;
//  const uint16_t  whiteColor        = ST7735_WHITE;

  tft.setTextColor(TEXTCOLOR, ST7735_BLACK);
}
//.....................................................................................................................................................................

void passTime(){
  DistanceClass Distance;
  if (Millis0 <= millis()) {
    if (position0Saved == true){
      Distance.distanceCalculating();
      timePassed = true; 
    }
  }
  else timePassed = false; 
}
//.....................................................................................................................................................................

void signalStrength(){   
  if (Satelites < 2) signalLost++;
  else if (Hdop > 12) veryLowSignal++;
  else if (Hdop > 4) lowSignal++;
}    
//.....................................................................................................................................................................

void saveToSD() {
  if (distance0 >= 30) {
    if(!SD.begin(D8)) {
      Print.Settings(1, 10, 90);
      tft.print("       ");
      tft.print("sd fail");
    }
    
    file = SD.open("poloha.txt", FILE_WRITE); //creating the file 
    if (file) { 
      char latString[9];
      char longString[9];
      dtostrf(Lat0, 9, 6, latString);
      dtostrf(Long0, 9, 6, longString);
      file.print("[");
      file.print(longString); 
      file.print(", ");     
      file.print(latString);    
      file.println("], ");   
      Serial.println("file");
      file.close();   
    }
    distance0 = 0; 
  }    
}



void next(){
  if (analogRead(A0) >= 600) {
    buttonValue++;
    if (buttonValue > 1) buttonValue = 0;
    tft.fillScreen(ST7735_BLACK);
  }
}
//.....................................................................................................................................................................
