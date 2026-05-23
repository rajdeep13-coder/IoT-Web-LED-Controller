const char* ssid = "YOUR_WIFI_SSID";   //put wifi ssid, yourself AI can't do this
const char* password = "YOUR_WIFI_PASSWORD";   //put wifi ssid, yourself AI can't do this

const char* http_username = "YOUR_HTTP_USERNAME";
const char* http_password = "YOUR_HTTP_PASSWORD";
#define LED_PIN 2
// Uncomment to enable debug prints
// #define DEBUG

// Semantic version
#define VERSION "0.1.1"

#ifdef DEBUG
#define DBG_PRINT(x) Serial.println(x)
#else
#define DBG_PRINT(x) do {} while(0)
#endif