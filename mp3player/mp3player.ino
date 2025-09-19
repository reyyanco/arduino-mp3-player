#include <SD.h>
#include <TMRpcm.h>


#define SD_CS_PIN 4

#define SPEAKER_PIN 9

TMRpcm audio;
File root;
char files[20][13]; 
int fileCount = 0;
int currentFileIndex = 0;

void setup() {
  Serial.begin(9600);
  
  audio.speakerPin = SPEAKER_PIN; 

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD Kart başlatılamadı!");
    while (1); 
  }
  Serial.println("SD Kart başlatıldı.");
  
  root = SD.open("/");
  
  Serial.println("Müzik dosyaları aranıyor...");
  while(true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      String fileName = entry.name();
      fileName.toUpperCase();
      
      
      if (fileName.endsWith(".MP3")) {
        fileName.toCharArray(files[fileCount], sizeof(files[fileCount]));
        Serial.print("Dosya bulundu: ");
        Serial.println(files[fileCount]);
        fileCount++;
      }
    }
    entry.close();
  }
  
  root.close();
  
  if (fileCount > 0) {
    Serial.print("İlk dosya çalınıyor: ");
    Serial.println(files[0]);
    audio.play(files[0]);
  } else {
    Serial.println("SD kartta MP3 dosyası bulunamadı.");
  }
}

void loop() {
  if (fileCount > 0) {
    if (!audio.isPlaying()) {
      delay(1000); 

      currentFileIndex++;
      if (currentFileIndex >= fileCount) {
        currentFileIndex = 0;
      }
      
      Serial.print("Sıradaki dosya çalınıyor: ");
      Serial.println(files[currentFileIndex]);
      audio.play(files[currentFileIndex]);
    }
  }
}