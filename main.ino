#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
#endif
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "config.h"

const char* hostName = "esp-led";
bool networkServicesStarted = false;

AsyncWebServer server(80);
bool requireAuth(AsyncWebServerRequest *request) {
  if (!request->authenticate(http_username, http_password)) {
    request->requestAuthentication();
    return false;
  }
  return true;
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(1000);
    DBG_PRINT("Connecting WiFi..");
  }
}

void startNetworkServices() {
  if (MDNS.begin(hostName)) {
    MDNS.addService("http", "tcp", 80);
    DBG_PRINT("mDNS ready: http://" + String(hostName) + ".local");
  } else {
    DBG_PRINT("mDNS start failed");
  }

  ArduinoOTA.setHostname(hostName);
  ArduinoOTA.begin();
  DBG_PRINT("OTA ready");
  networkServicesStarted = true;
}

void handleUpdate(AsyncWebServerRequest *request) {
  if (!request->hasParam("plain", true)) {
    request->send(400, "text/plain", "Missing JSON body");
    return;
  }

  String body = request->getParam("plain", true)->value();
  int keyIndex = body.indexOf("\"input_1\"");
  if (keyIndex < 0) {
    request->send(400, "text/plain", "Missing input_1");
    return;
  }

  int valueIndex = body.indexOf(':', keyIndex);
  if (valueIndex < 0) {
    request->send(400, "text/plain", "Invalid JSON body");
    return;
  }

  int value = body.substring(valueIndex + 1).toInt();
  value = constrain(value, 0, 1);
  digitalWrite(LED_PIN, value ? HIGH : LOW);
  DBG_PRINT("LED: " + String(value));
  request->send(200, "application/json", "{\"ok\":true}");
}

void handleStatus(AsyncWebServerRequest *request) {
  String response = "{\"pin\":" + String(LED_PIN) + ",\"state\":" + String(digitalRead(LED_PIN) ? 1 : 0) + "}";
  request->send(200, "application/json", response);
}

String outputState(){
  return digitalRead(LED_PIN) ? "checked" : "";
}

String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    return "<p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputState() + "><span class=\"slider\"></span></label></p>";
  }
  if (var == "STATE"){
    return digitalRead(LED_PIN) ? "ON" : "OFF";
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

#ifdef ESP32
  if (!LittleFS.begin(true)) {
    DBG_PRINT("LittleFS mount failed");
  }
#else
  if (!LittleFS.begin()) {
    DBG_PRINT("LittleFS mount failed");
  }
#endif
  
  for (int attempt = 0; attempt < 3 && WiFi.status() != WL_CONNECTED; attempt++) {
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      DBG_PRINT("IP: " + WiFi.localIP().toString());
      break;
    }
    if (attempt < 2) {
      DBG_PRINT("WiFi failed - retrying");
      delay(5000);
    }
  }
  if(WiFi.status() == WL_CONNECTED){
    DBG_PRINT("IP: " + WiFi.localIP().toString());
  } else {
    DBG_PRINT("WiFi failed - retrying");
    DBG_PRINT("WiFi unavailable - continuing without restart");
  }

  if (WiFi.status() == WL_CONNECTED && !networkServicesStarted) {
    startNetworkServices();
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!requireAuth(request)) {
      return;
    }
    request->send(LittleFS, "/index.html", "text/html", false, processor);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!requireAuth(request)) {
      return;
    }
    request->send(LittleFS, "/style.css", "text/css");
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!requireAuth(request)) {
      return;
    }
    handleUpdate(request);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!requireAuth(request)) {
      return;
    }
    handleStatus(request);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  server.begin();
  DBG_PRINT("Server ready!");
}

void loop() {
  static unsigned long lastReconnectAttempt = 0;

  ArduinoOTA.handle();

  if (WiFi.status() != WL_CONNECTED && millis() - lastReconnectAttempt > 30000) {
    networkServicesStarted = false;
    lastReconnectAttempt = millis();
    DBG_PRINT("WiFi disconnected - reconnecting");
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      DBG_PRINT("Reconnected: " + WiFi.localIP().toString());
      if (!networkServicesStarted) {
        startNetworkServices();
      }
    }
  }
}
