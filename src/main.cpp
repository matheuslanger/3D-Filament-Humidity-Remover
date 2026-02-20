/**********************************************************************
  Filename    : main
  Description : 3D Filament Humidity Dryer.
  Auther      : Matheus Araujo Langer
  Modification: 12/08/2024
**********************************************************************/
#include <Arduino.h>
#include <global_vars.h>
#include <functions.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

void setup() {
  Serial.begin(9600);
  pinMode(PWM_CONTROL, INPUT);

  lcdSetup();
  encoderSetup();
  dhtSetup();
}

void loop() {
  handleMainMenu();
  delay(500);
  Serial.println(analogRead(PWM_CONTROL));
  // float humi = dht_sensor.readHumidity();
  // float tempC = dht_sensor.readTemperature();

  // Serial.print(termValue(TERM1));
  // Serial.print("\t");
  // Serial.println(termValue(TERM2));

  //  if (isnan(tempC) || isnan(humi))
  //  {
  //     Serial.println("ERROR");
  //  }
  //  else
  //  {
  //     Serial.println(tempC);
  //     Serial.println(humi);
  //  }
}
