#include <StringSplitter.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Wire.h>

const char* ssid = "Chozhan's hotspot";
const char* password = "Ch0zh@n0n02";

const char* STA_ssid = "IRobotix_AP";
const char* STA_password = "rifanSifan1";

String message = " ";
String fan = "";
String light = "";

uint8_t relay1 = D1;
uint8_t relay2 = D2;
uint8_t relay3 = D3;
uint8_t relay4 = D4;

ESP8266WebServer server(80);

const char PAGE_Home_automation[] PROGMEM = R"=====(
<html>  
<body>
<h1>HOME AUTOMATION</h1>
<form name "myForm" action="http://192.168.4.1/submit" method="GET">
<h2>SELECTION </h2>
  <h2>Fan</h2>
  <input type="radio" name="fan" value="OFF" checked> OFF<br>
  <input type="radio" name="fan" value="ON"> ON<br>
  <h2>Light</h2>
    <input type="radio" name="light" value="OFF" checked> OFF<br>
  <input type="radio" name="light" value="ON"> ON<br><br>
    <input type="submit" value="Submit">
</form>
</body>
</html>
)=====";

const char PAGE_go_back[] PROGMEM = R"=====(
<html>  
<body>
<h1>HOME AUTOMATION</h1>
<form name "myForm" action="http://192.168.4.1/submit" method="GET">
<h2>SELECTION </h2>
  <h2>Fan</h2>
  <input type="radio" name="fan" value="OFF" checked> OFF<br>
  <input type="radio" name="fan" value="ON"> ON<br>
  <h2>Light</h2>
    <input type="radio" name="light" value="OFF" checked> OFF<br>
  <input type="radio" name="light" value="ON"> ON<br><br>
    <input type="submit" value="Submit">
</form>
</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/plain", "HOT AXIAL TEMPERATURE MONITOR!");
}

void mainpage() {
  String home_automation = PAGE_Home_automation;
  server.send (200, "text/html", home_automation );
}

void donePage() {
  String page = PAGE_go_back;
  server.send (200, "text/html", page );
}

void home_auto(){
    if (fan == "ON"){
    digitalWrite(relay1,HIGH);
    Serial.println("FAN: ON");
    }
  if (fan == "OFF"){
    digitalWrite(relay1,LOW);
    Serial.println("FAN: OFF");
    }
  if (light == "ON"){
    digitalWrite(relay2,HIGH);
    Serial.println("LIGHT: ON");
    }
  if (light == "OFF"){
    digitalWrite(relay2,LOW);
    Serial.println("LIGHT: OFF");
    }
}

void stringSplit(String web_configuration) {
  StringSplitter *splitter = new StringSplitter(web_configuration, '\n', 2);
  fan = splitter->getItemAtIndex(0);
  light = splitter->getItemAtIndex(1);  
  light.trim();
  home_auto(); 
}

void submit() {
  donePage();
  message = "";
  Serial.println(".......>");
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      //message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
      message += server.arg(i) + "\n";
//        Serial.println(".......>");
//      Serial.print(message);
    }
   stringSplit(message);
  }
}

void hotspot_mode() {
    Serial.println("Hotspot is switching ON");
    WiFi.mode(WIFI_AP_STA); 
    WiFi.softAP(ssid, password);  
    IPAddress myIP = WiFi.softAPIP(); 
    Serial.print("HotSpt IP:");
    Serial.println(myIP);
    delay(100);    
    server.on("/", mainpage);    
    server.on("/submit", submit); 
    server.begin();             
    Serial.println("HTTP server started");
}

void setup(){
    Serial.begin(9600);
    EEPROM.begin(512);   
    Serial.println("setup is running");
    hotspot_mode();
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);   
    pinMode(relay3, OUTPUT);
    pinMode(relay4, OUTPUT);
}

void loop(){
  server.handleClient(); 
//  Serial.println(".......>>>");       
//  Serial.println(light);
//  Serial.println(fan);
}
