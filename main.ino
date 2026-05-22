#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "config.h"

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
  
  for (int attempt = 0; attempt < 3 && WiFi.status() != WL_CONNECTED; attempt++) {
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("IP: " + WiFi.localIP().toString());
      break;
    }
    if (attempt < 2) {
      Serial.println("WiFi failed - retrying");
      delay(5000);
    }
  }
  if(WiFi.status() == WL_CONNECTED){
    DBG_PRINT("IP: " + WiFi.localIP().toString());
  } else {
    DBG_PRINT("WiFi failed - retrying");
    if (!requireAuth(request)) {
      return;
    }
    DBG_PRINT("WiFi unavailable - continuing without restart");
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
    if (!requireAuth(request)) {
      return;
    }
    handleUpdate(request);
  });

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not found");
  });

  server.begin();
  DBG_PRINT("Server ready!");
}

void loop() {
  static unsigned long lastReconnectAttempt = 0;

  if (WiFi.status() != WL_CONNECTED && millis() - lastReconnectAttempt > 30000) {
    lastReconnectAttempt = millis();
    DBG_PRINT("WiFi disconnected - reconnecting");
    connectWiFi();
    if (WiFi.status() == WL_CONNECTED) {
      DBG_PRINT("Reconnected: " + WiFi.localIP().toString());
    }
  }
}
//html file
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP IoT Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>body{font-family:Arial;text-align:center;margin:0px auto;padding-top:30px;}
  .switch{position:relative;display:inline-block;width:120px;height:68px;}.switch input{display:none;}
  .slider{position:absolute;top:0;left:0;right:0;bottom:0;background-color:#ccc;border-radius:34px}
  .slider:before{position:absolute;content:"";height:52px;width:52px;left:8px;bottom:8px;background-color:white;transition:.4s;border-radius:68px}
  input:checked+.slider{background-color:#2196F3}input:checked+.slider:before{transform:translateX(52px)}</style>
</head>
<body><h1>ESP IoT LED</h1><h3>Pin 2: <span id="state"><STATE></span></h3><BUTTONPLACEHOLDER>
<script>function toggleCheckbox(x){var xhr=new XMLHttpRequest();xhr.open("POST","/update",true);xhr.setRequestHeader("Content-Type","application/json");xhr.send(JSON.stringify({input_1:x.checked?1:0}));document.getElementById("state").innerText=x.checked?"ON":"OFF";}</script>
</body></html>)rawliteral";
