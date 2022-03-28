#include <Arduino.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <I2SMEMSSampler.h>
#include <ADCSampler.h>
#include <I2SOutput.h>
#include <DACOutput.h>
#include <SDCard.h>
#include "SPIFFS.h"
#include <WAVFileReader.h>
#include <WAVFileWriter.h>
#include "BluetoothSerial.h"
#include "config.h"

BluetoothSerial SerialBT;

int val_sensibility = 50;
int analogMICPin_1 = 35;
int vib_1 = 12;
int analogMICPin_2 = 32;
int vib_2 = 13;
int analogMICPin_3 = 33;
int vib_3 = 14;
int led = 34;
bool record_mode = 0;

int min (float list[3]){
  float min = 0;
  int index = 0;
  for (int i = 0; i < 3;  i++)

  {
    if (list[i] < min) {

      min = list[i];
      index = i;
    }
  }
  return index;
}

void wait_for_sound()
{
  record_mode = 0;

  /* CA MARCHE PAS LA BOUCLE WHILE */
  while (10*log(analogRead(analogMICPin_1)) < val_sensibility ||
          10*log(analogRead(analogMICPin_2)) < val_sensibility ||
          10*log(analogRead(analogMICPin_3)) < val_sensibility)
    { 
      Serial.println(10*log(analogRead(analogMICPin_3)));
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  
  //record_mode = 1;
}

void record(I2SSampler *input, const char *fname)
{
  record_mode = 1;
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  ESP_LOGI(TAG, "Start recording");
  input->start();
  // open the file on the sdcard
  FILE *fp = fopen(fname, "wb");
  // create a new wave file writer
  WAVFileWriter *writer = new WAVFileWriter(fp, input->sample_rate());
  // keep writing until the user releases the button

  int duree = 0;

  while (record_mode == 1)
  {
    float mic_output[3] = {
    10*log(analogRead(analogMICPin_1)),
    10*log(analogRead(analogMICPin_2)),
    10*log(analogRead(analogMICPin_3))
    };

    int min_index = min(mic_output);
    if(min_index == 0){
      digitalWrite(vib_1, HIGH);
    }
    else{
      if(min_index == 1){
        digitalWrite(vib_2, HIGH);
      }
      else{
        if(min_index == 2){
          digitalWrite(vib_3, HIGH);
        }
      }
    }

    digitalWrite(led, HIGH);
    int samples_read = input->read(samples, 1024);
    int64_t start = esp_timer_get_time();
    writer->write(samples, samples_read);
    int64_t end = esp_timer_get_time();
    ESP_LOGI(TAG, "Wrote %d samples in %lld microseconds", samples_read, end - start);

    duree += end-start;

    if(duree > 5e5){
      record_mode = 0;
    }

  }
  // stop the input
  input->stop();
  // and finish the writing
  writer->finish();
  fclose(fp);
  delete writer;
  free(samples);
  ESP_LOGI(TAG, "Finished recording");
  record_mode = 0;
}

void play(Output *output, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  // open the file on the sdcard
  FILE *fp = fopen(fname, "rb");
  // create a new wave file writer
  WAVFileReader *reader = new WAVFileReader(fp);
  ESP_LOGI(TAG, "Start playing");
  output->start(reader->sample_rate());
  ESP_LOGI(TAG, "Opened wav file");
  // read until theres no more samples
  while (true)
  {
    int samples_read = reader->read(samples, 1024);
    if (samples_read == 0)
    {
      break;
    }
    ESP_LOGI(TAG, "Read %d samples", samples_read);
    output->write(samples, samples_read);
    ESP_LOGI(TAG, "Played samples");
  }
  // stop the input
  output->stop();
  fclose(fp);
  delete reader;
  free(samples);
  ESP_LOGI(TAG, "Finished playing");
}


void main_task(void *param)
{
  ESP_LOGI(TAG, "Starting up");

  ESP_LOGI(TAG, "Mounting SDCard on /sdcard");
  new SDCard("/sdcard", PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_CS);

  ESP_LOGI(TAG, "Creating microphone");
  I2SSampler *input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);
  
  //Output *output = new DACOutput(I2S_NUM_0);
  while (true)
  {
    // wait for the user to push and hold the button
    wait_for_sound();
    record(input, "/sdcard/audio_1.wav");

    digitalWrite(led, LOW);
    digitalWrite (vib_1, LOW);
    digitalWrite (vib_2, LOW);
    digitalWrite (vib_3, LOW);

    /*play(output, "/sdcard/audio_1.wav");
    digitalWrite(led, LOW);
    digitalWrite (vib_1, LOW);
    */
    vTaskDelay(pdMS_TO_TICKS(1000));

  }
}


void setup()
{
  Serial.begin(115200);
  SerialBT.begin("(H)ear-me");

  delay(5000);
  
  pinMode(GPIO_BUTTON, INPUT_PULLUP);

  pinMode(vib_1, OUTPUT);
  pinMode(analogMICPin_1, INPUT);

  pinMode(vib_2, OUTPUT);
  pinMode(analogMICPin_2, INPUT);

  pinMode(vib_3, OUTPUT);
  pinMode(analogMICPin_3, INPUT);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
  xTaskCreate(main_task, "Main", 4096, NULL, 0, NULL);
  
  //pinMode(2, INPUT_PULLUP);
}

void loop()
{
  /*
  int val_analog_mic = analogRead(analogMICPin);
  int log_val_mic = 10*log(val_analog_mic);
  Serial.println(log_val_mic);
  */
}