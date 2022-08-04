#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"


class Hotspot{
  private:
    const char* server_name;

    const char* ap_ssid_name;
    const char* ap_ssid_password;

    String wrong_ssid="nnn";

    String user_ssid_value,user_ssid_password;
    

  public:
    Hotspot(const char* the_server_name,const char* hotspot_ssid,const char* hotspot_password);
    bool save_credentials(const char * the_ssid, const char * the_pass);
    bool erase_credentials();
    String read_credentials();
    void setup_AP();
    bool connect_wifi();
    String get_wrong_ssid();



    
};
