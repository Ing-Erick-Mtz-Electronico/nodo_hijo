#define ID_NODO ("27")

#include <ESP32Time.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME_ADDRESS (0x76)

//lib SPI para leer y escribir en la micro SD
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#define PATH ("/CSVnodoHijo27.txt")
#define PATH2 ("/CopiaCSVnodoHijo27.txt")

//variables del sleep
//#define TIME_OF_SAMPLING 3600000 //1 hora
#define SAMPLING_TIME 3600000 //240000 //2 minutos prueba

#define CONECTION_TIME 180000

const char* ssid = "GIDEAMSERVER";
const char* password = "1234567890";

String serverName = "http://192.168.4.1/";
const char* pathData = "data";
const char* pathHora = "hora";
String responseHora;

ESP32Time rtc;

Adafruit_BME280 bme;

unsigned long lastTime = 0;

char separador = ';';
RTC_DATA_ATTR boolean flag = false;
unsigned long timerDelay = 0;

//variables HD38
#define sensorPin 26 // Pin analógico conectado al sensor

//bateria
#define bateryPin 33

//varibles SOIL NPK MODBUS
#define RE 15
#define RXD2 16
#define TXD2 17

#include "mediciones.H";

void setup() {
  
  setUp();
  sendData();
  flag = true;
  //modo deep sleep
  Serial.println("entrando a modo deep sleep");
  sleepEsp(timerDelay*1000);
}

void loop() {}
