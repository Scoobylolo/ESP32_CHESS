#include "Access_Point.h"
#include <HTTPClient.h>
// #include <Arduino_JSON.h>
#include <ArduinoJson.h>
#include "api_endpoints.h"

Hotspot Chess_Hotspot("chessy","Chess Board","lola123");
bool wifi_connected=false;

const long interval = 60000;
long last_time = 0;

String httpGETRequest(const char* serverName);
String HttpPOSTRequest(const char* serverName,const char* key,const char* value);

void setup() {
  Serial.begin(115200);

  String contents = Chess_Hotspot.read_credentials();
  Serial.print("Contents: ");
  Serial.println(contents);
  if (contents==Chess_Hotspot.get_wrong_ssid() || contents==""){
    Serial.println("Now setting up the server");
    Chess_Hotspot.setup_AP();   
  }
  else{
    Serial.println("Now we have credentials");
    wifi_connected=Chess_Hotspot.connect_wifi();
    if (!wifi_connected){
        Chess_Hotspot.erase_credentials();
        Serial.println("\nErased credentials. Restarting ESP...");
        ESP.restart();
    }

  }

}

void loop(){
    if ((millis() - last_time > interval) && (WiFi.status() == WL_CONNECTED)){


      char my_buffer[100];
      sprintf(my_buffer,"%s%s",API_URL,IS_MOVE_LEGAL);
      String example = httpGETRequest(my_buffer);
      Serial.println(example);
      // JSONVar myObject = JSON.parse(example);
  
      // // JSON.typeof(jsonVar) can be used to get the type of the var
      // if (JSON.typeof(myObject) == "undefined") {
      //   Serial.println("Parsing input failed!");
      //   return;
      // }
    
      // Serial.print("JSON object = ");
      // Serial.println(myObject);
    
      // // myObject.keys() can be used to get an array of all the keys in the object
      // JSONVar keys = myObject.keys();
      // bool my_arr[10];
      // for (int i = 0; i < keys.length(); i++) {
      //   JSONVar value = myObject[keys[i]];
      //   Serial.print(keys[i]);
      //   Serial.print(" = ");
      //   Serial.println(value);
      //   my_arr[i] = (value);
      // }
      // for (int i=0;i<sizeof(my_arr)/sizeof(bool);i++){
      //   Serial.printf("%d = %d\n\n",i,my_arr[i]);
      // }

      char my_buffer2[100];
      sprintf(my_buffer2,"%s%s",API_URL,_KING_POSITION);
      String other = HttpPOSTRequest(my_buffer2,"side","white");
      Serial.println(other);



    last_time=millis();





    }
}


String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  Serial.print("Server name: ");
  Serial.println(serverName);
  http.begin(serverName);
  
  // Send HTTP GET request
  http.setConnectTimeout(10000);
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

String HttpPOSTRequest(const char* serverName,const char* key,const char* value){
  HTTPClient http;
  

  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  
  // Send HTTP GET request
  http.setConnectTimeout(10000); 

  StaticJsonDocument<200> doc;
  JsonObject obj = doc.to<JsonObject>();

  obj[key] = value;
     
  String requestBody;
  serializeJson(obj, requestBody);
  int httpResponseCode = http.POST(requestBody);
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
