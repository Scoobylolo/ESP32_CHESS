#include <HTTPClient.h>
#include <ArduinoJson.h>


struct http_response{
    int status_code;
    String resp_content;
};

struct json_content{
    String keys[10];
    String values[10];
};

class Requests{
    private:
        String payload,requestBody;
        byte httpResponseCode; 

        http_response get_request;
        http_response post_request;
        json_content json_as_string;

    public:
        void httpGETRequest(const char* serverName);
        void HttpPOSTRequest(const char* serverName,const char* key,const char* value);
        void update_keys_and_values(String req);
        void display_keys_and_values();
        http_response return_response(bool which);
        json_content return_content();
};

