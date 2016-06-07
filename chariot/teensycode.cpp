#define USE_OCTOWS2811

#include<OctoWS2811.h>
#include<FastLED.h>

// this is numbe of LEDs per OCTO channel
#define NUM_LEDS_PER_STRIP    500
// number of OCTO channels, up to 8
#define NUM_STRIPS            4

// wheels are the same length if that's ok
#define WHEEL_LENGTH          100

// need to know where each wheel begins/ends in the line
#define WHEEL_ONE_BEGIN       100
#define WHEEL_ONE_END         WHEEL_ONE_BEGIN + WHEEL_LENGTH
#define WHEEL_TWO_BEGIN       300
#define WHEEL_TWO_END         WHEEL_TWO_BEGIN + WHEEL_LENGTH
#define WHEEL_THREE_BEGIN     500
#define WHEEL_THREE_END       WHEEL_THREE_BEGIN + WHEEL_LENGTH
#define WHEEL_FOUR_BEGIN      700
#define WHEEL_FOUR_END        WHEEL_FOUR_BEGIN + WHEEL_LENGTH

// if we can do this, probably work well
#define WAVE_SIZE             WHEEL_LENGTH
#define WAVE_UPDATE_SPEED            100

// for reading strings from serial
#define BUFFER_S              32

// Pin layouts on the teensy 3:
// OctoWS2811: 2,14,7,8,6,20,21,5
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CHSV wave[WAVE_SIZE];

int wheel_begins[] = { WHEEL_ONE_BEGIN,
  WHEEL_TWO_BEGIN,
  WHEEL_THREE_BEGIN,
  WHEEL_FOUR_BEGIN };
int wheel_ends[] = { WHEEL_ONE_END,
  WHEEL_TWO_END,
  WHEEL_THREE_END,
  WHEEL_FOUR_END };

int wave_index = 0;

void setWheel(int index, int begin, int end);
void setup() 
{
  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(192);

  // calculate wave values
  for(int i=0; i<WAVE_SIZE; ++i)
    wave[i] = CHSV(0, 0, 255*i/(WAVE_SIZE) );

}

uint32_t index_timer = 0;
char buffer[BUFFER_S];
int some_int = 0;
void loop() 
{
  // do some serial stuff
  if( int count = Serial.available() )
  {
    for( int i=0; i<count; ++i)
      buffer[i] = Serial.read();
    some_int = atoi(buffer);
    Serial.print("i think you sent ");
    Serial.println(some_int);
  }
  
  // set color first
  for( int i=0; i<NUM_LEDS_PER_STRIP*NUM_STRIPS; ++i)
    leds[i] = CRGB::Navy;

  for( int i=0; i<4; ++i)
    setWheel( wave_index, wheel_begins[i], wheel_ends[i]);

  if( millis() > index_timer )
  {
    index_timer = millis() + WAVE_UPDATE_SPEED;
    wave_index ++;
  }

  // update all the things
  LEDS.show();
}

void setWheel(int index, int begin, int end)
{
  // try with this, should be close to what we want
  for( int i=begin; i<end; ++i)
    leds[i] = wave[(i + index)%WAVE_SIZE];
}
