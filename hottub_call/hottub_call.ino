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
#define MESSAGE_INTERVAL_S 10
// Message types
#define MSG_START 1
#define MSG_INFO 2
#define MSG_HEAT 3
#define MSG_READY 4

// NTC sensor constants
const int temp1_pin = A2;   // Heated water
const int temp2_pin = A3;   // Tub
const float invT25 = 1.00 / 298.15;
const float invBeta = 1.00 / 3435.00;
const float adcMax = 4096.00;

bool heating = false;
bool ready = false;
unsigned long startTime = 0;
unsigned long notifyTime = 0;
float heatTemp = 0;
float tubTemp = 0;


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

  //triggerIFTTT();
}


// LOOP //
void loop() {
  unsigned long currentTime = millis();
  measureHeatTemp();
  measureTubTemp();

  if(heating && !ready) {
    // Tub ready
    if (tubTemp >= 60) {
      Serial.println("tub ready");
      heating = false;
      ready = true;
      triggerIFTTT(MSG_READY);
    }

    // Message to add wood
    else if(heatTemp < 60) {
      Serial.println("add wood");
      triggerIFTTT(MSG_HEAT);
    }

    // 10 min info
    else if((currentTime - notifyTime) > (MESSAGE_INTERVAL_S * 1000)){
      Serial.println("10 min info");
      notifyTime = currentTime;
      triggerIFTTT(MSG_INFO);
    }
  }
  
  // Save start time
  else if(!heating && heatTemp > 20 && !ready) {
    Serial.println("heating started");
    startTime = currentTime;
    notifyTime = startTime;
    heating = true;
    triggerIFTTT(MSG_START);
  }

  delay(1000);
}


// FUNCTIONS //
// Measure temperature of heated water
void measureHeatTemp() {
  uint16_t temp_adc = analogRead(temp1_pin);
  // Convert to celcius
  heatTemp = (1.00 / (invT25 + invBeta * (log(adcMax / (float)temp_adc - 1.00)))) - 273.15;
  Serial.print("Heated water temp: ");
  Serial.println(heatTemp);
}

// Measure temperature of tub water
void measureTubTemp() {
  uint16_t temp_adc = analogRead(temp2_pin);
  tubTemp = 100 * temp_adc / adcMax; // 0...100 'C
  Serial.print("Tub water temp: ");
  Serial.println(tubTemp);
}

// Trigger IFTTT with message
void triggerIFTTT(int messageType) {
  WiFiClient client;
  String heatC = String(heatTemp, 2);
  String tubC = String(tubTemp, 2);
  String message = "";
  // Make message according to type
  switch (messageType) {
    case MSG_START:

    break;
    case MSG_INFO:

    break;
    case MSG_HEAT:

    break;
    case MSG_READY:

    break;
  }

  
  // Make and count POST content
  String content =  "value1=" + message + "&" +
                    "value2=" + heatC + "&" +
                    "value3=" + tubC;
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


