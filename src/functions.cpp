#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include <global_vars.h>
#include <functions.h>
#include <AiEsp32RotaryEncoder.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x3F, 16 column and 2 rows
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
AiEsp32RotaryEncoder encoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

double termValue(int termPin) {
  int adcValue = analogRead(termPin);                               // read ADC pin
  double voltage = (float)adcValue / 4095.0 * 3.3;                // calculate voltage
  double Rt = 10 * voltage / (3.3 - voltage);                     // calculate resistance value of thermistor
  double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); // calculate temperature (Kelvin)
  double tempThermistor = tempK - 278.25;                         // calculate temperature (Celsius)
  return tempThermistor;
}

void IRAM_ATTR readEncoderISR() {
  encoder.readEncoder_ISR();
}

void rotary_onButtonClick() {
  static unsigned long lastTimePressed = 0;
  // ignore multiple press in that time milliseconds
  if (millis() - lastTimePressed < 300) {
    return;
  }
  lastTimePressed = millis();
}

void encoderSetup() {
  encoder.begin();
  encoder.setup(readEncoderISR);
  encoder.setBoundaries(0, 4, false);
  encoder.disableAcceleration();
}

void dhtSetup() {
  pinMode(DHT_SENSOR_PIN, INPUT);
  dht_sensor.begin();
}

void lcdSetup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void handleSetTemperatureMenu(long acceleration) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  encoder.setBoundaries(0, 99, false);
  encoder.setAcceleration(acceleration);
  lcd.setCursor(14,1);
  lcd.print("ºC");
  while(!encoder.isEncoderButtonClicked()) {
    if(encoder.encoderChanged() != 0) {
      lcd.setCursor(12,1);
      lcd.print(encoder.readEncoder());
    }
  }
  lcd.clear();
  encoderSetup();
}

void handleSetHumidityMenu(long acceleration) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Humidity");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  encoder.setBoundaries(0, (int)dht_sensor.readHumidity(), false);
  Serial.println((int)dht_sensor.readHumidity());
  encoder.setAcceleration(acceleration);
  lcd.setCursor(15,1);
  lcd.print("%");
  while(!encoder.isEncoderButtonClicked()) {
    if(encoder.encoderChanged() != 0) {
      lcd.setCursor(12,1);
      lcd.print(encoder.readEncoder());
    }
  }
  lcd.clear();
  encoderSetup();
}
void handlePresetsMenu() {
  lcd.clear();
  encoderSetup();
}

void startControl() {

}

void handleMainMenu() {
  lcd.setCursor(1,0);
  lcd.print("!~ThermoDryer~!");
  lcd.setCursor(0,1);
  lcd.print(">");
  switch(encoder.readEncoder()) {  
    case 0:
      if(encoder.encoderChanged() != 0) {
        //(coluna,linha)
        lcd.setCursor(1, 1);
        lcd.print("                ");
      }
      if (encoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
      }
      break;
    case 1:
      if(encoder.encoderChanged() != 0) {
        lcd.setCursor(1, 1);
        lcd.print("                ");
        lcd.setCursor(1, 1);
        lcd.print("Set Temperature");
      }
      if (encoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
        handleSetTemperatureMenu(ACCELERATION_VALUE);
      }
      break;
    case 2:
      if(encoder.encoderChanged() != 0) {
        lcd.setCursor(1, 1);
        lcd.print("                ");
        lcd.setCursor(1, 1);
        lcd.print("Set Humidity");
      }
      if (encoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
        handleSetHumidityMenu(ACCELERATION_VALUE);
      }
      break;
    case 3:
      if(encoder.encoderChanged() != 0) {
        lcd.setCursor(1, 1);
        lcd.print("                ");
        lcd.setCursor(1, 1);
        lcd.print("Presets");
      }
      if (encoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
        handlePresetsMenu();
      }
      break;
    case 4:
      if(encoder.encoderChanged() != 0) {
        lcd.setCursor(1, 1);
        lcd.print("                ");
        lcd.setCursor(1, 1);
        lcd.print("Start");
      }
      if (encoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
        startControl();
      }
      break;
  }
}

#endif // FUNCTIONS_H
