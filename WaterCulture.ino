#include <Wire.h> // I2C 통신
#include "BH1750.h" // 조도 센서
#include "ECSensor.h" // EC 센서
#include "pHSensor.h" // pH 센서
#include "LiquidCrystal_I2C.h" // LCD
#include "DS3231.h" // RTC
#include "DHT11.h" // 온습도 센서

int R_LED = 4; // LED 릴레이
int R_FAN = 5; // 팬 릴레이
int R_LIGHT = 6; // 백열등 릴레이

LiquidCrystal_I2C LCD(0x27,16,2); // 2004 LCD
BH1750 LightMeter; // 조도 센서
DS3231 RTC; // RTC
#define DHT_PIN 3 // 온습도 센서 핀


uint16_t lux = 0;
DateTime NowTime;

// EC 센서 : A1  ms/cm
// pH 센서 : A0
// DS18B20(수온 센서) : 2

void setup()
{
  Serial.begin(9600); // 시리얼 통신
  Wire.begin(); // I2C 통신

  pinMode(R_LED, OUTPUT); // 릴레이
  pinMode(R_FAN, OUTPUT);
  pinMode(R_LIGHT, OUTPUT);

  LCD.begin(); // LCD
  LCD.backlight(); // LCD 백라이트 켜기
  LCD.clear();
  LCD.print("Loading...");
  LCD.setCursor(0, 2);
  LCD.print("Water-Culture!");

  //digitalWrite(R_LED, !HIGH);
  //digitalWrite(R_FAN, !HIGH);
  //digitalWrite(R_LIGHT, !HIGH);
  //delay(50);
  digitalWrite(R_LED, !LOW);
  digitalWrite(R_FAN, !LOW);
  digitalWrite(R_LIGHT, !LOW);

  DateTime DT(2022, 11, 07, 23, 27, 5, 2); // 년, 월, 일, 시(0 ~ 23), 분, 초, 요일(0 = 일, 1 = 월, ..., 6 = 토) 순서 (요일은 상관 없음.)
  RTC.setDateTime(DT);

  LightMeter.begin(); // 조도 센서
  lux = LightMeter.readLightLevel();

  for (int i = 0; i < 100; i ++) // EC 센서 초기화
  {
    ProcessECSensor();
    delay(10);
  }
}

void loop()
{
  char buf[20];
  float pHValue = GetpHValue(); // pH 센서 값 가져오기

  LCD.setCursor(0, 0);
  NowTime = RTC.now(); // 현재 시간 가져오기
  LCD.print(NowTime.year()); LCD.print("-");
  if (NowTime.month() < 10) LCD.print("0"); LCD.print(NowTime.month()); LCD.print("-");
  if (NowTime.date() < 10) LCD.print("0"); LCD.print(NowTime.date()); LCD.print("==");
  if (NowTime.hour() < 10) LCD.print("0"); LCD.print(NowTime.hour()); LCD.print(":");
  if (NowTime.minute() < 10) LCD.print("0"); LCD.print(NowTime.minute()); LCD.print(":");
  if (NowTime.second() < 10) LCD.print("0"); LCD.print(NowTime.second());

  lux = LightMeter.readLightLevel(); // 조도 센서 값

 
  LCD.setCursor(0, 1);
  LCD.print("EC: ");
  if (GetECValue() < 10.00) LCD.print("0");
  LCD.print(GetECValue());
  LCD.print(", pH: ");
  if (pHValue < 10.00) LCD.print("0");
  LCD.print(pHValue);

  Serial.print("<br> EC : ");
  Serial.print(GetECValue());
  Serial.print("<br> pH : ");
  Serial.print(pHValue);

  LCD.setCursor(0, 1);
  LCD.print("TE: ");
  if (DHT_SEN.readTemperature() < 10.00) LCD.print("0");
  LCD.print(DHT_SEN.readTemperature());
  LCD.print(", HU: ");
  if (DHT_SEN.readHumidity() < 10.00) LCD.print("0");
  LCD.print(DHT_SEN.readHumidity());

  Serial.print("<br> Temp. : ");
  Serial.print(DHT_SEN.readTemperature());
  Serial.print("<br> Humi. : ");
  Serial.print(DHT_SEN.readHumidity());
  Serial.println();

  for (int i = 0; i < 50; i ++) // EC 센서 처리
  {
    ProcessECSensor();
    delay(10);
  }

  if (NowTime.hour() >= 7 && NowTime.hour() <= 12 + 10)
  {
    if (lux < 1000)
      digitalWrite(R_LED, !HIGH);
    else
      digitalWrite(R_LED, !LOW);
  }
  else
    digitalWrite(R_LED, !LOW);

  if (DHT_SEN.readTemperature() < 25.00)
    digitalWrite(R_LIGHT, !HIGH);
  else
    digitalWrite(R_LIGHT, !LOW);

  if (DHT_SEN.readTemperature() > 30.00)
    digitalWrite(R_FAN, !HIGH);
  else
    digitalWrite(R_FAN, !LOW);
}
