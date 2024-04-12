#include <ESP32Time.h>
#include <RTClib.h>

// RTCESP_DATA_ATTR boolean flag = false; //se indica si el reset ha sido por el mmodo deep sleep
// RTCESP_DATA_ATTR boolean rebootNow = false;  //se indica si el reset ha sido por el mmodo deep sleep

// reloj externo
RTC_DS1307 rtc;
DateTime now;
// reloj interno de la esp
ESP32Time rtcESP;

RTC_DATA_ATTR boolean flag = false;

void setTimeRTC(long timeRequest)
{
  rtcESP.setTime(timeRequest);
  rtc.adjust(DateTime(timeRequest));
  now = rtc.now();
}

String getTimeRTC()
{
  if (rtc.begin())
  {
    Serial.println("hora modulo RTC");
    now = rtc.now();
    return now.timestamp();
  }
  else
  {
    Serial.println("hora esp");
    return rtcESP.getTime("%FT%T");
  }
}

int getMinute()
{
  if (rtc.begin())
  {
    now = rtc.now();
    return now.minute();
  }else
  {
    return rtcESP.getMinute();
  }
}

void setupRTC()
{
  lastTime = millis();
  DateTime rtcOneCharge = DateTime(F(__DATE__), F(__TIME__));
  long rtcUnixOneCharge = long(rtcOneCharge.unixtime());
  long rtcUnix;
  boolean noRTC = true;
  boolean lastTimeRTC = millis();
  while (noRTC)
  {
    if(!rtc.begin())
    {
      if (((millis() - lastTimeRTC) > CONECTION_SD_TIME))
      {
        digitalWrite(LED,true);
        noRTC = false;
      }
      Serial.println("Modulo RTC no conectado");
      digitalWrite(LED,stateLed);
      delay(500);
      stateLed = !stateLed;
    }else
    {
      noRTC = false;
    }

  }

  if (rtc.begin())
  {

    Serial.println("módulo RTC conectado");
    now = rtc.now();

    rtcUnix = long(now.unixtime());

    if (rtcUnix < rtcUnixOneCharge)
    {
      rtc.adjust(rtcOneCharge);
      now = rtc.now();
      Serial.println("hora ajustada");
    }

    rtcESP.setTime(long(now.unixtime()));
  }
  else
  {
    Serial.println("módulo RTC no conectado");

    rtcUnix = rtcESP.getEpoch();

    if (rtcUnix < rtcUnixOneCharge)
    {
      rtcESP.setTime(rtcOneCharge.unixtime());
    }
  }
}