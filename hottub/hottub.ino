// Home Automation - Hot tub
// Eemeli Halme & Leevi Kinnunen
// 2023

const int temp1_pin = A0;


// Send info about heating
void sendInfo(){

}

// Send reminder to add wood
void sendReminder(){

}

// Notify that hot tub is ready
void sendReady(){

}


void setup() {
  Serial.begin(115200);
  analogReadResolution(12);   // 0-4096
}


void loop() {
  // read the analog / millivolts value for pin 2:
  int analogValue = analogRead(temp1_pin);
  int analogVolts = analogReadMilliVolts(temp1_pin);
  
  // print out the values you read:
  Serial.printf("ADC analog value = %d\n",analogValue);
  Serial.printf("ADC millivolts value = %d\n",analogVolts);
  
  delay(1000);  // delay in between reads for clear read from serial
}

