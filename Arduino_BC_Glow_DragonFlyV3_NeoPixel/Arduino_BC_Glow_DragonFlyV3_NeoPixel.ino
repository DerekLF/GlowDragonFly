/*
Dont forget to include the eyes and tail in most modes
check all code for comments

*/
#define DEBUG 1
/*            DEBUG instructions            */
// DEBUG 0 -- Mainly for finished product use, so no debugging info is printed.
// DEBUG 1 -- Basic info printed to show the main steps of the program.
// DEBUG 2 -- Detailed info printed to show what each function is doing.

#define MCU ESP
/*  just for me   */
// MCU -- ESP
// MCU -- MEGA
#define synchronization

#ifdef synchronization

#endif

#include <Adafruit_NeoPixel.h>

#define LED_TYPE NEO_GRBW  //LED configuration used

#define Static 0
#define Static_P 1
#define Travel1 2
#define Travel2 3
#define Heartbeat 4
#define Spiral 5
#define Wing 6

// How many leds in the body parts?
#define Num_Leds_Wings 34
#define Num_Leds_Head 6
#define Num_Leds_Tail 31

//for conveniently switching MCUs
#if MCU == ESP
#define datWingLF 5
#define datWingRF 6
#define datWingLB 9
#define datWingRB 10
#define datHead 3
#define datTail 11
#elif MCU == MEGA
#define datWingLF 35
#define datWingRF 32
#define datWingLB 33
#define datWingRB 25
#define datHead 34
#define datTail 26
#endif

//The 4 wings, might have varying amount of LEDs
Adafruit_NeoPixel wingLF = Adafruit_NeoPixel(Num_Leds_Wings, datWingLF, LED_TYPE);
Adafruit_NeoPixel wingRF = Adafruit_NeoPixel(Num_Leds_Wings, datWingRF, LED_TYPE);
Adafruit_NeoPixel wingLB = Adafruit_NeoPixel(Num_Leds_Wings, datWingLB, LED_TYPE);
Adafruit_NeoPixel wingRB = Adafruit_NeoPixel(Num_Leds_Wings, datWingRB, LED_TYPE);
Adafruit_NeoPixel head = Adafruit_NeoPixel(Num_Leds_Head, datHead, LED_TYPE);
Adafruit_NeoPixel tail = Adafruit_NeoPixel(Num_Leds_Tail, datTail, LED_TYPE);


//usable variables
int Modes = 3;
int returnVar = 0;
int arDATALoop[4];
int randColor = 0, randColorBuf = 0;

//Time variables
unsigned long previousMillis = 0;
unsigned long interval = 1000 * 10;  //5min delay

