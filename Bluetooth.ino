#include <base64.h>
#include<SD.h>

#define SD_CSPin 4

short int i=0;
char ch[30];
char dh[40];

File sd,td;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
            Serial.println("An Error has occurred while mounting SD");
  }
  while (!SD.begin(SD_CSPin)) {
            Serial.print(".");
            delay(500);
  }

  while(1)
  {
  i=0;
  sd=SD.open("1.wav");
  if(!sd.available())
  {
    Serial.print("Conversion Complete");
    break;
  }
  for(i;i<30;i++)
  {
    ch[i]=sd.read();
  }
  strcpy(dh,base64.encode(ch));
  Serial.println(ch);
  sd.close();
  td=SD.open("tmt.txt",FILE_WRITE);
  td.print(ch);
  td.close();

}
}

void loop() {
  // put your main code here, to run repeatedly:

}
