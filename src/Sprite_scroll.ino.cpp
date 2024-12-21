#include <Arduino.h> // for uint8_t etc. etc.

#define AAAMODS // ST7789 LCD 170 x 320 pixels

#define DEF_TFT_TERMINAL // used in pin_config-t-display-esp32-s3.h
#define MYSERIALX Serial1
#define DEF_SERIAL_DELAY						100
#define BAUD_SERIAL1 (115200)

#if(0)
#define AAAMODS_SCROLL_DOWN
#else
#define AAAMODS_SCROLL_UP
#endif

const uint8_t TEXT_WIDTH = 16; // font plus spacing
const uint8_t TEXT_HEIGHT = 16; // font plus spacing

// The scrolling area must be a integral multiple of TEXT_HEIGHT
//#define TEXT_HEIGHT 16 // Height of text to be printed and scrolled
//const uint8_t TOP_FIXED_AREA = 2; // Number of lines in top fixed area (lines counted from top of screen)
const uint8_t TOP_FIXED_AREA = 0; // Number of lines in top fixed area (lines counted from top of screen)
const uint8_t BOT_FIXED_AREA = 0; // Number of lines in bottom fixed area (lines counted from bottom of screen)
//#define YMAX 320 // Bottom of screen area

//uint16_t yStart = TOP_FIXED_AREA;
byte data = 0;
uint16_t xPos = 0;

//uint16_t yDraw = YMAX - BOT_FIXED_AREA - TEXT_HEIGHT; // The initial y coordinate of the top of the bottom text line


const uint16_t MAX_PIXELS_WIDTH   = 170;
const uint16_t MAX_PIXELS_HEIGHT  = 320;



/*
  Sketch to show scrolling of the graphics in sprites.
  Scrolling in this way moves the pixels in a defined rectangle
  within the Sprite. By defalt the whole sprite is scrolled.
  The gap left by scrolling is filled with a defined colour.

  Example for library:
  https://github.com/Bodmer/TFT_eSPI

  A Sprite is notionally an invisible graphics screen that is
  kept in the processors RAM. Graphics can be drawn into the
  Sprite just as it can be drawn directly to the screen. Once
  the Sprite is completed it can be plotted onto the screen in
  any position. If there is sufficient RAM then the Sprite can
  be the same size as the screen and used as a frame buffer.

  A 16 bit Sprite occupies (2 * width * height) bytes in RAM.

  An 8 bit Sprite occupies (width * height) bytes in RAM.

*/


#ifdef INIDEF_LILYGO_T_DISPLAY_S3
#include "pin_config-t-display-esp32-s3.h" // 8-bit parallel data LCD interface
#endif

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

//TFT_eSprite graph1 = TFT_eSprite(&tft); // Sprite object graph1

TFT_eSprite stext1 = TFT_eSprite(&tft); // Sprite object stext1
TFT_eSprite stextTop = TFT_eSprite(&tft); // Sprite object stextTop

//TFT_eSprite stext2 = TFT_eSprite(&tft); // Sprite object stext2

int graphVal = 1;
int delta = 1;
int grid = 0;
int tcount = 0;

