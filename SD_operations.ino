/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Base64.h"

int file_l = 0;
String file_contents = "";





void rw_to_hex(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    File file_to_write = fs.open("/sd_contents.txt",FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
    Serial.print("Converting to Hex...");
    while(file.available()){

        //Serial.write(file.read());
        file_to_write.print(file.read(),HEX);

        file_l++;
    }

    file.close();
    file_to_write.close();

} 
void read_hex(fs::FS &fs){
  File file_hex = fs.open("/sd_contents.txt");
  while(file_hex.available()){
    file_contents += (char)file_hex.read();
  }
}





// void writeFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Writing file: %s\n", path);

//     File file = fs.open(path, FILE_WRITE);
//     if(!file){
//         Serial.println("Failed to open file for writing");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("File written");
//     } else {
//         Serial.println("Write failed");
//     }
//     file.close();
// }

// void appendFile(fs::FS &fs, const char * path, const char * message){
//     Serial.printf("Appending to file: %s\n", path);

//     File file = fs.open(path, FILE_APPEND);
//     if(!file){
//         Serial.println("Failed to open file for appending");
//         return;
//     }
//     if(file.print(message)){
//         Serial.println("Message appended");
//     } else {
//         Serial.println("Append failed");
//     }
//     file.close();
// }



// void deleteFile(fs::FS &fs, const char * path){
//     Serial.printf("Deleting file: %s\n", path);
//     if(fs.remove(path)){
//         Serial.println("File deleted");
//     } else {
//         Serial.println("Delete failed");
//     }
// }



void setup(){
    Serial.begin(115200);
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    // Serial.print("SD Card Type: ");
    // if(cardType == CARD_MMC){
    //     Serial.println("MMC");
    // } else if(cardType == CARD_SD){
    //     Serial.println("SDSC");
    // } else if(cardType == CARD_SDHC){
    //     Serial.println("SDHC");
    // } else {
    //     Serial.println("UNKNOWN");
    // }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    //Serial.printf("SD Card Size: %lluMB\n", cardSize);

    
    // readFile(SD, "/foo.txt");
    rw_to_hex(SD,"/test.wav");
    read_hex(SD);
    Serial.println("File contents:");
    Serial.println(file_contents);
    
    // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(){

}
