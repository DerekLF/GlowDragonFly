/*
Dont forget to include the eyes and tail in most modes
check all code for comments

*/

#include <FastLED.h>

#define Static 0
#define Static_P 1
#define Travel1 2
#define Travel2 3
#define Heartbeat 4
#define Spiral 5
#define Wing 6

// How many leds in your strip?
#define Num_Leds_Wings 60
#define Num_Leds_Head 6
#define Num_Leds_Tail 60


//To be determined
#define datWingLF 12
#define datWingRF 13
#define datWingLB 14
#define datWingRB 15
#define datHead 16
#define datTail 17

//The 4 wings, might have varying amount of LEDs
CRGB wingLF[Num_Leds_Wings];
CRGB wingRF[Num_Leds_Wings];
CRGB wingLB[Num_Leds_Wings];
CRGB wingRB[Num_Leds_Wings];
CRGB head[Num_Leds_Tail];
CRGB tail[Num_Leds_Head];

//usuable variables
int Modes = 0;
int returnVar = 0;
int arDATA[3];
//Time variables
unsigned long previousMillis = 0;
unsigned long interval = 5000;  //5min delay

void setup() {
  //4 iterations of the declaration, one for each wing, plus one for the head and tail.
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.addLeds<WS2812, datWingLF, RGB>(wingLF, Num_Leds_Wings);  // GRB ordering is typical
  FastLED.addLeds<WS2812, datWingRF, RGB>(wingRF, Num_Leds_Wings);  // GRB ordering is typical
  FastLED.addLeds<WS2812, datWingLB, RGB>(wingLB, Num_Leds_Wings);  // GRB ordering is typical
  FastLED.addLeds<WS2812, datWingRB, RGB>(wingRB, Num_Leds_Wings);  // GRB ordering is typical
  FastLED.addLeds<WS2812, datHead, RGB>(head, Num_Leds_Head);       // GRB ordering is typical
  FastLED.addLeds<WS2812, datTail, RGB>(tail, Num_Leds_Tail);       // GRB ordering is typical
  // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);

  //use mode_Static to display the booting up state
}

