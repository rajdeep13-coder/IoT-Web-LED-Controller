#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "config.h"

const int output = 2;
AsyncWebServer server(80);
void handleUpdate(AsyncWebServerRequest *request) {
  if(!request->authenticate(http_username, http_password))
    return request->requestAuthentication();

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
  digitalWrite(output, value ? HIGH : LOW);
  Serial.println("LED: " + String(value));
  request->send(200, "application/json", "{\"ok\":true}");
}

String outputState(){
  return digitalRead(output) ? "checked" : "";
}

String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    return "<p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputState() + "><span class=\"slider\"></span></label></p>";
  }
  if (var == "STATE"){
    return digitalRead(output) ? "ON" : "OFF";
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  
  WiFi.begin(ssid, password);
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(1000);
    Serial.println("Connecting WiFi..");
  }
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("WiFi failed - restart");
    ESP.restart();
  }
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_POST, handleUpdate);

  server.begin();
  Serial.println("Server ready!");
}

void loop() {}
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
