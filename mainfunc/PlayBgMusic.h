#define AUDIO_DEBUG true 

uint8_t channelSelection = 1;

void loadBuffer();

/*********************************************************/

void DACC_Handler(void) {
  forSD();
  loadBuffer();
  forLCD();
  aaAudio.feedDAC(channelSelection);      //Feed the DAC with the data loaded into the dacBuffer
  aaAudio.dacHandler();                   //Link the DAC ISR/IRQ to the library. Called by the MCU when DAC is ready for data
}

/*********************************************************/
/* Function to open the audio file, seek to starting position and enable the DAC */

void playAudio(char* audioFile) {

  uint32_t sampleRate = 8000;
  uint16_t numChannels = 1;
  uint16_t bitsPerSample = 8;
  uint32_t dataSize = 0;
  uint32_t startPosition = 44;

  if (myFile_bgm) {
    aaAudio.disableDAC();
    myFile_bgm.close();
    //delay(25);
  }

  //Open the designated file
  path_for_bgm = SD.open(audioFile);
  myFile_bgm = path_for_bgm.openNextFile();
  if (myFile_bgm) {
    myFile_bgm.seek(22);
    myFile_bgm.read((byte*)&numChannels, 2);
    myFile_bgm.read((byte*)&sampleRate, 4);
    myFile_bgm.seek(34);
    myFile_bgm.read((byte*)&bitsPerSample, 2);
    myFile_bgm.seek(40);
    myFile_bgm.read((byte*)&dataSize, 4);
    dataSize += 44; //Set this variable to the total size of header + data

#if defined (AUDIO_DEBUG)
    Serial.print("\nNow Playing ");
    Serial.println(audioFile);
    Serial.print("Channels ");
    Serial.print(numChannels);
    Serial.print(", SampleRate ");
    Serial.print(sampleRate);
    Serial.print(", BitsPerSample ");
    Serial.println(bitsPerSample);
#endif

    if (myFile_bgm.size() > dataSize) {
      startPosition = myFile_bgm.size() - dataSize;
      Serial.println("Skipping metadata");
    }
    Serial.println(bitsPerSample);
    if (bitsPerSample > 10 ) {
      bitsPerSample = 12;
    } else if (bitsPerSample > 8) {
      bitsPerSample = 10;
    } else {
      bitsPerSample = 8;
    }

    sampleRate *= numChannels;
    aaAudio.dacBitsPerSample = bitsPerSample;
    aaAudio.setSampleRate(sampleRate);

#if defined (AUDIO_DEBUG)
    Serial.print("Timer Rate ");
    Serial.print(sampleRate);
    Serial.print(", DAC Bits Per Sample ");
    Serial.println(bitsPerSample);
#endif

    //Skip past the WAV header
    myFile_bgm.seek(startPosition);
    //Load one buffer
    Serial.println("Calling loadbuffer()");
    loadBuffer();
    Serial.println("After calling loadbuffer()");
    //Feed the DAC to start playback
    aaAudio.feedDAC(channelSelection);
    Serial.println("After calling feedDAC()");
  } else {
#if defined (AUDIO_DEBUG)
    Serial.print("Failed to open ");
    Serial.println(audioFile);
#endif
  }
}
/*********************************************************/
/* Function to open the audio file, seek to starting position and enable the DAC */

void playSoundTrack(char* audioFile) {

  uint32_t sampleRate = 16000;
  uint16_t numChannels = 1;
  uint16_t bitsPerSample = 8;
  uint32_t dataSize = 0;
  uint32_t startPosition = 44;

  if (myFile_sem) {
    //aaAudio.disableDAC();
    myFile_sem.close();
    //delay(25);
  }

  //Open the designated file
  myFile_sem = SD.open(audioFile);
  if (myFile_sem) {
    myFile_sem.seek(22);
    myFile_sem.read((byte*)&numChannels, 2);
    myFile_sem.read((byte*)&sampleRate, 4);
    myFile_sem.seek(34);
    myFile_sem.read((byte*)&bitsPerSample, 2);
    myFile_sem.seek(40);
    myFile_sem.read((byte*)&dataSize, 4);
    dataSize += 44; //Set this variable to the total size of header + data

#if defined (AUDIO_DEBUG)
    Serial.print("\nNow Playing ");
    Serial.println(audioFile);
    Serial.print("Channels ");
    Serial.print(numChannels);
    Serial.print(", SampleRate ");
    Serial.print(sampleRate);
    Serial.print(", BitsPerSample ");
    Serial.println(bitsPerSample);
#endif

    if (myFile_sem.size() > dataSize) {
      startPosition = myFile_sem.size() - dataSize;
      Serial.println("Skipping metadata");
    }

    if (bitsPerSample > 10 ) {
      bitsPerSample = 12;
    } else if (bitsPerSample > 8) {
      bitsPerSample = 10;
    } else {
      bitsPerSample = 8;
    }

    sampleRate *= numChannels;
    aaAudio.dacBitsPerSample = bitsPerSample;
    aaAudio.setSampleRate(sampleRate);

#if defined (AUDIO_DEBUG)
    Serial.print("Timer Rate ");
    Serial.print(sampleRate);
    Serial.print(", DAC Bits Per Sample ");
    Serial.println(bitsPerSample);
#endif

    //Skip past the WAV header
    myFile_sem.seek(startPosition);
    //Feed the DAC to start playback
    aaAudio.feedDAC();
  } else {
#if defined (AUDIO_DEBUG)
    Serial.print("Failed to open ");
    Serial.println(audioFile);
#endif
  }
}
/*********************************************************/
/* Function called from DAC interrupt after dacHandler(). Loads data into the dacBuffer */

void loadBuffer() {
  if (myFile_bgm) {
    if (myFile_bgm.available() && myFile_sem.available()) {
      if (aaAudio.dacBitsPerSample == 8) {
        uint8_t dacBuffForbgm[MAX_BUFFER_SIZE], dacBuffForsem[MAX_BUFFER_SIZE];
        myFile_bgm.read((byte*)dacBuffForbgm, MAX_BUFFER_SIZE);
        myFile_sem.read((byte*)dacBuffForsem, MAX_BUFFER_SIZE);
        for(int i = 0; i < MAX_BUFFER_SIZE; i++){
          aaAudio.dacBuffer[i] = ((dacBuffForbgm[i]-127)+(dacBuffForsem[i]-127))/2+127;
        }
      }
    }
    else if(myFile_bgm.available()){
      if (aaAudio.dacBitsPerSample == 8) {
        //Load 32 samples into the 8-bit dacBuffer
        myFile_bgm.read((byte*)aaAudio.dacBuffer, MAX_BUFFER_SIZE);
      }
    }
    else {//файл закончился;
      myFile_bgm = path_for_bgm.openNextFile();
      if (!myFile_bgm) {
        Serial.println("Hello");
        path_for_bgm = SD.open("/game/music/");
        myFile_bgm = path_for_bgm.openNextFile();
      }
    }
  }
}

/*********************************************************/
