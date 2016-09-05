## Add ESP8266

Add support for ESP8266. Interrupts are not used because the ISR takes too much
time resulting in WDT crashes. Instead loop() must call feedBuffer() as often
as possible. Add ESP8266 example.

Add vendor patches to add FLAC playback.

## Original text
  This is a library for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
