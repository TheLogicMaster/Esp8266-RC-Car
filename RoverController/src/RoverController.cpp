/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h>

const char* ssid     = "Super Cool Rover";
const char* password = "123456789";

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

FSInfo fs_info;

char bufferTemp[300];

void ShowInfo(void){
 
  SPIFFS.info(fs_info);
  sprintf(bufferTemp,  "Total Bytes:     %d", fs_info.totalBytes);
  Serial.println(bufferTemp);
  sprintf(bufferTemp,  "Used Bytes:      %d", fs_info.usedBytes);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Block Size:      %d", fs_info.blockSize);
  Serial.println(bufferTemp);   
  sprintf(bufferTemp,  "Page Size:       %d", fs_info.pageSize);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Max open files:  %d", fs_info.maxOpenFiles);
  Serial.println(bufferTemp); 
  sprintf(bufferTemp,  "Max path lenght: %d", fs_info.maxPathLength);
  Serial.println(bufferTemp);
  Serial.println("");
}

void ShowDIR(void){

    Serial.println("\n--------------------------------");
    Serial.println("Directory:");
    Dir dir = SPIFFS.openDir("");
 
  while (dir.next()) {
    Serial.print(dir.fileName());
    File f = dir.openFile("r");
    Serial.print("\t\t");
    Serial.println(f.size());
  }
  Serial.println("--------------------------------");
  Serial.println("");
}

void setup(){
  pinMode(D5, OUTPUT); 
  pinMode(D6, OUTPUT); 
  pinMode(D7, OUTPUT); 
  pinMode(D8, OUTPUT); 
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  //WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
  SPIFFS.begin(); 
  ShowInfo();
  ShowDIR();
  // Route for root / web page
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on("/data/left", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println(request->params());
    Serial.println(request->getParam(0)->value().toFloat());
    Serial.println(request->getParam(1)->value().toFloat());
    if (request->getParam(1)->value().toInt() == 0) {
        analogWrite(D5, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D6, 0);
      } else {
        analogWrite(D6, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D5, 0);
      }
    request->send_P(200, "text/plain", String("Recieved").c_str());
  });
  server.on("/data/right", HTTP_POST, [](AsyncWebServerRequest *request){
      if (request->getParam(1)->value().toInt() == 0) {
        analogWrite(D7, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D8, 0);
      } else {
        analogWrite(D8, request->getParam(0)->value().toFloat() / 100.0 * 1023.0);
        analogWrite(D7, 0);
      }
    request->send_P(200, "text/plain", String("Recieved").c_str());
  });
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });*/

 server.onNotFound([](AsyncWebServerRequest *request){
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404);
  }
});

  // Start server
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.begin();

  /*if (MDNS.begin("esp8266")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("MDNS responder started");
    Serial.print("You can now connect to http://");
    Serial.print("esp8266");
    Serial.println(".local");
  } else {
    Serial.println("MDNS responder failed to start");
  }*/
}
 
void loop(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
      
  }
}