void setup() {
  //initialize all the LEDstrips
  wingLF.begin();
  wingRF.begin();
  wingLB.begin();
  wingRB.begin();
  head.begin();
  tail.begin();

  //start with a blank slate
  wingLF.clear();
  wingRF.clear();
  wingLB.clear();
  wingRB.clear();
  head.clear();
  tail.clear();

  delay(50);



  Serial.begin(9600);
//ESP_NOW setup
#ifdef synchronization

#endif
  delay(1000);
  fetchColourCombo(random(32), arDATALoop);
  //Implement Jaspers crazy ass idea of synchronization
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
                  //Works fine
      returnVar = mode_Static(1, 100);
      break;
    case Static_P:  //dual colour static
                    //Works fine, except for there being no delay. So add blink without delay to it
      returnVar = mode_Static_P(arDATALoop);
      break;
    case Travel1:  //Light travels from tail to head across the wings
                   //works perfectly, takes 8.3 seconds per cycle
      returnVar = mode_Travel_1(50, arDATALoop);
      break;
    case Travel2:  //Light travels from the center of the dragonfly to the outside
      returnVar = mode_Travel_2(2000, arDATALoop);
      break;
    case Heartbeat:  //Butterfly mimics a heartbeat
                     //feels off, make it feel more like a heartbeat, you know. with the LEDs slowly lighting up.
                     //also the heartrate doesn't really work that well (60bps is faster then 120bps)
      returnVar = mode_Heartbeat(120);
      break;
    case Spiral:  //Light spiral across it's body
      returnVar = mode_Spiral(0, arDATALoop);
      break;
    case Wing:  //Light slowly travels across it's wings matching it's flapping speed
      returnVar = mode_Wing(100, arDATALoop);
      break;
  }
  wingLF.show();
  wingRF.show();
  wingLB.show();
  wingRB.show();
  head.show();
  tail.show();

  if (returnVar == 1) {
    while (randColor == randColorBuf) {
      randColor = random(32);
    }
    randColorBuf = randColor;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval && returnVar == 1) {  //5min interval timer to switch up the lighting effects
    //save the last time you switched modes
    previousMillis = currentMillis;

#if DEBUG > 0
    Serial.println("Fetching a new colorcombo");
#endif
    fetchColourCombo(randColor, arDATALoop);
    returnVar = 0;


    /*if (returnVar == 1) {
      if (Modes == Wing) {
        Modes = 1;
      } else {
        Modes++;
      }
    }*/
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
//Function for showing a random dual + blend colour spectacle on the wings, staticly
//Input NULL
/**************************************************************************/
int mode_Static_P(int arDATA[4]) {  //clean this up and merge with lightLED
  int arRGB[3];
  int subColRGB[3];

  lightLED(arDATA[0], 2, arDATA[2], arRGB);  //fetching the background color
                                             //filling the color in
  wingLF.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  wingRF.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  wingLB.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  wingRB.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  head.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  tail.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));

  blendColorsRGBW(arDATA, subColRGB);                                    //fetching the blend of the 2 colors
  for (int i = (Num_Leds_Wings / 3); i < Num_Leds_Wings * 2 / 3; i++) {  //colouring the second part of the wings
    wingLF.setPixelColor(i, wingLF.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
    wingRF.setPixelColor(i, wingRF.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
    wingLB.setPixelColor(i, wingLB.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
    wingRB.setPixelColor(i, wingRB.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
  }
  for (int i = (Num_Leds_Head / 3); i < Num_Leds_Head * 2 / 3; i++) {  //colouring the second part of the head
    head.setPixelColor(i, head.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
  }
  for (int i = (Num_Leds_Tail / 3); i < Num_Leds_Tail * 2 / 3; i++) {  //colouring the second part of the tail
    tail.setPixelColor(i, tail.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
  }

  lightLED(arDATA[1], 2, arDATA[3], arRGB);                          //fetching the background accent color
  for (int i = (Num_Leds_Wings * 2 / 3); i < Num_Leds_Wings; i++) {  //colouring the second part of the wings
    wingLF.setPixelColor(i, wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingRF.setPixelColor(i, wingRF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingLB.setPixelColor(i, wingLB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingRB.setPixelColor(i, wingRB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  }
  for (int i = (Num_Leds_Head * 2 / 3); i < Num_Leds_Head; i++) {  //colouring the second part of the head
    head.setPixelColor(i, head.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  }
  for (int i = (Num_Leds_Tail * 2 / 3); i < Num_Leds_Tail; i++) {  //colouring the second part of the tail
    tail.setPixelColor(i, tail.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  }

  return 1;
}
/**************************************************************************/
//Function for light traveling from tail to head.
//Input intervalT1, interval time during the traveling of the LEDs
/**************************************************************************/
int travelLength = 0, position = 0;
unsigned long previousMillisT1 = 0;
int iT1 = 0, state = 0;
int PulseWidthT1 = 6;
int returnValT1 = 0;
int mode_Travel_1(int intervalT1, int arDATA[4]) {  //travel from tail tip to head
  returnValT1 = 0;
  int arRGB[3];
  int subColRGB[3];
  unsigned long currentMillisT1 = millis();
  if (currentMillisT1 - previousMillisT1 >= intervalT1) {
    previousMillisT1 = currentMillisT1;

    lightLED(arDATA[0], 1, arDATA[2], NULL);
    blendColorsRGBW(arDATA, subColRGB);  //fetching the perfect blend
    switch (state) {
      case 0:
        lightLED(arDATA[1], 2, arDATA[3], arRGB);

        break;
      case 1:  // Tail; end to tip
        travelLength = Num_Leds_Tail;
        for (int i = PulseWidthT1; i > 0; i--) {
          int pos = travelLength - (position - (PulseWidthT1 / 2) + i);
          for (int j = 0; j < PulseWidthT1 / 2; j++) {
            tail.setPixelColor(pos - j, tail.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
            tail.setPixelColor(pos + j, tail.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
          }
          tail.setPixelColor(pos, tail.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
        }
        position++;
        break;  //needs to be inverted, from end to body

      case 2:  // Back wings; begin to end, both sides
        travelLength = Num_Leds_Wings;
        for (int i = 0; i < PulseWidthT1; i++) {
          int pos = position - (PulseWidthT1 / 2) + i;
          for (int j = 0; j < PulseWidthT1 / 2; j++) {
            wingLB.setPixelColor(pos - j, wingLB.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
            wingRB.setPixelColor(pos + j, wingRB.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
          }
          wingLB.setPixelColor(pos, wingLB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
          wingRB.setPixelColor(pos, wingRB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
        }
        position++;
        break;

      case 3:  // Front wings; begin to end, both sides
        travelLength = Num_Leds_Wings;
        for (int i = 0; i < PulseWidthT1; i++) {
          int pos = position - (PulseWidthT1 / 2) + i;
          for (int j = 0; j < PulseWidthT1 / 2; j++) {
            wingLF.setPixelColor(pos - j, wingLF.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
            wingRF.setPixelColor(pos + j, wingRF.Color(subColRGB[0], subColRGB[1], subColRGB[2], subColRGB[3]));
          }
          wingLF.setPixelColor(pos, wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
          wingRF.setPixelColor(pos, wingRF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
        }
        position++;
        break;
        /****************************************************/
        /*              This needs to be fixed              */
        /****************************************************/
      case 4:  //head; light up both eyes //change to 3 colour move; base > blend > accent
        head.fill(head.Color(arRGB[0] - iT1, arRGB[1] - iT1, arRGB[2] - iT1, arRGB[3]));
        if (iT1 >= 200) {
          position = travelLength * 2;
          iT1 = 0;
        } else iT1 += 25;

        break;
    }
    /****************************************************/
    /*              This needs to be fixed              */
    /****************************************************/
    if (position >= travelLength + PulseWidthT1 || state == 0) {
      position = 0;  // - (PulseWidth / 2);
      state++;
      if (state >= 5) {
        state = 0;
        returnValT1 = 1;
      }
    }
  }
  return returnValT1;
}
//maybe give it 2 states before finishing, switching between front colour followed by background colour
int PulseWidthT2 = 10;
int positionT2W = 0, positionT2T = 0;
unsigned long previousMillisT2W = 0, previousMillisT2T = 0, previousMillisT2H = 0;
int run2 = 0;
int mode_Travel_2(int interval, int arDATA[4]) {  //travel from body to ends
  int returnValT2 = 0;
  int arRGB[3];

  if (run2 == 0) {
    lightLED(arDATA[1], 2, arDATA[3], arRGB);  //fetching the initial colour
  } else {
    lightLED(arDATA[0], 2, arDATA[2], arRGB);  //run the second colour
  }

  unsigned long currentMillisT2 = millis();

  if (currentMillisT2 - previousMillisT2H >= (interval / Num_Leds_Head)) {  //Head
    previousMillisT2H = currentMillisT2;
    head.fill(head.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
  }
  if (currentMillisT2 - previousMillisT2T >= (interval / (Num_Leds_Tail + PulseWidthT2))) {  //Tail
    previousMillisT2T = currentMillisT2;
    for (int i = 0; i < PulseWidthT2; i++) {
      int posT = positionT2T - PulseWidthT2 + i;
      tail.setPixelColor(posT, tail.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    }
    positionT2T++;
  }
  if (currentMillisT2 - previousMillisT2W >= (interval / (Num_Leds_Wings + PulseWidthT2))) {  //Wings
    previousMillisT2W = currentMillisT2;
    for (int i = 0; i < PulseWidthT2; i++) {
      int posW = positionT2W - PulseWidthT2 + i;
      wingLF.setPixelColor(posW, wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingRF.setPixelColor(posW, wingRF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingLB.setPixelColor(posW, wingLB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingRB.setPixelColor(posW, wingRB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    }

    positionT2W++;
  }
#if DEBUG > 1
  Serial.print("posT2Wing : ");
  Serial.println(positionT2W);
  Serial.print("posT2Tail : ");
  Serial.println(positionT2T);
#endif
  if (positionT2W >= Num_Leds_Wings && positionT2T >= Num_Leds_Tail) {
    positionT2W = 0;
    positionT2T = 0;
    if (run2 == 0) {
      run2 = 1;
    } else {
      run2 = 0;
      returnValT2 = 1;
    }
  }

  return returnValT2;
}

unsigned long previousMillisHB = 0;
unsigned int minToMil = 60000;
int hbJumps = 50;  // Adjust as needed for smoothness
int hbBrightness = 0;
bool hbDir = false;

/**************************************************************************/
// Function for showing a heartbeat on the wings of the dragonfly
// Input: int BPM; heartrate in beats per minute
/**************************************************************************/
int mode_Heartbeat(int BPM) {

  unsigned long currentMillisHB = millis();

  if ((currentMillisHB - previousMillisHB) >= (minToMil / BPM / (hbJumps * 2))) {
    previousMillisHB = currentMillisHB;  // Save the last time we changed brightness
// Debug output for monitoring
#if DEBUG > 1
    Serial.print("Mode - Wing");
    Serial.print(": Brightness ");
    Serial.println(hbBrightness);
#endif
    // Set brightness for all parts
    wingLF.fill(wingLF.Color(hbBrightness, 0, 0, 0));
    wingRF.fill(wingRF.Color(hbBrightness, 0, 0, 0));
    wingLB.fill(wingLB.Color(hbBrightness, 0, 0, 0));
    wingRB.fill(wingRB.Color(hbBrightness, 0, 0, 0));

    head.fill(head.Color(hbBrightness, 0, 0, 0));
    tail.fill(tail.Color(hbBrightness, 0, 0, 0));

    // Update brightness with direction control
    if (!hbDir) {
      hbBrightness += 5;
      if (hbBrightness >= 250) hbDir = true;  // Change direction when max brightness reached
    } else {
      hbBrightness -= 5;
      if (hbBrightness <= 0) hbDir = false;  // Change direction when min brightness reached
    }
  }

  // Return 1 when brightness is back to 0, otherwise 0
  if (hbBrightness == 0) return 1;
  else return 0;
}


/**************************************************************************/
//Function for having a light spectacle around the center of the body, including eyes and head
//Input int speed; speed in mS at which colour 1 travels
//Input int colour1; main colour
//Input int colour2; background colour
/**************************************************************************/
int mode_Spiral(int speed, int arDATA[4]) {
  int arRGB[3];
}

/**************************************************************************/
//Function for having a light spectacle travel on the dragonflies wings
//Input int speed; speed in mS at which colour 1 travels
//Input int colour1; main colour
//Input int colour2; background colour
/**************************************************************************/

//note to self, we might wanne play around with the background brightness


int beginMC = Num_Leds_Wings;
int lengthMC = Num_Leds_Wings / 2;
int returnValW = 0;
unsigned long previousMillisWM = 0;
int mode_Wing(int speed, int arDATA[4]) {
  int arRGB[3];
  returnValW = 0;



  //applying the traveling colour
  unsigned long currentMillisWM = millis();
  if (currentMillisWM - previousMillisWM >= speed) {
    previousMillisWM = currentMillisWM;
    lightLED(arDATA[1], 2, arDATA[3], arRGB);  //applying the background colour

    wingLF.fill(wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingRF.fill(wingRF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingLB.fill(wingLB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    wingRB.fill(wingRB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));


    lightLED(arDATA[0], 2, arDATA[2], arRGB);  //applying the background colour
    beginMC++;
    if (beginMC > Num_Leds_Wings) {
      beginMC = 0;
      returnValW = 1;
    }
    for (int i = 0; i < lengthMC; i++) {
      int ledPos = (beginMC + i) % Num_Leds_Wings;  // Calculate the current LED position
      wingLF.setPixelColor(ledPos, wingLF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingRF.setPixelColor(ledPos, wingRF.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingLB.setPixelColor(ledPos, wingLB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
      wingRB.setPixelColor(ledPos, wingRB.Color(arRGB[0], arRGB[1], arRGB[2], arRGB[3]));
    }
  }
}

/**************************************************************************/
//Function for returning the predetermined colour pairs
//Input int colourpair; Location of the colour pair in the case statement.
//Input int* returnArr; Pointer to array passed along for returning the values.
/**************************************************************************/
int blendColorsRGBW(int* colorArr, int* arrRGBWt) {
  int arrRGBW1[3];
  int arrRGBW2[3];
  lightLED(colorArr[0], 2, colorArr[2], arrRGBW1);
  lightLED(colorArr[1], 2, colorArr[3], arrRGBW2);
  for (int i = 0; i < 4; i++) {
    arrRGBWt[i] = (arrRGBW1[i] + arrRGBW2[i]) / 2;
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
#if DEBUG > 0
      Serial.println("Inner Red - Outer Cyan");
#endif
      innerColour = 2;       // Red
      outerColour = 6;       // Cyan
      innerBrightness = 80;  // Inner brightness for Red
      outerBrightness = 60;  // Outer brightness for Cyan
      break;

    case 1:  // Green and Magenta
#if DEBUG > 0
      Serial.println("Inner Green - Outer Magenta");
#endif
      innerColour = 3;       // Green
      outerColour = 7;       // Magenta
      innerBrightness = 80;  // Inner brightness for Green
      outerBrightness = 60;  // Outer brightness for Magenta
      break;

    case 2:  // Blue and Yellow
#if DEBUG > 0
      Serial.println("Inner Blue - Outer Yellow");
#endif
      innerColour = 4;       // Blue
      outerColour = 5;       // Yellow
      innerBrightness = 80;  // Inner brightness for Blue
      outerBrightness = 60;  // Outer brightness for Yellow
      break;

    case 3:  // Orange and Teal
#if DEBUG > 0
      Serial.println("Inner Orange - Outer Teal");
#endif
      innerColour = 8;       // Orange
      outerColour = 13;      // Teal
      innerBrightness = 80;  // Inner brightness for Orange
      outerBrightness = 60;  // Outer brightness for Teal
      break;

    case 4:  // Pink and Light Blue
#if DEBUG > 0
      Serial.println("Inner Pink - Outer Light Blue");
#endif
      innerColour = 10;      // Pink
      outerColour = 12;      // Light Blue
      innerBrightness = 80;  // Inner brightness for Pink
      outerBrightness = 60;  // Outer brightness for Light Blue
      break;

    case 5:  // Gold and Charcoal
#if DEBUG > 0
      Serial.println("Inner Gold - Outer Charcoal");
#endif
      innerColour = 11;      // Gold
      outerColour = 22;      // Charcoal
      innerBrightness = 80;  // Inner brightness for Gold
      outerBrightness = 60;  // Outer brightness for Charcoal
      break;

    case 6:  // Lavender and Dark Blue
#if DEBUG > 0
      Serial.println("Inner Lavender - Outer Dark Blue");
#endif
      innerColour = 14;      // Lavender
      outerColour = 29;      // Dark Blue
      innerBrightness = 80;  // Inner brightness for Lavender
      outerBrightness = 60;  // Outer brightness for Dark Blue
      break;

    case 7:  // Mint and Brown
#if DEBUG > 0
      Serial.println("Inner Mint - Outer Brown");
#endif
      innerColour = 16;      // Mint
      outerColour = 21;      // Brown
      innerBrightness = 80;  // Inner brightness for Mint
      outerBrightness = 60;  // Outer brightness for Brown
      break;

    case 8:  // Salmon and Light Green
#if DEBUG > 0
      Serial.println("Inner Salmon - Outer Light Green");
#endif
      innerColour = 17;      // Salmon
      outerColour = 25;      // Light Green
      innerBrightness = 80;  // Inner brightness for Salmon
      outerBrightness = 60;  // Outer brightness for Light Green
      break;

    case 9:  // Sea Green and Peach
#if DEBUG > 0
      Serial.println("Inner Sea Green - Outer Peach");
#endif
      innerColour = 28;      // Sea Green
      outerColour = 18;      // Peach
      innerBrightness = 80;  // Inner brightness for Sea Green
      outerBrightness = 60;  // Outer brightness for Peach
      break;

    case 10:  // Light Yellow and Indigo
#if DEBUG > 0
      Serial.println("Inner Light Yellow - Outer Indigo");
#endif
      innerColour = 26;      // Light Yellow
      outerColour = 20;      // Indigo
      innerBrightness = 80;  // Inner brightness for Light Yellow
      outerBrightness = 60;  // Outer brightness for Indigo
      break;

    case 11:  // Purple and Gold
#if DEBUG > 0
      Serial.println("Inner Purple - Outer Gold");
#endif
      innerColour = 9;       // Purple
      outerColour = 11;      // Gold
      innerBrightness = 80;  // Inner brightness for Purple
      outerBrightness = 60;  // Outer brightness for Gold
      break;

    case 12:  // Coral and Light Blue
#if DEBUG > 0
      Serial.println("Inner Coral - Outer Light Blue");
#endif
      innerColour = 15;      // Coral
      outerColour = 12;      // Light Blue
      innerBrightness = 80;  // Inner brightness for Coral
      outerBrightness = 60;  // Outer brightness for Light Blue
      break;

    case 13:  // Light Green and Charcoal
#if DEBUG > 0
      Serial.println("Inner Light Green - Outer Charcoal");
#endif
      innerColour = 25;      // Light Green
      outerColour = 22;      // Charcoal
      innerBrightness = 80;  // Inner brightness for Light Green
      outerBrightness = 60;  // Outer brightness for Charcoal
      break;

    case 14:  // Mint and Lavender
#if DEBUG > 0
      Serial.println("Inner Mint - Outer Lavender");
#endif
      innerColour = 16;      // Mint
      outerColour = 14;      // Lavender
      innerBrightness = 80;  // Inner brightness for Mint
      outerBrightness = 60;  // Outer brightness for Lavender
      break;

    case 15:  // Pink and Gold
#if DEBUG > 0
      Serial.println("Inner Pink - Outer Gold");
#endif
      innerColour = 10;      // Pink
      outerColour = 11;      // Gold
      innerBrightness = 80;  // Inner brightness for Pink
      outerBrightness = 60;  // Outer brightness for Gold
      break;

    case 16:  // Blue and Light Yellow
#if DEBUG > 0
      Serial.println("Inner Blue - Outer Light Yellow");
#endif
      innerColour = 4;       // Blue
      outerColour = 26;      // Light Yellow
      innerBrightness = 80;  // Inner brightness for Blue
      outerBrightness = 60;  // Outer brightness for Light Yellow
      break;

    case 17:  // Salmon and Light Blue
#if DEBUG > 0
      Serial.println("Inner Salmon - Outer Light Blue");
#endif
      innerColour = 17;      // Salmon
      outerColour = 12;      // Light Blue
      innerBrightness = 80;  // Inner brightness for Salmon
      outerBrightness = 60;  // Outer brightness for Light Blue
      break;

    case 18:  // Sea Green and Indigo
#if DEBUG > 0
      Serial.println("Inner Sea Green - Outer Indigo");
#endif
      innerColour = 28;      // Sea Green
      outerColour = 20;      // Indigo
      innerBrightness = 80;  // Inner brightness for Sea Green
      outerBrightness = 60;  // Outer brightness for Indigo
      break;

    case 19:  // Orange and Lavender
#if DEBUG > 0
      Serial.println("Inner Orange - Outer Lavender");
#endif
      innerColour = 8;       // Orange
      outerColour = 14;      // Lavender
      innerBrightness = 80;  // Inner brightness for Orange
      outerBrightness = 60;  // Outer brightness for Lavender
      break;

    case 20:  // Teal and Coral
#if DEBUG > 0
      Serial.println("Inner Teal - Outer Coral");
#endif
      innerColour = 13;      // Teal
      outerColour = 15;      // Coral
      innerBrightness = 80;  // Inner brightness for Teal
      outerBrightness = 60;  // Outer brightness for Coral
      break;

    case 21:  // Dark Blue and Peach
#if DEBUG > 0
      Serial.println("Inner Dark Blue - Outer Peach");
#endif
      innerColour = 29;      // Dark Blue
      outerColour = 18;      // Peach
      innerBrightness = 80;  // Inner brightness for Dark Blue
      outerBrightness = 60;  // Outer brightness for Peach
      break;

    case 22:  // Light Green and Mint
#if DEBUG > 0
      Serial.println("Inner Light Green - Outer Mint");
#endif
      innerColour = 25;      // Light Green
      outerColour = 16;      // Mint
      innerBrightness = 80;  // Inner brightness for Light Green
      outerBrightness = 60;  // Outer brightness for Mint
      break;

    case 23:  // Light Blue and Salmon
#if DEBUG > 0
      Serial.println("Inner Light Blue - Outer Salmon");
#endif
      innerColour = 12;      // Light Blue
      outerColour = 17;      // Salmon
      innerBrightness = 80;  // Inner brightness for Light Blue
      outerBrightness = 60;  // Outer brightness for Salmon
      break;

    case 24:  // Coral and Sea Green
#if DEBUG > 0
      Serial.println("Inner Coral - Outer Sea Green");
#endif
      innerColour = 15;      // Coral
      outerColour = 28;      // Sea Green
      innerBrightness = 80;  // Inner brightness for Coral
      outerBrightness = 60;  // Outer brightness for Sea Green
      break;

    case 25:  // Gold and Light Green
#if DEBUG > 0
      Serial.println("Inner Gold - Outer Light Green");
#endif
      innerColour = 11;      // Gold
      outerColour = 25;      // Light Green
      innerBrightness = 80;  // Inner brightness for Gold
      outerBrightness = 60;  // Outer brightness for Light Green
      break;

    case 26:  // Light Yellow and Coral
#if DEBUG > 0
      Serial.println("Inner Light Yellow - Outer Coral");
#endif
      innerColour = 26;      // Light Yellow
      outerColour = 15;      // Coral
      innerBrightness = 80;  // Inner brightness for Light Yellow
      outerBrightness = 60;  // Outer brightness for Coral
      break;


      /*    This somehow breaks the tail and head LED strips..      */
    case 27:  // Pink and Light Green
#if DEBUG > 0
      Serial.println("Inner Pink - Outer Light Green");
#endif
      innerColour = 10;      // Pink
      outerColour = 25;      // Light Green
      innerBrightness = 80;  // Inner brightness for Pink
      outerBrightness = 60;  // Outer brightness for Light Green
      break;

    case 28:  // Indigo and Mint
#if DEBUG > 0
      Serial.println("Inner Indigo - Outer Mint");
#endif
      innerColour = 20;      // Indigo
      outerColour = 16;      // Mint
      innerBrightness = 80;  // Inner brightness for Indigo
      outerBrightness = 60;  // Outer brightness for Mint
      break;

    case 29:  // Dark Blue and Light Yellow
#if DEBUG > 0
      Serial.println("Inner Dark Blue - Outer Light Yellow");
#endif
      innerColour = 29;      // Dark Blue
      outerColour = 26;      // Light Yellow
      innerBrightness = 80;  // Inner brightness for Dark Blue
      outerBrightness = 60;  // Outer brightness for Light Yellow
      break;

    case 30:  // Teal and Gold
#if DEBUG > 0
      Serial.println("Inner Teal - Outer Gold");
#endif
      innerColour = 13;      // Teal
      outerColour = 11;      // Gold
      innerBrightness = 80;  // Inner brightness for Teal
      outerBrightness = 60;  // Outer brightness for Gold
      break;

    case 31:  // Sea Green and Light Blue
#if DEBUG > 0
      Serial.println("Inner Sea Green - Outer Light Blue");
#endif
      innerColour = 28;      // Sea Green
      outerColour = 12;      // Light Blue
      innerBrightness = 80;  // Inner brightness for Sea Green
      outerBrightness = 60;  // Outer brightness for Light Blue
      break;
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
  float G = 0, R = 0, B = 0, W = 0;
  switch (colorCode) {
    case 0:  // Black
      // R, G, B, W are already initialized to 0, no need to change them.
      break;
    // Primary colors
    case 1:                   // White
      W = 2.55 * brightness;  // Full brightness for White
      R = 2.55 * brightness;
      G = 2.55 * brightness;
      B = 2.55 * brightness;
      break;

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
    case 5:                   // Yellow
      R = 2.55 * brightness;  // Red + Green
      G = 2.55 * brightness;
      break;

    case 6:                   // Cyan
      G = 2.55 * brightness;  // Green + Blue
      B = 2.55 * brightness;
      break;

    case 7:                   // Magenta
      R = 2.55 * brightness;  // Red + Blue
      B = 2.55 * brightness;
      break;

    // Tertiary colors
    case 8:                   // Orange
      R = 2.55 * brightness;  // More red than green
      G = 1.27 * brightness;  // Less green
      break;

    case 9:                   // Purple
      R = 2.55 * brightness;  // Red
      B = 1.27 * brightness;  // Less blue
      break;

    case 10:                  // Pink
      R = 2.55 * brightness;  // Red
      G = 1.27 * brightness;  // Less green
      break;

    case 11:                  // Gold
      R = 2.55 * brightness;  // Red
      G = 2.55 * brightness;  // Green
      break;

    case 12:                  // Light Blue
      G = 1.27 * brightness;  // Less green
      B = 2.55 * brightness;  // More blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 13:                  // Teal
      G = 2.55 * brightness;  // Green
      B = 1.27 * brightness;  // Less blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 14:                  // Lavender
      R = 2.55 * brightness;  // Red
      B = 2.55 * brightness;  // Blue
      G = 1.27 * brightness;  // Less green
      W = 0.3 * brightness;   // Touch of white
      break;

    case 15:                  // Coral
      R = 2.55 * brightness;  // Red
      G = 1.9 * brightness;   // More green
      break;

    case 16:                  // Mint
      G = 2.55 * brightness;  // Green
      B = 1.9 * brightness;   // More blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 17:                  // Salmon
      R = 2.55 * brightness;  // Red
      G = 1.9 * brightness;   // More green
      B = 0.5 * brightness;   // Less blue
      break;

    case 18:                  // Peach
      R = 2.55 * brightness;  // Red
      G = 1.9 * brightness;   // More green
      B = 0.3 * brightness;   // Less blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 19:                  // Sky Blue
      R = 0.5 * brightness;   // Less red
      G = 2.55 * brightness;  // Green
      B = 2.55 * brightness;  // Blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 20:                  // Indigo
      R = 0.5 * brightness;   // Less red
      G = 0;                  // No green
      B = 2.55 * brightness;  // Blue
      break;

    case 21:                  // Brown
      R = 2.55 * brightness;  // Red
      G = 1.2 * brightness;   // Less green
      B = 0.2 * brightness;   // Less blue
      break;

    case 22:                 // Charcoal
      R = 0.5 * brightness;  // Less red
      G = 0.5 * brightness;  // Less green
      B = 0.5 * brightness;  // Less blue
      break;

    case 23:                 // Slate
      R = 0.2 * brightness;  // Very little red
      G = 0.3 * brightness;  // Very little green
      B = 0.5 * brightness;  // More blue
      break;

    case 24:                  // Lavender Blush
      R = 2.55 * brightness;  // Red
      G = 1.9 * brightness;   // More green
      B = 2.55 * brightness;  // Blue
      W = 0.3 * brightness;   // Touch of white
      break;

    case 25:                  // Light Green
      R = 0.5 * brightness;   // Less red
      G = 2.55 * brightness;  // Green
      W = 0.3 * brightness;   // Touch of white
      break;

    case 26:                  // Light Yellow
      R = 2.55 * brightness;  // Red
      G = 2.55 * brightness;  // Green
      W = 0.3 * brightness;   // Touch of white
      break;

    case 27:                  // Lime
      R = 0;                  // No red
      G = 2.55 * brightness;  // Green
      W = 0.3 * brightness;   // Touch of white
      break;

    case 28:                  // Sea Green
      R = 0;                  // No red
      G = 2.55 * brightness;  // Green
      B = 1.5 * brightness;   // More blue
      break;

    case 29:                 // Dark Blue
      R = 0;                 // No red
      G = 0;                 // No green
      B = 1.5 * brightness;  // More blue
      break;

    case 30:                  // Crimson
      R = 2.55 * brightness;  // Red
      G = 0;                  // No green
      B = 0.5 * brightness;   // Less blue
      break;

    case 31:                  // Gold
      R = 2.55 * brightness;  // Red
      G = 2.55 * brightness;  // Green
      break;

    default:
      // Handle unexpected color codes, reset to Black
      R = 0;
      G = 0;
      B = 0;
      W = 0;
      break;
  }


  if (mode == 1) {
    wingLF.fill(wingLF.Color(R, G, B, W));
    wingRF.fill(wingRF.Color(R, G, B, W));
    wingLB.fill(wingLB.Color(R, G, B, W));
    wingRB.fill(wingRB.Color(R, G, B, W));
    head.fill(head.Color(R, G, B, W));
    tail.fill(tail.Color(R, G, B, W));
  } else if (mode == 2) {
    returnArRGB[0] = R;
    returnArRGB[1] = G;
    returnArRGB[2] = B;
    returnArRGB[3] = W;
  }
}
