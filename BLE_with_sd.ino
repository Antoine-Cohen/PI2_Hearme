#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
String file_contents = "";
int file_l = 0;

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
    }

    file.close();
    file_to_write.close();

} 
void read_hex(fs::FS &fs){
  File file_hex = fs.open("/sd_contents.txt");
  file_l = 0;
  while(file_hex.available()){
    file_contents += (char)file_hex.read();
    file_l++;
  }
}

void setup() {
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
  rw_to_hex(SD,"/test.wav");
  read_hex(SD);



  Serial.println("Starting BLE work!");
  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  int count = 10000;
  char random_content[count];

  for(int i=0; i<count; i++)
  {
    random_content[i] = 'A';
  }
  // const char* contents_str = file_contents.c_str();
  
  // Serial.println(contents_str);                                                                                
  pCharacteristic->setValue(random_content);
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
