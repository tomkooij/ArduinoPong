//
// NeoPixel Pong
//
// Tom Kooij

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

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(40, PIN, NEO_GRB + NEO_KHZ800);


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
#define WIT 3
#define ZWART 4

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255,255,255), matrix.Color(0,0,0) };

// bal positie
int x,y;
// bal snelheid
int vx, vy;
// x positie (korte as) van batje
int x_bat_links;
int x_bat_rechts;

// Bron: Adafruit NeoPixel example (strip)
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


void setup() {
  
  // intro
  strip.begin();
  colorWipe(strip.Color(0, 0, 20), 50); // Blauw 20/255 brightness

  // nunchuck aangesloten op SCLK/SDA/AREF/GND aan de kant van de RST van de Arduino UNO
  Serial.begin(BAUDRATE);
  Serial.write("Hello... now setting up the Nunchuck (port A4/A5)!\n");
  
  nunchuk.init();
  Serial.write("Init done!\n ");

  // bitmap (framebuffer) setup
  matrix.begin();
  matrix.setBrightness(10);  
  x_bat_links=1;
  x_bat_rechts=2;
}

int c,k, score;

void loop() {
  
  // startpositie 
  x = 2;
  y = 3;
  vx = -1;
  vy = 1;
 
//  score = 0;
  
  // intro
  
  // spel loop
  while (1) {
    
    nunchuk.update();

    Serial.print(nunchuk.analogX, DEC);
    Serial.print(' ');
    Serial.println(nunchuk.analogY, DEC);

/* oude code om balletje te sturen 
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
*/     
     
     // verplaatsing
     x += vx;
     y += vy;
     
     // controleer of het batje op de juiste plaats staat. JA: weerkaats. NEE: verlaat spel
     if (y>=7) { if ((x_bat_rechts==x) || ((x_bat_rechts+1)==x))  { vy = -vy; score++; } else break; }
     if (y<=0) { if ((x_bat_links==x) || ((x_bat_links+1)==x))  { vy = -vy; score++; } else break; }

     // weerkaats tegen de muren
     if (x>=4) vx = -vx;
     if (x<=0) vx = -vx;
     
     // achtergrond     
     matrix.fillScreen(colors[BLAUW]);
    
     // bal
     matrix.drawPixel(x,y,colors[ROOD]);
     
     // batjes
     matrix.drawPixel(x_bat_links,0,colors[GROEN]);
     matrix.drawPixel(x_bat_links+1,0,colors[GROEN]);
     matrix.drawPixel(x_bat_rechts,7,colors[GROEN]);
     matrix.drawPixel(x_bat_rechts+1,7,colors[GROEN]);
     
     // OUTPUT!
     matrix.show();
     delay(200);
    
  }     
     
  // laat rood scherm zien en stop
  matrix.fillScreen(colors[ROOD]);
  matrix.show();
  
  // scorebord in binair
  for (c = 5; c >= 0; c--)
  {
    k = score >> c;
 
    if (k & 1)
      matrix.drawPixel(2,6-c,colors[WIT]);  // "1" is wit 
    else
      matrix.drawPixel(2,6-c,colors[ZWART]);;  // "0" is zwart
    
    delay(200);
    matrix.show();
  }
 
  delay(4000);
            
  // demo de neopixels          
  //rainbowCycle(20) ;
 
 
  // start het spel opnieuw  
  x_bat_links++;
  x_bat_rechts--;
  if (x_bat_links>=4) x_bat_links=0;
  if (x_bat_rechts<=0) x_bat_rechts=3;
}