//==========================================================================================
void setup() {

#ifdef DEF_TFT_TERMINAL

  Serial.begin(115200);
	delay(DEF_SERIAL_DELAY); // Need time here?
  
#ifdef MYSERIAL1_BEGIN
	MYSERIAL1_BEGIN;
	delay(DEF_SERIAL_DELAY); // Need time here?
#endif

#endif // AAA_TFT_TERMINAL

 
#ifdef INIDEF_LILYGO_T_DISPLAY_S3
	pinMode(PIN_POWER_ON, OUTPUT);
	digitalWrite(PIN_POWER_ON, HIGH);

	pinMode(PIN_LCD_BL, OUTPUT);
	digitalWrite(PIN_LCD_BL, HIGH);
#endif

  tft.init();

#ifdef AAAMODS // scrolling vertically
  tft.fillScreen(TFT_BLUE);
  //tft.fillScreen(TFT_BLACK);
  graphVal = 1; // initial number to scroll vertically

  // Create a sprite for the top line(s)
  stextTop.setColorDepth(8);
  stextTop.createSprite(MAX_PIXELS_WIDTH, TEXT_HEIGHT); // width, height, frames
  stextTop.fillSprite(TFT_BLACK); // Fill sprite with colour
  stextTop.setTextColor(TFT_WHITE); // White text, no background
  stextTop.setTextDatum(BR_DATUM);  // Bottom right coordinate datum

  stextTop.print("TCM RC SERIAL 2024-12-21");
  stextTop.pushSprite(0, 0); // x, y - insert text at top-left of printing area


  // Create a sprite for the scrolling numbers
  stext1.setColorDepth(8);
  //stext1.createSprite(32, 64); // width, height, frames
  stext1.createSprite(MAX_PIXELS_WIDTH, (MAX_PIXELS_HEIGHT - (TOP_FIXED_AREA * TEXT_HEIGHT))); // width, height, frames
  stext1.fillSprite(TFT_BLUE); // Fill sprite with blue
  //stext1.setScrollRect(0, 0, 32, 64, TFT_BLUE);     // x, y, w. h, color
  stext1.setScrollRect(0, 0, MAX_PIXELS_WIDTH, (MAX_PIXELS_HEIGHT - (TOP_FIXED_AREA * TEXT_HEIGHT)), TFT_BLUE); // x, y, w. h, color
  stext1.setTextColor(TFT_WHITE); // White text, no background
  stext1.setTextDatum(BR_DATUM);  // Bottom right coordinate datum

#else
  tft.fillScreen(TFT_BLACK);

  // Create a sprite for the graph
  graph1.setColorDepth(8);
  graph1.createSprite(128, 61);
  graph1.fillSprite(TFT_BLUE); // Note: Sprite is filled with black when created

  // The scroll area is set to the full sprite size upon creation of the sprite
  // but we can change that by defining a smaller area using "setScrollRect()"if needed
  // parameters are x,y,w,h,color as in drawRect(), the color fills the gap left by scrolling
  //graph1.setScrollRect(64, 0, 64, 61, TFT_DARKGREY);  // Try this line to change the graph scroll area

  // Create a sprite for the scrolling numbers
  stext1.setColorDepth(8);
  stext1.createSprite(32, 64);
  stext1.fillSprite(TFT_BLUE); // Fill sprite with blue
  stext1.setScrollRect(0, 0, 32, 64, TFT_BLUE);     // here we set scroll gap fill color to blue
  stext1.setTextColor(TFT_WHITE); // White text, no background
  stext1.setTextDatum(BR_DATUM);  // Bottom right coordinate datum

  // Create a sprite for Hello World
  stext2.setColorDepth(8);
  stext2.createSprite(80, 16);
  stext2.fillSprite(TFT_DARKGREY);
  stext2.setScrollRect(0, 0, 40, 16, TFT_DARKGREY); // Scroll the "Hello" in the first 40 pixels
  stext2.setTextColor(TFT_WHITE); // White text, no background
#endif // AAAMODS

 
} // setup