void loop() {
  //light starts traveling from the tip of his tail towards his body, it then spreads through his back wings. halfway there it starts in the front wings. and halfway there it travels to the head.
  //where it lights up until the wings have gone off.
  //Mode 0, static.
  //Mode 1, static Plus; dual colour static.
  //Mode 2, travel1; travel tail to head.
  //Mode 3, travel2; body to outside.
  //Mode 4, heartbeat.
  //Mode 5, spiral; lights spirals around it's body
  //Mode 6, wing; light slowly travels away from the center
  switch (Modes) {
    case Static:  //static one colour, only used during startup phase
      returnVar = mode_Static(1, 50);
      break;
    case Static_P:  //dual colour static
      returnVar = mode_Static_P();
      break;
    case Travel1:  //Light travels from tail to head across the wings
      returnVar = mode_Travel_1(0, 0);
      break;
    case Travel2:  //Light travels from the center of the dragonfly to the outside
      returnVar = mode_Travel_2(0, 0);
      break;
    case Heartbeat:  //Butterfly mimics a heartbeat
      returnVar = mode_Heartbeat(0);
      break;
    case Spiral:  //Light spiral across it's body
      returnVar = mode_Spiral(0, 0, 0);
      break;
    case Wing:  //Light slowly travels across it's wings matching it's flapping speed
      returnVar = mode_Wing(0, 0, 0);
      break;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {  //5min interval timer to switch up the lighting effects
    //save the last time you switched modes
    previousMillis = currentMillis;
    if (returnVar == 1) {
      if (Modes == Wing) {
        Modes = 1;
      } else {
        Modes++;
      }
    }
  }
}

/*  Various displaying modes  */

/**************************************************************************/
//Function for showing a single colour. only used for signaling purposes
//Input colour; single colour
//Input brightness; .. .. ..
/**************************************************************************/
int mode_Static(int colour, int brightness) {
  // wingLF[0] = CRGB::Red;
  // wingRF[0] = CRGB(255, 255, 255);
  lightLED(colour, 1, brightness, NULL);

  return 1;
}

/**************************************************************************/
//Function for showing a random dual colour spectacle on the wings, staticly
//Input NULL
/**************************************************************************/
int mode_Static_P() {  //clean this up and merge with lightLED
  int arRGB[3];
  int arDATA[4];
  fetchColourCombo(random(0, 8), arDATA);

  lightLED(arDATA[0], 2, arDATA[2], arRGB);
  for (int i = 0; i < (Num_Leds_Wings / 2); i++) {  //colouring the first half of the wings
    wingLF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingLB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
  }
  lightLED(arDATA[1], 2, arDATA[3], arRGB);
  for (int i = (Num_Leds_Wings / 2); i < Num_Leds_Wings; i++) {  //colouring the second part of the wings
    wingLF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingLB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
  }
  FastLED.show();

  return 1;
}

int mode_Travel_1(int wait, int swoosh) {  //travel from tail tip to head
}
int mode_Travel_2(int wait, int swoosh) {  //travel from body to ends
}

unsigned int pattern[] = { 2, 1, 2, 5 };
unsigned long previousMillisHB = 0;
int i = 0;
/**************************************************************************/
//Function for showing a heartbeat on the wings of the dragonfly
//Input int beepRate; heartrate in multiplications of 10ms
/**************************************************************************/
int mode_Heartbeat(int beepRate) {

  unsigned long currentMillisHB = millis();
  if (currentMillisHB - previousMillisHB >= (pattern[i] * beepRate)) {
    // save the last time you blinked the LED
    previousMillisHB = currentMillisHB;
    if (i < 4) i++;
    else {
      i = 0;
      return 1;
    }
  }
  //output the mode
  switch (i) {
    case 0:  //off
      lightLED(1, 8, 30, NULL);
      break;
    case 1:  //on
      lightLED(1, 3, 80, NULL);
      break;
    case 2:  //off
      lightLED(1, 8, 30, NULL);
      break;
    case 3:  //on
      lightLED(1, 3, 100, NULL);
      break;
  }
  return 0;
}

/**************************************************************************/
//Function for having a light spectacle around the center of the body, including eyes and head
//Input int speed; speed in mS at which colour 1 travels
//Input int colour1; main colour
//Input int colour2; background colour
/**************************************************************************/
int mode_Spiral(int speed, int colour1, int colour2) {
  int arRGB[3];
  int arDATA[4];

  fetchColourCombo(random(0, 8), arDATA);
}

/**************************************************************************/
//Function for having a light spectacle travel on the dragonflies wings
//Input int speed; speed in mS at which colour 1 travels
//Input int colour1; main colour
//Input int colour2; background colour
/**************************************************************************/

//note to self, we might wanne play around with the background brightness


int beginMC = Num_Leds_Wings;
int lengthMC = 5;
unsigned long previousMillisWM = 0;
int mode_Wing(int speed, int colour1, int colour2) {
  int arRGB[3];
  int arDATA[4];

  fetchColourCombo(random(0, 8), arDATA);

  lightLED(arDATA[1], 2, arDATA[3], arRGB);  //applying the background colour
  for (int i = 0; i < Num_Leds_Wings; i++) {
    wingLF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRF[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingLB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    wingRB[i] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
  }

  lightLED(arDATA[0], 2, arDATA[2], arRGB);  //applying the background colour
  //applying the traveling colour
  unsigned long currentMillisWM = millis();
  if (currentMillisWM - previousMillisWM >= speed) {
    previousMillisWM = currentMillisWM;

    beginMC++;
    if (beginMC > Num_Leds_Wings) {
      beginMC = 0;
    }
    for (int i = 0; i < lengthMC; i++) {
      int ledPos = (beginMC + i) % Num_Leds_Wings;  // Calculate the current LED position
      wingLF[ledPos] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
      wingRF[ledPos] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
      wingLB[ledPos] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
      wingRB[ledPos] = CRGB(arRGB[0], arRGB[1], arRGB[2]);
    }
  }
}

/**************************************************************************/
//Function for returning the predetermined colour pairs
//Input int colourpair; Location of the colour pair in the case statement.
//Input int* returnArr; Pointer to array passed along for returning the values.
/**************************************************************************/
int fetchColourCombo(int colourPair, int* returnArr) {
  int innerColour = 0, outerColour = 0, innerBrightness = 0, outerBrightness = 0;
  switch (colourPair) {
    case 0:  // Red and Cyan
      innerColour = 2;
      outerColour = 6;
      innerBrightness = 80;
      outerBrightness = 60;
      break;
    case 1:  //Blue and Orange
      innerColour = 4;
      outerColour = 8;
      innerBrightness = 70;
      outerBrightness = 90;
      break;
    case 2:  //Yellow and Purple
      innerColour = 5;
      outerColour = 9;
      innerBrightness = 80;
      outerBrightness = 70;
      break;
    case 3:  //Green and Pink
      innerColour = 3;
      outerColour = 10;
      innerBrightness = 90;
      outerBrightness = 80;
      break;
    case 4:  //Magenta and Light Blue
      innerColour = 7;
      outerColour = 11;
      innerBrightness = 70;
      outerBrightness = 85;
      break;
    case 5:  //Teal and Brown
      innerColour = 12;
      outerColour = 13;
      innerBrightness = 70;
      outerBrightness = 60;
      break;
    default: break;
  }
  returnArr[0] = innerColour;
  returnArr[1] = outerColour;
  returnArr[2] = innerBrightness;
  returnArr[3] = outerBrightness;
}

/**************************************************************************/
//Function for returning the predetermined colours and/or outputing them
//Input int colorCode; Code to the corresponding colour
//Input int mode; 1 fill solid mode. 2 return RGB value mode.
//Input int brightness; speaks for itself, doesn't it?
//Input int* returnArRGB; Pointer to array passed along the RGB values adjusted with the brightness.
/**************************************************************************/
int lightLED(int colorCode, int mode, int brightness, int* returnArRGB) {
  float G = 0, R = 0, B = 0;

  switch (colorCode) {
    case 0:  // Black
      // G, R, B are already initialized to 0, no need to change them.
      break;
    case 1:  // White
      R = 2.55 * brightness;
      G = 2.55 * brightness;
      B = 2.55 * brightness;
      break;
    // Primary colors
    case 2:  // Red
      R = 2.55 * brightness;
      break;
    case 3:  // Green
      G = 2.55 * brightness;
      break;
    case 4:  // Blue
      B = 2.55 * brightness;
      break;
    // Secondary colors
    case 5:  // Yellow (Red + Green)
      R = 2.55 * brightness;
      G = 2.55 * brightness;
      break;
    case 6:  // Cyan (Green + Blue)
      G = 2.55 * brightness;
      B = 2.55 * brightness;
      break;
    case 7:  // Magenta (Red + Blue)
      R = 2.55 * brightness;
      B = 2.55 * brightness;
      break;
    // Tertiary and other colors
    case 8:  // Orange (Red + less Green)
      R = 2.55 * brightness;
      G = 1.65 * brightness;
      break;
    case 9:  // Purple (Red + Blue, less Green)
      R = 1.60 * brightness;
      G = 0.32 * brightness;
      B = 2.40 * brightness;
      break;
    case 10:  // Pink (Light Red + White tint)
      R = 2.55 * brightness;
      G = 1.92 * brightness;
      B = 2.03 * brightness;
      break;
    case 11:  // Light Blue (Less Red, more Blue)
      R = 1.65 * brightness;
      G = 1.65 * brightness;
      B = 2.55 * brightness;
      break;
    case 12:  // Teal (Cyan with less brightness)
      G = 1.65 * brightness;
      B = 2.55 * brightness;
      break;
    case 13:  // Brown (Darker Orange)
      R = 1.65 * brightness;
      G = 1.32 * brightness;
      B = 0.80 * brightness;
      break;
    case 14:  // Gray
      R = 1.80 * brightness;
      G = 1.80 * brightness;
      B = 1.80 * brightness;
      break;
    default:
      // Handle unexpected color codes, reset to Black
      R = 0;
      G = 0;
      B = 0;
      break;
  }

  if (mode == 1) {
    fill_solid(wingLF, Num_Leds_Wings, CRGB(G, R, B));
    fill_solid(wingRF, Num_Leds_Wings, CRGB(G, R, B));
    fill_solid(wingLB, Num_Leds_Wings, CRGB(G, R, B));
    fill_solid(wingRB, Num_Leds_Wings, CRGB(G, R, B));
    fill_solid(head, Num_Leds_Head, CRGB(G, R, B));
    fill_solid(tail, Num_Leds_Tail, CRGB(G, R, B));
    FastLED.show();
  } else if (mode == 2) {
    returnArRGB[0] = G;
    returnArRGB[1] = R;
    returnArRGB[2] = B;
  }
}
