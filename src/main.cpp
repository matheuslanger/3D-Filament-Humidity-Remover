/**********************************************************************
  Filename    : main
  Description : 3D Filament Humidity Dryer.
  Auther      : Matheus Araujo Langer
  Modification: 20/11/2023
**********************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define PIN_ANALOG_IN 13
#define DHT_SENSOR_PIN 23 // ESP32 pin GPIO23 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x3F, 16 column and 2 rows
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup()
{
  Serial.begin(9600);
  pinMode(DHT_SENSOR_PIN, INPUT);
  lcd.init();
  lcd.backlight();
  dht_sensor.begin();
}

void loop()
{
  lcd.setCursor(0, 0);

  int analogValue = analogRead(12);

  float humi = dht_sensor.readHumidity();
  float tempC = dht_sensor.readTemperature();

  int adcValue = analogRead(PIN_ANALOG_IN);                       //read ADC pin
  double voltage = (float)adcValue / 4095.0 * 3.3;                // calculate voltage
  double Rt = 10 * voltage / (3.3 - voltage);                     //calculate resistance value of thermistor
  double tempK = 1 / (1 / (273.15 + 25) + log(Rt / 10) / 3950.0); //calculate temperature (Kelvin)
  double tempThermistor = tempK - 278.25;                         //calculate temperature (Celsius)
  Serial.println(tempThermistor);

  lcd.clear();

  if (isnan(tempC) || isnan(humi))
  {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  }
  else
  {
    if(analogValue <= 1365) {
      lcd.print("1");
    }
    else if (analogValue <= 2730 && analogValue > 1365) {
      lcd.print("2");
    }
    else {
      lcd.print("3");
    }
  }
  delay(1000);
}