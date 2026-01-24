#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

const char* ssid = "XXXXXXXX";   // Replace with your network SSID
const char* password = "69696969"; // Replace with your network password

const char* http_username = "admin";
const char* http_password = "admin";

const int output = 2;
AsyncWebServer server(80);
void handleUpdate(AsyncWebServerRequest *request) {
  if(!request->authenticate(http_username, http_password))
    return request->requestAuthentication();
    
  if (request->hasParam("input_1")) {
    int value = request->getParam("input_1")->value().toInt();
    digitalWrite(output, value);
    Serial.println("LED: " + String(value));
  }
  request->send(200, "text/plain", "OK");
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

  server.on("/update", HTTP_GET, handleUpdate);

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
<script>function toggleCheckbox(x){var xhr=new XMLHttpRequest();xhr.open("GET","/update?input_1="+(x.checked?1:0),true);xhr.send();document.getElementById("state").innerText=x.checked?"ON":"OFF";}</script>
</body></html>)rawliteral";
