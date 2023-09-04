// Home Automation - Hot tub
// Eemeli Halme & Leevi Kinnunen
// 2023 

const int temp1_pin = A0;
//NTC sensor constants
const float invBeta = 1.00 / 3435.00;
const float adcMax = 4096.00;
const float invT0 = 1.00 / 298.15;

// Send info about heating
void sendInfo() {
}

// Send reminder to add wood
void sendReminder() {
}

// Notify that hot tub is ready
void sendReady() {
}

// Measure temperature of heated water
float measureHeatTemp() {
  uint16_t temp_adc = analogRead(temp1_pin);
  // Convert to celcius
  float temp_c = (1.00 / (invT0 + invBeta*(log ( adcMax / (float) temp_adc - 1.00))))- 273.15;

  Serial.print("Heated water temp: ");
  Serial.println(temp_c);
  return temp_c;
}

// Measure temperature of tub water
float measureTubTemp() {
}


void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 0...4096
}


void loop() {
  //Serial.printf("ADC analog value = %d\n", measureHeatTemp());
  measureHeatTemp();

  delay(1000);  // delay in between reads for clear read from serial
}
