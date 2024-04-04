/*               _   __              _               ____   ___
 _    __  ____  (_) / /_ ___        (_)  ___        / __/  / _ \
| |/|/ / / __/ / / / __// -_)      / /  / _ \      _\ \   / // /
|__,__/ /_/   /_/  \__/ \__/      /_/  /_//_/     /___/  /____/ */


/*Bibliothèque*/
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "time.h"

/*Port SPI*/
#define SCK  18
#define MISO  19
#define MOSI  23
#define CS  32



//Initialisation variable 
String dataMessage;
int Refresh = 500; // 1min = 60000
SPIClass spi = SPIClass(VSPI);

/*Initialisation de la carte SD*/
void initSDCard(){
  spi.begin(SCK, MISO, MOSI, CS);
  if (!SD.begin(CS,spi,80000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

/*Écriture du fichier*/
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

/*Rajout dans le fichier*/
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

/*Préparation du fichier d'enregistrement*/
void initFile(){
  File file = SD.open("/Valeur.csv");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/Valeur.csv", "Date; Heure; Température; Humidité; Indice de COV; Forme Aldéhyde; CO²; PM_2.5; PM_1; PM_10 \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();
}

/*Écriture des lignes d'informations dans la carte SD*/
void ecriture(){
  //Variable de fonctionement
int Date = rand()%31 +1;
int Heure = rand()%24 +1;
int Temp = rand()%35 +1;
int Hum = rand()%99 +1;
int COV = rand()%25000 +1;
int Alde = rand();
int CO2 = rand()%1000 +1;
int PM_25 = rand();
int PM_1 = rand();
int PM_10 = rand();
  dataMessage = String(Date) +";"+ String(Heure) +";"+ String(Temp) + ";" + String(Hum) + ";" + String(COV) + ";" + String(Alde) + ";" + String(CO2) + ";" + String(PM_25) + ";" + String(PM_1) + ";" + String(PM_10) + "\r\n";
  Serial.print("Sauvegarde: ");
  Serial.println(dataMessage);
  appendFile(SD, "/Valeur.csv", dataMessage.c_str());
}

void setup(){
  Serial.begin(115200);
  initSDCard();
  initFile();
}

void loop(){
  ecriture();
  delay(Refresh);
}
