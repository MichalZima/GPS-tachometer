#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8
#define SerialDebugging true

static const int RXPin = 7, TXPin = 6;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss (RXPin, TXPin);

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const uint16_t  blackColor        = ST7735_BLACK;
const uint16_t  redColor          = 0xF800;
const uint16_t  greenColor        = 0x07E0;
const uint16_t  yellowColor       = 0xFFE0;
const uint16_t  whiteColor        = ST7735_WHITE;

// char array for printing on the screen
char rychlost[7] = {"0.0"};
char satelity[2];
char vzdialenost[4] = {"0"};
char latString[10] = {"0"};
char longString[10] = {"0"};
char hdopString[5];
char courseString[4] = {"0"};

float Speed = 0;
int satelites = 0;
long Lat0;
long Long0;
long Lat;
long Long;
long distance = 0;
float rawLat;
float rawLong;
float Hdop;
unsigned int fail;
unsigned int lowSignal = 0;
unsigned int veryLowSignal = 0;
unsigned int signalLost = 0;
unsigned long Millis0 = 0;
bool position0Saved = false;
bool distanceCalculated = false;
bool timePassed = true;
float totalDistance = 0;

int buttonState = 0;


void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  pinMode(2, INPUT);
  pinMode(A4, INPUT);
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(blackColor);
  tft.setFont();

}



void next(){
  buttonState++;
  if (buttonState > 2) buttonState = 0;
  tft.fillScreen(ST7735_BLACK);
}

void pushButton(){
  if (digitalRead(2) == HIGH){
    if (buttonState == 0)
      buttonState = 1;
  
    else if (buttonState == 1)
      buttonState = 2;
 
    else if (buttonState == 2) buttonState = 0;
    
    tft.fillScreen(blackColor);
  }
}

void loop() {  
  smartDelay(0);  
  Hdop = gps.hdop.hdop();
  Speed = gps.speed.kmph();
  satelites = gps.satellites.value();
  fail = gps.failedChecksum();

  if (satelites < 2) signalLost++;
  else if (Hdop > 15) veryLowSignal++;
  else if (Hdop > 4) lowSignal++;

  dtostrf(satelites, 2, 0, satelity);
  tft.setTextSize(1);
  tft.setTextColor(whiteColor, blackColor);
  tft.setCursor(10,40);
  tft.print(satelity);

  dtostrf(Hdop, 4, 1, hdopString);
  tft.setTextSize(1);
  tft.setCursor(40,40);
  tft.setTextColor(whiteColor, blackColor);
  tft.print(hdopString);

  pushButton();
  passTime();

  if (position0Saved == false && Speed > 3){
    if (timePassed == true){
      savePosition0();
      Millis0 = millis() + 2000;
    }
      
//        if (Speed > 4 && Speed < 10) {
//         Millis0 = millis() + 3000;          
//        }
//        
//        if (Speed > 10 && Speed < 20) {
//         Millis0 = millis() + 2000;          
//        }
//        
//        if (Speed > 20 && Speed < 35) {
//         Millis0 = millis() + 1000;          
//        }
//
//        if (Speed > 35 && Speed < 55) {
//         Millis0 = millis() + 500;          
//        }
  }
  
  if(buttonState == 0){
    //if (Speed < 4) Speed = 0; 
    dtostrf(Speed, 4, 1, rychlost);
    tft.setTextSize(2);
    tft.setCursor(10,10);
    tft.setTextColor(whiteColor, blackColor);
    tft.print(rychlost);
    tft.setTextSize(1);
    tft.print(" km/h");
  }


  if(buttonState == 1){
    dtostrf(totalDistance, 4, 0, vzdialenost);
    tft.setTextSize(2);
    tft.setCursor(10,10);
    tft.setTextColor(whiteColor, blackColor);
    tft.print(vzdialenost);
    tft.setTextSize(1);
    tft.print(" meter");
  }


  if(buttonState == 2){
    char failString [6];
    tft.setTextColor(whiteColor, blackColor);
    tft.setTextSize(1);
    
    dtostrf(fail, 6, 0, failString);
    tft.setCursor(10,115);
    tft.print(failString);
    
    dtostrf(lowSignal, 6, 0, failString);
    tft.setCursor(50, 115);
    tft.print(failString);
    
    dtostrf(veryLowSignal, 6, 0, failString);
    tft.setCursor(10, 125);
    tft.print(failString);
    
    dtostrf(signalLost, 6, 0, failString);
    tft.setCursor(50, 125);
    tft.print(failString); 
  }
  smartDelay(1000);
}


void savePosition0(){
  smartDelay(0);
  rawLat = gps.location.lat();
  rawLong = gps.location.lng();
  Lat0 = rawLat * 100000;
  Long0 = rawLong * 100000;
  position0Saved = true;
}

void savePosition1(){
  smartDelay(0);
  rawLat = gps.location.lat();
  rawLong = gps.location.lng();
  Lat = rawLat * 100000;
  Long = rawLong * 100000;
}

void distanceCalculating(){
    float distLat = abs(Lat0 - Lat) * 111194.9;
    float distLong = 111194.9 * abs(Long0 - Long) * cos(radians((Lat0 + Lat) / 2));
    distance = sqrt(pow(distLat, 2) + pow(distLong, 2));
    distance = distance/100000;
    position0Saved = false;
    distanceCalculated = true;
    totalDistance += distance;
    smartDelay(0);
}

void passTime(){
  smartDelay(0);
  if (Millis0 <= millis()) {
    if (position0Saved == true){
      savePosition1();
      distanceCalculating();
      timePassed = true; 
    }
  }
  else timePassed = false; 
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
