#define PHSensorPin 0

unsigned long int avgValue;
float b;
int buf[10], temp;

float GetpHValue()
{
  int i, j;

  for (i = 0; i < 10; i++)
  {
    buf[i] = analogRead(PHSensorPin);
    delay(10);
  }

  for (i = 0; i < 9; i++)
  {
    for (j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  for (avgValue = 0, i = 2; i < 8; i++)
    avgValue += buf[i];

  //Serial.print("pH Meter ## RAW Value : ");
  //Serial.print(avgValue / 6.0);
  //Serial.print("\tRAW % : ");
  //Serial.print(avgValue / 6.0 / 1024.0 * 100.0);
  float phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = 3.5 * phValue;
  //Serial.print("\tpH : ");
  //Serial.print(phValue);

  return phValue; 
}

