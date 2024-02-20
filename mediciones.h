String leerArchivo(fs::FS &fs, const char * path){
  
  String payload ="";
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(file){
    while(file.available()){
      char ch = file.read();
      payload += ch;
    }
    file.close();
    Serial.println(" Archivo leido");
    return  payload;
  } else {
    Serial.println("Error leyendo el archivo");
    return "";
  } 
}

//check para archivo csv
boolean checkChar(fs::FS &fs, const char * path, String headerCheck){
  File file = fs.open(path);
  String header="";
  if(!file){
    Serial.println("Failed to open file for reading");
    return false;
  }

  Serial.print("Read from file for check char: ");
  while(file.available()){
    char charFile = file.read();
    header += charFile;
    if(charFile == '\n'){
      file.close();
      if(header == headerCheck){
        return true;
      }
      return false;
    }
  }
  file.close();
  return false;
}

boolean checkFile(fs::FS &fs, const char * path){
  
  File file = fs.open(path);
  if(!file){
    Serial.println("El archivo buscado no existe");
    file.close();
    return false;
  }
  
  Serial.printf("verificando archivo: %s\n", path);
  if(file.available()){
    file.close();
    return true;
  }
   
}

void writeFile(fs::FS &fs, const char * path,const String mensaje) {
  Serial.printf("Escribiendo el archivo: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Error al abrir el archivo para escribirlo");
    return;
  }
  if (file.print(mensaje)) {
    Serial.println("Archivo escrito");
  } else {
    Serial.println("Error al escribir");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

String readBME280Temperature() {
  //while(!bme.begin(BME_ADDRESS));
  // Read temperature as Celsius (the default)
  float t = bme.readTemperature();
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {    
    Serial.println("Failed to read from BME280 sensor!");
    return "0";
  }
  else {
    //Serial.println(String("temperatura BME: ")+String(t));
    return String(t);
  }
}

String readBME280Humidity() {
  //while(!bme.begin(BME_ADDRESS));
  float h = bme.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "0";
  }
  else {
    //Serial.println(String("humedad BME: ")+String(h));
    return String(h);
  }
}

String readBME280Pressure() {
  //while(!bme.begin(BME_ADDRESS));
  float p = bme.readPressure() / 100.0F;
  if (isnan(p)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "0";
  }
  else {
    //Serial.println(String("presion BME: ")+String(p));
    return String(p);
  }
}
String readBME280Altitude() {
  //while(!bme.begin(BME_ADDRESS));
  float a = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  if (isnan(a)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "0";
  }
  else {
    //Serial.println(String("altitud BME: ")+String(a));
    return String(a);
  }
}

String readDH38Humidity(){
  int sensorValue = 0; // Variable para almacenar el valor leído
  int humedad = 0;
  sensorValue = analogRead(sensorPin);
  humedad = map(sensorValue, 0, 4095, 100, 0);
  return String(humedad);
}

String readBatery(){
  int sensorValue = 0;
  float voltaje = 0.0;
  sensorValue = analogRead(bateryPin);
  voltaje = (sensorValue*(13.0/4096.0));
  return String(voltaje);
}

String soilData(){
  // Modbus request for reading all values
  const byte allMeasure[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08};

  // Recoleccion de valores
  byte values[19];
  
  uint16_t humidityInt,temperatureInt,conductivity,PHInt,nitro,phos,pota = 0;
  float humidity,temperature,PH = 0.0;
  
  digitalWrite(RE,HIGH);
  
  if(Serial2.write(allMeasure,sizeof(allMeasure))==8){
    delay(18);
    digitalWrite(RE,LOW);
    delay(12);

    Serial2.readBytes(values,sizeof(values));
    
    Serial.println("sensor SOIL leido");
//    for(int i=0; i<sizeof(values);i++){
//      Serial.print(values[i],HEX);
//      Serial.print(" ");
//    }
    //Serial.println();
    humidityInt = (values[3] << 8) | values[4];
    humidity = humidityInt/10.0F;
    
    temperatureInt = (values[5] << 8) | values[6];
    temperature = temperatureInt/10.0F;

    conductivity = (values[7] << 8) | values[8];
    
    PHInt = (values[9] << 8) | values[10];
    PH = PHInt/10.0F;
    
    nitro = (values[11] << 8) | values[12];
    
    phos = (values[13] << 8) | values[14];
    
    pota = (values[15] << 8) | values[16];
    
    /*Serial.print("humedad: ");
    Serial.print(humidity);
    Serial.println();

    Serial.print("temperatura: ");
    Serial.print(temperature);
    Serial.println();
    
    Serial.print("conductividad: ");
    Serial.print(conductivity);
    Serial.println();
    
    Serial.print("PH: ");
    Serial.print(PH);
    Serial.println();

    Serial.print("Nitrogeno: ");
    Serial.print(nitro);
    Serial.println();

    Serial.print("Fosforo: ");
    Serial.print(phos);
    Serial.println();

    Serial.print("potasio: ");
    Serial.print(pota);
    Serial.println();*/

  }
  String result = "";
  result += String(humidity);
  result.concat(separador);
  result += String(temperature);
  result.concat(separador);
  result += String(conductivity);
  result.concat(separador);
  result += String(PH);
  result.concat(separador);
  result += String(nitro);
  result.concat(separador);
  result += String(phos);
  result.concat(separador);
  result += String(pota);
  
  return result;
}

