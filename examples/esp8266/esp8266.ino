/***************************************************
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins.
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#ifdef ESP8266
#define BREAKOUT_RESET  0     // VS1053 reset pin (output)
#define BREAKOUT_CS     5      // VS1053 chip select pin (output)
#define BREAKOUT_DCS    16     // VS1053 Data/command select pin (output)
#else
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
#endif
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

#ifdef ESP8266
// These are common pins between breakout and shield
#define CARDCS 15    // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 4       // VS1053 Data request, ideally an Interrupt pin
#else
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin
#endif

Adafruit_VS1053_FilePlayer musicPlayer =
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
// create shield-example object!
//Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  while (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    delay(500);
  }
  Serial.println("SD OK!");
  //SD.begin(CARDCS);    // initialise the SD card

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }
  Serial.println(F("VS1053 found"));

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20, 20);

  // Do not use interrupts on ESP8266. The feedBuffer function takes too
  // long to execute with interrupts disabled. Calling feedBuffer() from
  // loop() works fine on the ESP8266 probably because it has a much faster
  // CPU than the Uno.
  //
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Start playing a file in the background
  Serial.println(F("Playing track 001"));
  musicPlayer.startPlayingFile("track001.mp3");
}

void loop() {
  // File is playing in the background
  if (musicPlayer.playing()) {
    musicPlayer.feedBuffer();
  }
  if (Serial.available()) {
    char c = Serial.read();

    switch (c) {
      case 's':
        Serial.println(F("Stopped"));
        musicPlayer.stopPlaying();
        break;
      case 'p':
        if (! musicPlayer.paused()) {
          Serial.println("Paused");
          musicPlayer.pausePlaying(true);
        } else {
          Serial.println("Resumed");
          musicPlayer.pausePlaying(false);
        }
        break;
      case '1':
        musicPlayer.stopPlaying();
        Serial.println(F("Playing track 001"));
        musicPlayer.startPlayingFile("track001.mp3");
        break;
      case '2':
        musicPlayer.stopPlaying();
        Serial.println(F("Playing track 002"));
        musicPlayer.startPlayingFile("track002.mp3");
        break;
      default:
        break;
    }
  }
}
