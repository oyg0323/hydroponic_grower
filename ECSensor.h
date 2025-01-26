#pragma once

#include "OneWire.h"

#define StartConvert 0
#define ReadTemperature 1

const byte numReadings = 20;
byte ECsensorPin = A1;
byte DS18B20_Pin = 2;
unsigned int AnalogSampleInterval = 25, printInterval = 700, tempSampleInterval = 850;
unsigned int readings[numReadings];
byte index = 0;
unsigned long AnalogValueTotal = 0;
unsigned int AnalogAverage = 0, averageVoltage = 0;
unsigned long AnalogSampleTime, printTime, tempSampleTime;
float temperature, ECcurrent;

OneWire ds(DS18B20_Pin);

float GetECValue()
{
  return ECcurrent;
}

float GetWaterTemp()
{
  return temperature;
}

float TempProcess(bool ch)
{
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;

  if (!ch)
  {
    if ( !ds.search(addr))
    {
      Serial.println("no more sensors on chain, reset search!");
      ds.reset_search();
      return 0;
    }
    if ( OneWire::crc8( addr, 7) != addr[7])
    {
      Serial.println("CRC is not valid!");
      return 0;
    }
    if ( addr[0] != 0x10 && addr[0] != 0x28)
    {
      Serial.print("Device is not recognized!");
      return 0;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);
  }
  else
  {
    byte present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);

    for (int i = 0; i < 9; i++)
      data[i] = ds.read();

    ds.reset_search();
    byte MSB = data[1];
    byte LSB = data[0];
    float tempRead = ((MSB << 8) | LSB);
    TemperatureSum = tempRead / 16;
  }

  return TemperatureSum;
}

void ProcessECSensor()
{
  if (millis() - AnalogSampleTime >= AnalogSampleInterval)
  {
    AnalogSampleTime = millis();
    AnalogValueTotal = AnalogValueTotal - readings[index];
    readings[index] = analogRead(ECsensorPin);
    AnalogValueTotal = AnalogValueTotal + readings[index];
    index = index + 1;

    if (index >= numReadings)
      index = 0;

    AnalogAverage = AnalogValueTotal / numReadings;
  }

  if (millis() - tempSampleTime >= tempSampleInterval)
  {
    tempSampleTime = millis();
    temperature = TempProcess(ReadTemperature);
    TempProcess(StartConvert);
  }

  if (millis() - printTime >= printInterval)
  {
    printTime = millis();
    averageVoltage = AnalogAverage * (float)5000 / 1024;
    //Serial.print("Analog value:");
    //Serial.print(AnalogAverage);
    //Serial.print("    Voltage:");
    //Serial.print(averageVoltage);
    //Serial.print("mV    ");
    //Serial.print("temp:");
    //Serial.print(temperature);
    //Serial.print("^C     EC:");

    float TempCoefficient = 1.0 + 0.0185 * (temperature - 25.0);
    float CoefficientVolatge = (float)averageVoltage / TempCoefficient;

    if (CoefficientVolatge < 150)
      Serial.println("No solution!");
    else if (CoefficientVolatge > 3300)
      Serial.println("Out of the range!");
    else
    {
      if (CoefficientVolatge <= 448)
        ECcurrent = 6.84 * CoefficientVolatge - 64.32;
      else if (CoefficientVolatge <= 1457)
        ECcurrent = 6.98 * CoefficientVolatge - 127;
      else
        ECcurrent = 5.3 * CoefficientVolatge + 2278;
        
      ECcurrent /= 1000;

      //Serial.print(ECcurrent, 2);
      //Serial.println("ms/cm");
    }
  }
}

void ECInit()
{
  for (byte thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

  TempProcess(StartConvert);
  AnalogSampleTime = millis();
  printTime = millis();
  tempSampleTime = millis();
}