//convierte de long a string
unsigned long longToString(String timeEpoch){
  
  unsigned long y = 0;

  for (int i = 0; i < timeEpoch.length(); i++) {
      char c = timeEpoch.charAt(i);
     if (c < '0' || c > '9') break;
     y *= 10;
     y += (c - '0');
  }

  return y;
}

void sleepEsp(unsigned long timerSleep){
  esp_sleep_enable_timer_wakeup(timerSleep);
  Serial.flush(); 
  esp_deep_sleep_start();
}

boolean getHora(){
  
  //while(true){
    
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName+pathHora);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode == 200) {
      Serial.println("Petición de hora exitosa");
      responseHora = http.getString();
      rtc.setTime(longToString(responseHora));
      http.end();
      return true;
    }
    Serial.print("Error solicitando hora, response code: ");
    Serial.println(httpResponseCode);
    http.end();
    return false;
    //delay(1000);
  //}
}

boolean sendPost(String httpRequestData){
  //enviar post
  //while(true){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,serverName+pathData);
    http.addHeader("Content-Type", "text/plain");
    
    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode == 200) {
      Serial.println("Enviado");
      timerDelay = longToString(http.getString());
      timerDelay = SAMPLING_TIME - timerDelay;
      Serial.println(timerDelay);
      http.end();
      return true;
    }
    Serial.print("Error de envio, response code: ");
    Serial.println(httpResponseCode);
    http.end();
    return false;
    //delay(1000);
  //}
}

String measurement(String timeNow){
  char finalizador = '\n';
  Serial.println("Medicion inicida");
  String object_med = ID_NODO;
  object_med.concat(separador);
  object_med.concat(timeNow);
  object_med.concat(separador);
  delay(500);
  object_med.concat(readBME280Temperature());
  object_med.concat(separador);
  delay(500);
  object_med.concat(readBME280Humidity());
  object_med.concat(separador);
  delay(500);
  object_med.concat(readBME280Pressure());
  object_med.concat(separador);
  delay(500);
  object_med.concat(readBME280Altitude());
  object_med.concat(separador);
  object_med.concat(readDH38Humidity());
  object_med.concat(separador);
  object_med.concat(soilData());
  object_med.concat(separador);
  object_med.concat(readBatery());
  object_med.concat(finalizador);
  Serial.println(object_med);
  
  return object_med;
}

void sendData(){
  //if ((millis() - lastTime) > timerDelay) {
    
    String dataMeasurement = leerArchivo(SD, PATH2);
    if(dataMeasurement.length()>1){
      if(sendPost(dataMeasurement)){
        writeFile(SD, PATH2, String("").c_str());
      }
    }
    
    getHora();
    dataMeasurement = measurement(rtc.getTime("%FT%T"));
    if(sendPost(dataMeasurement)){
      appendFile(SD, PATH, dataMeasurement.c_str());
    }else{
      appendFile(SD, PATH2, dataMeasurement.c_str());
      Serial.println("Trama no enviada, entrando a modo deep sleep");
      sleepEsp((SAMPLING_TIME-(millis() - lastTime))*1000);
    }
    //lastTime = millis();
  //}
}

void setUp(){
  lastTime = millis();
  
  String init = "ID_NODO";
         init.concat(separador);
         init.concat("fecha");
         init.concat(separador);
         init.concat("temperatura_BME");
         init.concat(separador);
         init.concat("humedad_BME");
         init.concat(separador);
         init.concat("presion_BME");
         init.concat(separador);
         init.concat("altitud_BME");
         init.concat(separador);
         init.concat("humedad_HD38");
         init.concat(separador);
         init.concat("humedad_SOIL");
         init.concat(separador);
         init.concat("temperatura_SOIL");
         init.concat(separador);
         init.concat("conductividad_SOIL");
         init.concat(separador);
         init.concat("PH_SOIL");
         init.concat(separador);
         init.concat("nitrogeno_SOIL");
         init.concat(separador);
         init.concat("fosforo_SOIL");
         init.concat(separador);
         init.concat("potasio_SOIL");
         init.concat(separador);
         init.concat("nivel_bateria\n");
  
  Serial.begin(115200);
  while(!Serial) {
    Serial.print("."); // Espera hasta que el puerto serial se conecte
  }
  
  Serial2.begin(4800, SERIAL_8N1, RXD2, TXD2);
  pinMode(RE, OUTPUT);
  digitalWrite(RE,HIGH);
  
  Wire.begin();  //enable I2C port.
  if(!bme.begin(BME_ADDRESS)){
    Serial.println("No hay un módulo BME conectado");
    delay(2000);
  }else{
    Serial.println("BME conectado");
  }
  
  Serial.println("Inicializando SD card...");
  while(!SD.begin(5)) {
    Serial.println("Inicialización fallida!");
    delay(2000);
  }
  Serial.println("Inicialización lista.");
  if(checkFile(SD, PATH)){
    if(!checkChar(SD, PATH, init)){
      writeFile(SD, PATH, init.c_str());
    }
  }else{
    writeFile(SD, PATH, init.c_str());
  }
  Serial.println("Archivo verificado");

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  
  while(WiFi.status() != WL_CONNECTED) {
    if (((millis() - lastTime) > CONECTION_TIME)){
      appendFile(SD, PATH2, measurement(rtc.getTime("%FT%T")).c_str());
      sleepEsp((SAMPLING_TIME-(millis() - lastTime))*1000);
      //lastTime = millis();
     }
    delay(10000);
    WiFi.begin(ssid, password);
    Serial.println("Connecting");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
