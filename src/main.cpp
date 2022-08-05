#include "api_endpoints.h"
#include "Access_Point.h"
#include "Requests.h"


Hotspot Chess_Hotspot("chessy","Chess Board","lola123");
Requests Custom_Requests;
bool wifi_connected=false;

const long interval = 15000;
long last_time = 0;



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


      char get_buffer[100];
      sprintf(get_buffer,"%s%s",API_URL,IS_MOVE_LEGAL);
      Custom_Requests.httpGETRequest(get_buffer);
      Serial.println(Custom_Requests.return_response(true).resp_content);
      Custom_Requests.update_keys_and_values(Custom_Requests.return_response(true).resp_content);
      Custom_Requests.display_keys_and_values();


      char post_buffer[100];
      sprintf(post_buffer,"%s%s",API_URL,_KING_POSITION);
      Custom_Requests.HttpPOSTRequest(post_buffer,"side","white");
      Serial.println(Custom_Requests.return_response(false).resp_content);
      Custom_Requests.update_keys_and_values(Custom_Requests.return_response(false).resp_content);
      Custom_Requests.display_keys_and_values();
      Serial.println("");



    last_time=millis();



    }
}


