#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <AiEsp32RotaryEncoder.h>

extern LiquidCrystal_I2C lcd;
extern DHT dht_sensor;
extern AiEsp32RotaryEncoder encoder;

double termValue(int termPin);
void IRAM_ATTR readEncoderISR();
void rotary_onButtonClick();
void encoderSetup();
void dhtSetup();
void lcdSetup();
void handleSetTemperatureMenu(long acceleration);
void handleSetHumidityMenu(long acceleration);
void handleMainMenu();
void handlePresetsMenu();
void startControl();

#endif // FUNCTIONS_H
