#include <ESP8266WiFi.h>
#include <SD.h>
#include "ESP8266FtpServer.h"

// config static IP
IPAddress apIP(192, 168, 12,7);  
IPAddress gateway(192, 168,12, 7);
IPAddress subnet(255, 255, 255, 0); 

FtpServer ftpSrv;//set #define FTP_DEBUG in ESP8266FtpServer.h to see ftp verbose on serial

class FTPClass {
  private:
    const char* ssid = "SDcard Reader";
    const char* password = "ASDF@123";
  public:
    void FTPsetup(void){
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
    
      if (SD.begin(D8)) {
          Serial.println("SD opened!");
          ftpSrv.begin("esp8266","esp8266");    //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
      }   
    }

    
    void FTPloop(void){
      ftpSrv.handleFTP();        //make sure in loop you call handleFTP()!!   
    }
};
