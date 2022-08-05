#include "Requests.h"

http_response Requests::return_response(bool which){
    //if which is 1(true) it means they want the get request
    //if which is 2(false) it means thet want the post request
    if (which==true) return get_request;
    return post_request;
}

json_content Requests::return_content(){
    return json_as_string;
}

void Requests::display_keys_and_values(){
  for(int i=0;i<sizeof(json_as_string.keys)/sizeof(String);i++){
    if (json_as_string.keys[i]=="") continue;
    Serial.print("Key: ");
    Serial.println(json_as_string.keys[i]);
    Serial.println("");
    Serial.print("Value: ");
    Serial.println(json_as_string.values[i]);
    Serial.println("");
  }
}

void Requests::update_keys_and_values(String req){
  DynamicJsonDocument doc(100);
  deserializeJson(doc, req);
  JsonObject root = doc.as<JsonObject>();
    

  byte c = 0;
  for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
    json_as_string.keys[c]=it->key().c_str();
    json_as_string.values[c]=it->value().as<const char*>();
    c+=1;
  }


}


void Requests::httpGETRequest(const char* serverName){
  HTTPClient http;
    
  http.begin(serverName);
  
  http.setConnectTimeout(10000);
  httpResponseCode = http.GET();
  
  payload = "{}"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }

  // Free resources
  http.end();

  get_request.resp_content=payload;
  get_request.status_code=httpResponseCode;

}

void Requests::HttpPOSTRequest(const char* serverName,const char* key,const char* value){

  HTTPClient http;
  
  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  
  // Send HTTP GET request
  http.setConnectTimeout(10000); 

  StaticJsonDocument<100> doc;
  JsonObject obj = doc.to<JsonObject>();

  obj[key] = value;
     
  requestBody="";
  serializeJson(obj, requestBody);
  httpResponseCode = http.POST(requestBody);
  payload = "{}"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }

  //Free resources
  http.end();

  post_request.resp_content=payload;
  post_request.status_code=httpResponseCode;

}