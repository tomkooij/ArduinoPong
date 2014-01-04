// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#include <Wire.h>
#include <ArduinoNunchuk.h>

#define BAUDRATE 19200

ArduinoNunchuk nunchuk = ArduinoNunchuk();

#define PIN 6

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

#define ROOD 0
#define GROEN 1
#define BLAUW 2

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

int x,y;

void setup() {
  
  // nunchuck aangesloten op SCLK/SDA/AREF/GND aan de kant van de RST van de Arduino UNO
  Serial.begin(BAUDRATE);
  Serial.write("Hello... now setting up the Nunchuck (port A4/A5)!\n")
  
  nunchuk.init();
  Serial.write("Init done!\n ");

  // bitmap (framebuffer) setup
  matrix.begin();
  matrix.setBrightness(10);
  
  // startpositie piel
  x = 2;
  y = 2;
  
}


void loop() {
   
    nunchuk.update();

    Serial.print(nunchuk.analogX, DEC);
    Serial.print(' ');
    Serial.println(nunchuk.analogY, DEC);
 
     // x is de korte zijde x = 0 is aan de reset/pwr kant
     // y is de lange zijde y = 0 is aan de reset kant
     if (nunchuk.analogX<40) {  // links
          if (y==0) y = 7;
          y--;
     }
      
     if (nunchuk.analogX>150) { // rechts
          if (y==7) y = 0;
          y++;
     }
     
     if (nunchuk.analogY<40) {  // omlaag
          if (x==0) x = 4;
          x--;
     }
      
     if (nunchuk.analogY>150) { // omhoog
          if (x==4) x = 0;
          x++;
     }
     
     Serial.write("coordinaten ");
     Serial.print(x, DEC);
     Serial.println(y, DEC); 
          
          matrix.fillScreen(colors[BLAUW]);
          matrix.drawPixel(x,y,colors[ROOD]);
          matrix.show();
          delay(40);
  
}
