#include "Access_Point.h"


AsyncWebServer server(80);

Hotspot::Hotspot(const char* the_server_name,const char* hotspot_ssid,
                 const char* hotspot_password){
  
  server_name=the_server_name;
  ap_ssid_name=hotspot_ssid;
  ap_ssid_password=hotspot_password;
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File creds_file = SPIFFS.open("/wifi_creds.txt",FILE_READ);
  if(!creds_file){
      Serial.println("Failed to open file");
      return;
  }

  creds_file.close();
  
}



bool Hotspot::erase_credentials(){
    File creds_file = SPIFFS.open("/wifi_creds.txt",FILE_WRITE);
 
    if(!creds_file){
        Serial.println("Failed to open file for reading");
        return false;
    }

    creds_file.print("");
 
    creds_file.close();
    return true;
}



String Hotspot::read_credentials(){
    File creds_file = SPIFFS.open("/wifi_creds.txt");
 
    if(!creds_file){
        Serial.println("Failed to open file for reading");
        return wrong_ssid;
    }

    String fileContent="";
    while(creds_file.available()){
        fileContent+=String((char)creds_file.read());
    }
    creds_file.close();
    return fileContent;
    
}

bool Hotspot::save_credentials(const char * the_ssid, const char * the_pass){
    File creds_file = SPIFFS.open("/wifi_creds.txt",FILE_WRITE);
 
    if(!creds_file){
        Serial.println("Failed to open file for reading");
        return false;
    }

    if (the_ssid==wrong_ssid.c_str() || the_pass==wrong_ssid.c_str()){
        Serial.println("We have a problem. SSID or password are blank");
        return false;
    }

    creds_file.printf("%s:%s",the_ssid,the_pass);

    Serial.printf("Wrote to creds file: user - %s and pass - %s\n",the_ssid,the_pass);
 
    creds_file.close();
    return true;
    
}

bool Hotspot::connect_wifi(){
    WiFi.mode(WIFI_STA);

    String creds = read_credentials();
    
    String WIFI_SSID = creds.substring(0,creds.indexOf(":"));
    String WIFI_PASS = creds.substring(creds.indexOf(":")+1,creds.length());


    Serial.print("WIFI: ");
    Serial.println(WIFI_SSID);
    Serial.print("PASS: ");
    Serial.println(WIFI_PASS);
    
    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID.c_str());
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    
    // Wait
    char c = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("... ");
        delay(400);
        c+=1;
        if (c>20){
            Serial.println("\nCouldn't connect to WIFI.");
            return false;
        }
    }

    Serial.println("\nConnected");
    Serial.printf("\n[WIFI] STATION Mode, SSID: %s\nIP address: %s, MAC address: %s\n", WiFi.SSID().c_str(), 
    WiFi.localIP().toString().c_str(),WiFi.macAddress().c_str());
  
    return true;
}

String Hotspot::get_wrong_ssid(){
  return this->wrong_ssid;
}

void Hotspot::setup_AP(){
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting up hotspot...\n");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ap_ssid_name, ap_ssid_password);


  if(!MDNS.begin(server_name)) {
     Serial.println("Error starting mDNS");
     return;
  }


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/credentials.html", HTTP_POST, [&](AsyncWebServerRequest *request){

    int params = request->params();
    for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
            // HTTP POST ssid value
            if (p->name() == "SSID") {
                user_ssid_value = p->value().c_str();
                Serial.print("SSID set to: ");
                Serial.println(user_ssid_value);
          
            }
            if (p->name() == "password") {
                user_ssid_password = p->value().c_str();
                Serial.print("SSID set to: ");
                Serial.println(user_ssid_password);
          
            } 
        }
    }
    
    save_credentials(user_ssid_value.c_str(),user_ssid_password.c_str());         

//    if (request->getParam("SSID")->value() != "") {
//        ssid_value = request->getParam("SSID")->value();
//    } else {
//        ssid_value = wrong_ssid;
//    }
//
//    if (request->getParam("password")->value() != "") {
//        ssid_password = request->getParam("password")->value();
//    } else {
//        ssid_password = wrong_ssid;
//    }


    const char * msg = "Done. ESP will restart and automatically connect to your WIFI network. If it can't, the SSID "
                       "and password will have to be provided again.";
    request->send(200, "text/plain", msg);
    
    //request->send(SPIFFS, "/credentials.html", "text/html");

    delay(500);
    ESP.restart();
    
  });

  
  server.begin();
}
