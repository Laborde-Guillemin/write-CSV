#include<Arduino.h>
#include "FS.h"
#include "SD.h"

void fichier(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
    
}