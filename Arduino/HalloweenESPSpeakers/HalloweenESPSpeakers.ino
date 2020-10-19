#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

#include "ledeffects.h"
#include <Ticker.h>

//////////// WIRING //////////////
// [PCM5102 DAC > ESP8266/NODEMCU]
// -------------------------------
// VCC  > 3.3
// 3.3V > 3.3
// GND  > GND
// FLT  > GND
// DMP  > GND
// SCL  > GND
// BCK  > GPIO15/D8 (I2S CLOCK)
// DIN  > GPIO3/RX  (I2S DATA)
// LCK  > GPIO2/D4  (L/R CONTROL)
// FMT  > GND
// XMT  > 3.3
//////////////////////////////////

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

const int pirPin = 14;      //D5/GPIO14

float onceGain = 0.0, defaultGain = 0.3;
Ticker randomSound;

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
    (void)cbData;
    Serial.printf("ID3 callback for: %s = '", type);

    if (isUnicode)
    {
        string += 2;
    }

    while (*string)
    {
        char a = *(string++);
        if (isUnicode)
        {
            string++;
        }
        Serial.printf("%c", a);
    }
    Serial.printf("'\n");
    Serial.flush();
}

String readMP3FileNamesfromSPIFFS(void)
{
    Dir dir = SPIFFS.openDir("/");
    String fileNameList[255];
    uint32_t ct = 0;
    while (dir.next())
    {
        String fileName = dir.fileName();
        if (fileName.endsWith(".mp3"))
        {
            //size_t fileSize = dir.fileSize();
            //Serial.printf("FS File: %s, size: %dB\n", fileName.c_str(), fileSize);
            fileNameList[ct] = fileName;
            ct++;
        }
    }
    uint32_t randomPick = RANDOM_REG32 % ct;
    return fileNameList[randomPick];
}

bool stopPlaying(void)
{
    bool stopped = false;
    if (mp3)
    {
        mp3->stop();
        delete mp3;
        mp3 = nullptr;
        stopped = true;
    }
    if (file)
    {
        file->close();
        delete file;
        file = nullptr;
    }
    return stopped;
}

void playMP3(void)
{
    stopPlaying();

    String fileName = readMP3FileNamesfromSPIFFS();

    if (!out)
    {
        out = new AudioOutputI2S();
    }

    out->SetGain(onceGain ?: defaultGain);
    onceGain = 0;

    Serial.println("Playing: " + fileName);

    file = new AudioFileSourceSPIFFS(fileName.c_str());
    mp3 = new AudioGeneratorMP3();
    mp3->begin(file, out);

    ledBlink(50, 0xeb6123);
}

void setup(void)
{
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200);
    pinMode(pirPin, INPUT);
    Serial.setDebugOutput(true);
    delay(1000);
    Serial.println();
    if (SPIFFS.begin())
    {
        Dir dir = SPIFFS.openDir("/");
        while (dir.next())
        {
            String fileName = dir.fileName();
            size_t fileSize = dir.fileSize();
            if (Serial)
                Serial.printf("FS File: %s, size: %dB\n", fileName.c_str(), fileSize);
        }

        FSInfo fs_info;
        SPIFFS.info(fs_info);
        if (Serial)
        {
            Serial.printf("FS Usage: %d/%d bytes\n", fs_info.usedBytes, fs_info.totalBytes);
            Serial.println(F("SPIFFs started"));
            Serial.println(F("---------------------------"));
        }
    }
    Serial.println();
    setupLEDs();

    Serial.printf("Sample MP3 playback begins...\n");
    randomSeed(analogRead(A0));
    audioLogger = &Serial;
}

void loop(void)
{
    if (mp3 && mp3->isRunning())
    {
        if (!mp3->loop())
        {
            mp3->stop();
            ledDisco(50);
            randomSound.attach((float)(random(15, 30)), playMP3);
        }
    }
    else
    {
        if (digitalRead(pirPin) == HIGH)
        {
            randomSound.detach();
            playMP3();
        }
        else
        {
            if (!ledActionInProgress)
            {
                ledDisco(50);
            }
        }
    }

    ledLoop();

}