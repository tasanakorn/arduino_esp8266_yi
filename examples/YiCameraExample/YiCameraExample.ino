#include <Yi.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>



const char* ssid     = "TAS_YI";
const char* password = "1234567890";

const char* host = "192.168.42.1";

WiFiClient client;
int client_status = 0;
int token = 0;
YiClient yi(client, host);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  

  Serial.print("Connecting camera ");
  yi.begin();
  while(!yi.logon()) {
    yi.loop();
    Serial.print(".");
  }
  Serial.println("");
}

void loop() {  
  if (Serial.available()) {
    int c = Serial.read();
    if (c == 'p') {
        yi.takePhoto();      
    }
    if (c == 's') {
        yi.startRecording();
    }
    if (c == 't') {
        yi.stopRecording();
    }
  }
  yi.loop();
  delay(100);
}

