// lib SPI para leer y escribir en la micro SD
#include <SPI.h>
#include <SD.h>
#include <FS.h>

#define CONECTION_SD_TIME 12000 // tiempo que chequea si la SD funciona

char separador = ';';

String leerArchivo(fs::FS &fs, const char *path)
{

  String payload = "";
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (file)
  {
    while (file.available())
    {
      char ch = file.read();
      payload += ch;
    }
    file.close();
    Serial.println(" Archivo leido");
    return payload;
  }
  else
  {
    Serial.println("Error leyendo el archivo");
    return "";
  }
}

// check para archivo csv
boolean checkChar(fs::FS &fs, const char *path, String headerCheck)
{
  File file = fs.open(path);
  String header = "";
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return false;
  }

  Serial.print("Read from file for check char: ");
  while (file.available())
  {
    char charFile = file.read();
    header += charFile;
    if (charFile == '\n')
    {
      file.close();
      if (header == headerCheck)
      {
        return true;
      }
      return false;
    }
  }
  file.close();
  return false;
}

boolean checkFile(fs::FS &fs, const char *path)
{

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("El archivo buscado no existe");
    file.close();
    return false;
  }

  Serial.printf("verificando archivo: %s\n", path);
  if (file.available())
  {
    file.close();
    return true;
  }
}

void writeFile(fs::FS &fs, const char *path, const String mensaje)
{
  Serial.printf("Escribiendo el archivo: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Error al abrir el archivo para escribirlo");
    return;
  }
  if (file.print(mensaje))
  {
    Serial.println("Archivo escrito");
  }
  else
  {
    Serial.println("Error al escribir");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void initFile(fs::FS &fs, const char * path, String init){
  if(checkFile(SD, path)){
    if(!checkChar(SD, path, init)){
      writeFile(SD, path, init.c_str());
    }
  }else{
    writeFile(SD, path, init.c_str());
  }
}