//==========================================================================================
void loop() {

#ifdef AAAMODS

  //delay(500); // wait so things do not scroll too fast

  while (MYSERIALX.available()) {
    data = MYSERIALX.read();

    MYSERIALX.write(data); // Perform a remote echo for testing.

    // If it is a CR or we are near end of line then scroll one line
    if (data == '\r' || xPos > (MAX_PIXELS_WIDTH - TEXT_WIDTH)) {
      xPos = 0;
      stext1.setCursor(0,(MAX_PIXELS_HEIGHT - TEXT_HEIGHT));
      //stext1.setCursor(0, 0);
      //stext1.pushSprite(0, 0); // x, y - insert text at top-left of printing area. bot-left ?
      stext1.pushSprite(0, (TOP_FIXED_AREA * TEXT_HEIGHT)); // x, y - insert text at top-left of scrolling area
      stext1.scroll(0, -TEXT_HEIGHT); // dx,dy : scroll stext 0 pixels left/right, 16 up.
    }
    if (data > 31 && data < 128) { // check if data is ASCII printable AAAMAGIC
      //xPos += TEXT_WIDTH;
      //xPos += (TEXT_WIDTH / 2);
      xPos += (TEXT_WIDTH - 10); // AAAFIXME
      //xPos += tft.drawChar(data,xPos,yDraw,2);
      //blank[(18+(yStart-TOP_FIXED_AREA)/TEXT_HEIGHT)%19]=xPos; // Keep a record of line lengths
      stext1.print((char)data);
    }
    //change_colour = 1; // Line to indicate buffer is being emptied

  } // while

  // Draw number in stext1 sprite at 31,63 (bottom right datum set)
  //stext1.drawNumber(graphVal, 31, 63, 2); // plot value in font 2

#if(1)
  //graphVal+=delta;
  //stext1.print(graphVal, 10); // num, base
  //stext1.print(data);
#else
  //stext1.println(graphVal, 10); // num, base
#endif

#ifdef AAAMODS_SCROLL_DOWN
  stext1.setCursor(0,0);
  stext1.pushSprite(0, 0); // x, y - insert text at top-left of printing area
  stext1.scroll(0, +16); // dx,dy : scroll stext 0 pixels left/right, 16 up. Not reqd for println()?
#endif

#ifdef AAAMODS_SCROLL_UP
  //stext1.setCursor(0,(MAX_PIXELS_HEIGHT - TEXT_HEIGHT));
  //stext1.pushSprite(0, 0); // x, y - insert text at top-left of printing area
  //stext1.scroll(0,-8); // dx,dy : scroll stext 0 pixels left/right, 16 up. Not reqd for println()?
#endif





#else

  // Draw point in graph1 sprite at far right edge (this will scroll left later)
  graph1.drawFastVLine(127,60-graphVal,2,TFT_YELLOW); // draw 2 pixel point on graph

  // Draw number in stext1 sprite at 31,63 (bottom right datum set)
  stext1.drawNumber(graphVal, 31, 63, 2); // plot value in font 2

  // Push the sprites onto the TFT at specied coordinates
  graph1.pushSprite(0, 0);
  stext1.pushSprite(0, 64);
  stext2.pushSprite(40, 70);

  // Change the value to plot
  graphVal+=delta;

  // If the value reaches a limit, then change delta of value
  if (graphVal >= 60)     delta = -1;  // ramp down value
  else if (graphVal <= 1) delta = +1;  // ramp up value

  delay(50); // wait so things do not scroll too fast

  // Now scroll the sprites scroll(dt, dy) where:
  // dx is pixels to scroll, left = negative value, right = positive value
  // dy is pixels to scroll, up = negative value, down = positive value
  graph1.scroll(-1, 0); // scroll graph 1 pixel left, 0 up/down
  stext1.scroll(0,-16); // scroll stext 0 pixels left/right, 16 up
  stext2.scroll(1);     // scroll stext 1 pixel right, up/down default is 0

  // Draw the grid on far right edge of sprite as graph has now moved 1 pixel left
  grid++;
  if (grid >= 10)
  { // Draw a vertical line if we have scrolled 10 times (10 pixels)
    grid = 0;
    graph1.drawFastVLine(127, 0, 61, TFT_NAVY); // draw line on graph
  }
  else
  { // Otherwise draw points spaced 10 pixels for the horizontal grid lines
    for (int p = 0; p <= 60; p += 10) graph1.drawPixel(127, p, TFT_NAVY);
  }

  tcount--;
  if (tcount <=0)
  { // If we have scrolled 40 pixels the redraw text
    tcount = 40;
    stext2.drawString("Hello World", 6, 0, 2); // draw at 6,0 in sprite, font 2
  }

#endif // AAAMODS

} // Loop back and do it all again
//==========================================================================================
