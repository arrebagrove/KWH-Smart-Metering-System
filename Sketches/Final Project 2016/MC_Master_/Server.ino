unsigned long data[NUM_METERS];
bool isNull[NUM_METERS];

void serverOnMinutePassed()
{
  unsigned long minute = getCurrentTime() - METER_POLL_INTERVAL_MINS - 3;
  for (int i = 0; i < NUM_METERS; i++)
  {
    unsigned long dat = currentUsage[i];
    bool nul = currentIsNull[i];
    //<NOSD>retrieveData(i, minute, &dat, &nul);
    data[i] = dat;
    isNull[i] = nul;
    Serial.print(i + 1);
    Serial.print(", ");
    Serial.print(minute);
    Serial.print(": ");
    
    if (isNull[i])
      Serial.println("Null");
    else
      Serial.println(data[i]);
  }

  unsigned long day = minute / 1440;
  int month;
  int year = 2016;
  while (day > 1461)
  {
    day -= 1461;
    year += 4;
  }
  while (day > (year % 4 == 0 ? 366 : 365))
  {
    day -= (year % 4 == 0 ? 366 : 365);
    year++;
  }
  if (day < 31)
  {
    month = 1;
    day++;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28))
  {
    month = 2;
    day += 1 - 31;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31)
  {
    month = 3;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28);
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30)
  {
    month = 4;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31)
  {
    month = 5;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30)
  {
    month = 6;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30 + 31)
  {
    month = 7;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31)
  {
    month = 8;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30 - 31;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30)
  {
    month = 9;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30 - 31 - 31;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
  {
    month = 10;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30 - 31 - 31 - 30;
  }
  else if (day < 31 + (year % 4 == 0 ? 29 : 28) + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
  {
    month = 11;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31;
  }
  else
  {
    month = 12;
    day += 1 - 31 - (year % 4 == 0 ? 29 : 28) - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31 - 30;
  }
  unsigned long minInDay = minute % 1440;
  int _min = minInDay % 60;
  int _hour = minInDay / 60;

  gprsSerial.println("AT+CIPSTART=\"tcp\",\"ssh.daveg.work\",\"6003\"");
  delay(2000);
  //loopGPRS();
  gprsSerial.println("AT+CIPSEND");
  delay(4000);
  //loopGPRS();
  gprsSerial.print("#STA:31415;TM:");
  if (month < 10)
    gprsSerial.print('0');
  gprsSerial.print(month);
  gprsSerial.print('/');
  if (day < 10)
    gprsSerial.print('0');
  gprsSerial.print(day);
  gprsSerial.print('/');
  gprsSerial.write(year / 1000 + '0');
  gprsSerial.write((year % 1000) / 100 + '0');
  gprsSerial.write((year % 100) / 10 + '0');
  gprsSerial.write((year % 10) + '0');
  gprsSerial.print(',');
  if (_hour < 10)
    gprsSerial.print('0');
  gprsSerial.print(_hour);
  gprsSerial.print(":");
  if (_min < 10)
    gprsSerial.print('0');
  gprsSerial.print(_min);
  gprsSerial.print(":00;C:15;V:7.19;");
  for (int i = 0; i < NUM_METERS; i++)
  {
    if (!isNull[i])
    {
      int puNum = i + 1;
      gprsSerial.print("PU");
      if (puNum < 10)
        gprsSerial.print('0');
      gprsSerial.print(puNum);
      gprsSerial.print(":");
      gprsSerial.print(data[i]);
      gprsSerial.print(";");
    }
  }
  gprsSerial.print("DI:333300;DO:0000;#");
  delay(500);
  gprsSerial.println((char)26);
  delay(5000);
  //loopGPRS();
  gprsSerial.println("AT+CIPCLOSE");
}


