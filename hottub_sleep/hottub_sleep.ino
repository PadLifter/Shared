// Home Automation - Hot tub
// Eemeli Halme & Leevi Kinnunen
// 2023
// IFTTT: https://randomnerdtutorials.com/esp32-door-status-monitor-email/
// NTP: https://randomnerdtutorials.com/esp32-date-time-ntp-client-server-arduino/

#include <WiFi.h>
#include <time.h>

#define WIFI_SSID "eme"
#define WIFI_PASSWORD "sanasala"
#define CONNECTION_TIMEOUT 10
#define HOST "maker.ifttt.com"
#define API_KEY "dfPt1cD9pYzS4k9xeypNCz"
#define HTTPPORT 80
#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60        // Time ESP32 will go to sleep (in seconds)

RTC_DATA_ATTR int bootCount = 0;

// NTC sensor constants
const int temp1_pin = A2;   // Heated water
const int temp2_pin = A3;   // Tub
const float invT25 = 1.00 / 298.15;
const float invBeta = 1.00 / 3435.00;
const float adcMax = 4096.00;


// SETUP //
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 0-4096

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  int timeout_counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
    timeout_counter++;
    if(timeout_counter >= CONNECTION_TIMEOUT * 5) {
      Serial.println();
      Serial.println("Failed to connect, restarting...");
      ESP.restart();
    }
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  triggerIFTTT();
}


// LOOP //
void loop() {
  bool heating;
  bool ready;
  unsigned long startTime;
  float heatTemp = measureHeatTemp();
  float tubTemp = measureTubTemp();

  // Save start time
  if(heatTemp > 15) {
    startTime = millis();
    bool heating = true;    
  }

  // Message to add wood
  if(heating && heatTemp < 60) {
    //triggerIFTTT(addwood);
  }

  // Tub ready
  if (tubTemp >= 60) {
    heating = false;
    //triggerIFTTT(ready);
}
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");
  
  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();    
  
}


// FUNCTIONS //
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

// Trigger IFTTT with message
void triggerIFTTT() {
  WiFiClient client;
  String heatTemp = String(measureHeatTemp(), 2);
  String tubTemp = String(measureTubTemp(), 2);
  String message = "test_text"; // Change according to message type!
  
  // Make and count POST content
  String content =  "value1=" + message + "&" +
                    "value2=" + heatTemp + "&" +
                    "value3=" + tubTemp;
  String length = (String)content.length();

  // POST data
  Serial.print("connecting to ");
  Serial.println(HOST);
  if (!client.connect(HOST, HTTPPORT)) {
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
                      "Content-Length: " + length + "\r\n\r\n" +
                      content + "\r\n");
  /*
  Serial.print(String("POST ") + url + " HTTP/1.1\r\n" +
                      "Host: " + HOST + "\r\n" +
                      "Content-Type: application/x-www-form-urlencoded\r\n" +
                      "Content-Length: " + length + "\r\n\r\n" +
                      content + "\r\n");
  */
}


