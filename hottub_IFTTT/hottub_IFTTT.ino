// Home Automation - Hot tub
// Eemeli Halme & Leevi Kinnunen
// 2023
// based on https://randomnerdtutorials.com/esp32-door-status-monitor-email/

#include <WiFi.h>

#define WIFI_SSID "S23U"
#define WIFI_PASSWORD "98709870"
#define HOST "maker.ifttt.com"
#define API_KEY "REPLACE_WITH_YOUR_IFTTT_API_KEY"

// NTC sensor constants
const int temp1_pin = A2;
const float invBeta = 1.00 / 3435.00;
const float adcMax = 4096.00;
const float invT25 = 1.00 / 298.15;


// Measure temperature of heated water
float measureHeatTemp() {
  uint16_t temp_adc = analogRead(temp1_pin);
  // Convert to celcius
  float temp_c = (1.00 / (invT25 + invBeta * (log(adcMax / (float)temp_adc - 1.00)))) - 273.15;
  Serial.print("Heated water temp: ");
  Serial.println(temp_c);
  return temp_c;
}

// Measure temperature of tub water
float measureTubTemp() {
}


// SETUP //
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 0-4096

  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}


// LOOP //
void loop() {
}


// FUNCTIONS //
void trigger_IFTTT() {
  String message;
  const int httpPort = 80;
  WiFiClient client;
  
  Serial.print("connecting to ");
  Serial.println(HOST);
  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/trigger/event_name/with/key/"; // replace event_name with own!!!
  url += API_KEY;

  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                      "Host: " + HOST + "\r\n" +
                      "Content-Type: application/x-www-form-urlencoded\r\n" +
                      "Content-Length: 13\r\n\r\n" +
                      "value1=" + message + "\r\n");
  
}
