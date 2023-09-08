// Home Automation - Hot tub
// Eemeli Halme & Leevi Kinnunen
// 2023
// based on https://randomnerdtutorials.com/esp32-door-status-monitor-email/

#include <WiFi.h>

#define WIFI_SSID "eme"
#define WIFI_PASSWORD "sanasala"
#define HOST "maker.ifttt.com"
#define API_KEY "dfPt1cD9pYzS4k9xeypNCz"

// NTC sensor constants
const int temp1_pin = A2;   // Heated water
const int temp2_pin = A3;   // Tub
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
  uint16_t temp_adc = analogRead(temp2_pin);
  float temp_c = 100 * temp_adc / adcMax; // 0...100 'C
  Serial.print("Tub water temp: ");
  Serial.println(temp_c);
  return temp_c;
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
  String message = "test text"; // Change according to message type!
  String heatTemp = String(measureHeatTemp(), 2);
  String tubTemp = String(measureTubTemp(), 2);
  const int httpPort = 80;
  WiFiClient client;
  
  Serial.print("connecting to ");
  Serial.println(HOST);
  if (!client.connect(HOST, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // template: https://maker.ifttt.com/trigger/{event}/with/key/{webhooks_key}?value1=value1&value2=value2&value3=value3
  String url = "/trigger/hot_tub/with/key/";
  url += API_KEY;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                      "Host: " + HOST + "\r\n" +
                      "Content-Type: application/x-www-form-urlencoded\r\n" +
                      "Content-Length: 13\r\n\r\n" +
                      "value1=" + message + "&" +
                      "value2=" + heatTemp + "&" +
                      "value3=" + tubTemp + "&" + "\r\n");
}


