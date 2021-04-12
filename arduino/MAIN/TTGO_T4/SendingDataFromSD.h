#include <WiFi.h>
#include <WiFiClient.h>
#include "ESP32FtpServer.h"

#include <SPI.h>
#include <SD.h>

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13

SPIClass sdSPI(VSPI);

FtpServer ftpSrv;   //set #define FTP_DEBUG in ESP32FtpServer.h to see ftp verbose on serial

#define FTP_DEBUG
//SD card options
/*
#define SD_CS 5
#define SDSPEED 40000000
*/

IPAddress apIP(192, 168, 12,7);  
IPAddress gateway(192, 168,12, 7);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "GPStracker";
const char* password = "mikkoflosso";


void setup(void){
  Serial.begin(115200);
  Serial.println("");
  //Setting the Ip address for the WiFi connectivity
  WiFi.config(apIP, gateway, subnet);
  Serial.println();
  Serial.print("Configuring access point...");
  //Setting the esp to softaccess point mode
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  //changing softAP config and starting the Start AP
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(ssid, password);

  /////FTP Setup, ensure SD is started before ftp;  /////////
  
  ////if (SD.begin( SD_CS, SPI, SDSPEED)) {
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
    if (SD.begin(SD_CS, sdSPI)) {
      Serial.println("SD opened!");
      ftpSrv.begin("esp32","123456");    //username, password for ftp.  set ports in ESP32FtpServer.h  (default 21, 50009 for PASV)   
    }
}


void loop(void){
  ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   

}
