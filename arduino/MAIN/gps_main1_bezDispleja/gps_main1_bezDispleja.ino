#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

static const int RXPin = 7, TXPin = 6;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss (RXPin, TXPin);

// char array for printing on the screen
int buttonValue = 0;
float Speed = 0;
int Satelites = 0;
float Hdop;
float rawLat;
float rawLong;
unsigned int Fail;
unsigned int lowSignal = 0;
unsigned int veryLowSignal = 0;
unsigned int signalLost = 0;
unsigned long Millis0 = 0;
bool position0Saved = false;
bool distanceCalculated = false;
bool timePassed = true;
float totalDistance = 0;
char convertString[10];
float Lat0;
float Long0;
long Lat;
long Long;
float distance; 
float distanceInKM;
bool changedToKM = false;
char filename[16]; 
char SD_date_time[16]; 
int Year;
byte Month;
byte Day;


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
      if (totalDistance < 1000) totalDistance += distance;
      
      else if (totalDistance >= 1000) {
        if (changedToKM == false) {
          totalDistance = totalDistance / 1000;
          changedToKM == true;
        }
        distance = distance / 1000.0;
        totalDistance += distance; 
      }
      
      smartDelay(0);
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
File root;

void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  root = SD.open("suradnic.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (root) {
    Serial.print("Writing to test1.txt...");
    root.println("testing 1, 2, 3.");
    // close the file:
    root.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  pinMode(2, INPUT);
  pinMode(A4, INPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  root = SD.open("/");

  printDirectory(root, 0);

  Serial.println("done!");

  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DistanceClass Distance;

void loop() {  
//  file = SD.open("test.txt", FILE_WRITE);
//
//  // if the file opened okay, write to it:
//  if (file) {
//    Serial.print("Writing to test.txt...");
//    file.println("testing 1, 2, 3.");
//    // close the file:
//    file.close();
//    Serial.println("done");
//  }
  
  smartDelay(0); 
  //signalStrength();

  Hdop = gps.hdop.hdop();
  Speed = gps.speed.kmph();
  
  if (Hdop < 50){  
    passTime();
  
    if (position0Saved == false && Speed > 1){
      if (timePassed == true){
        Distance.savePosition0();
        Serial.println("position 0 saved");
        Millis0 = millis() + 2000;
      }
        
  //    if (Speed > 4 && Speed < 10) {
  //     Millis0 = millis() + 3000;          
  //    }
  //    
  //    if (Speed > 10 && Speed < 20) {
  //     Millis0 = millis() + 2000;          
  //    }
  //    
  //    if (Speed > 20 && Speed < 35) {
  //     Millis0 = millis() + 1000;          
  //    }
  //
  //    if (Speed > 35 && Speed < 55) {
  //     Millis0 = millis() + 500;          
  //    }
    }
  }
  next();
  
  switch (buttonValue) {
    //printing speed
    case 0:
      //if (Speed < 4 || Hdop > 12) Speed = 0; 
      Serial.print(Speed);
      Serial.print(" km/h");
      Serial.print("\t");
      Serial.println(Hdop);
      break;

    //printing distance
    case 1:
      if (totalDistance < 1000) {
        Serial.print(totalDistance);
        Serial.print(" m");  
      }
      else if (totalDistance >= 1000) {
        Serial.print(totalDistance);
        Serial.print(" m"); 
      }
      Serial.print("\t");
      Serial.println(Hdop);
      break;


    //printing signal strength
//    case 2:
//      Fail = gps.failedChecksum();
//      Print.Settings(1, 10, 115);
//      Print.Value(Fail, 4, 0);
//      
//      Print.Settings(1, 50, 115);
//      Print.Value(lowSignal, 5, 0);
//      
//      Print.Settings(1, 10, 125);
//      Print.Value(veryLowSignal, 5, 0);
//  
//      Print.Settings(1, 50, 125);
//      Print.Value(signalLost, 4, 0);
//      break;
    } 
  smartDelay(1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void passTime(){
  DistanceClass Distance;
  Serial.println("pass time function");
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


void next(){
  if (digitalRead(2) == HIGH) {
    buttonValue++;
    if (buttonValue > 1) buttonValue = 0;
  }
}
//.....................................................................................................................................................................

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
