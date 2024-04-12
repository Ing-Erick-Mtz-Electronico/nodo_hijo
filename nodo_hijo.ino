#define ID_NODO ("22")
#define PATH ("/CSVnodoHijo_22.txt")
#define PATH2 ("/CopiaCSVnodoHijo_22.txt")
#define DATA_TOTAL ("/dataTotal_hijo_22.txt")

#include <WiFi.h>
#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#define BME_ADDRESS (0x76)

// variables del sleep
// #define TIME_OF_SAMPLING 3600000 //1 hora
#define SAMPLING_TIME 3600000 // 900000//3600000 //240000 //2 minutos prueba
#define CONECTION_TIME 180000   // tiempo en que se espera la conexion al nodo padre
#define mS_TO_uS_FACTOR 1000    // factor para pasar milis a micro segundos
#define S_TO_mS_FACTOR 60000       // factor para pasar segundos a milisegundos segundos

const char *ssid = "GIDEAMSERVER";
const char *password = "1234567890";

String serverName = "http://192.168.4.1/";
const char *pathData = "data";
const char *pathHora = "hora";
String responseHora;

Adafruit_BME280 bme;

unsigned long lastTime = 0;
unsigned long timerDelay = 0;

// variables HD38
#define sensorPin 26 // Pin analógico conectado al sensor

// bateria
#define bateryPin 33

// varibles SOIL NPK MODBUS
#define RE 15
#define RXD2 16
#define TXD2 17

//pin led
#define LED 32
boolean stateLed = true;

#include "usb_functions.h";
#include "tinyRTC_functions.h";
#include "mediciones.h";

void setup()
{
  pinMode(LED,OUTPUT);
  digitalWrite(LED,stateLed);

  Serial.begin(115200);
  while (!Serial)
  {
    Serial.print("."); // Espera hasta que el puerto serial se conecte
  }

  Wire.begin();
  setupRTC();
  setUp();
  sendData();
  flag = true;

  digitalWrite(LED,false);
  // modo deep sleep
  Serial.println("entrando a modo deep sleep");
  sleepEsp();
}

void loop() {}
