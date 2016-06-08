#define USE_OCTOWS2811

#include<OctoWS2811.h>
#include<FastLED.h>

// this is numbe of LEDs per OCTO channel
#define NUM_LEDS_PER_STRIP    250
// number of OCTO channels, up to 8
#define NUM_STRIPS            4

#define WHEEL_ONE_LENGTH      200
#define WHEEL_TWO_LENGTH      200
#define WHEEL_THREE_LENGTH    250
#define WHEEL_FOUR_LENGTH     250

// need to know where each wheel begins/ends in the line
#define WHEEL_ONE_BEGIN       0
#define WHEEL_ONE_END         WHEEL_ONE_BEGIN + WHEEL_ONE_LENGTH
#define WHEEL_TWO_BEGIN       NUM_LEDS_PER_STRIP
#define WHEEL_TWO_END         WHEEL_TWO_BEGIN + WHEEL_TWO_LENGTH
#define WHEEL_THREE_BEGIN     NUM_LEDS_PER_STRIP*2
#define WHEEL_THREE_END       WHEEL_THREE_BEGIN + WHEEL_THREE_LENGTH
#define WHEEL_FOUR_BEGIN      NUM_LEDS_PER_STRIP*3
#define WHEEL_FOUR_END        WHEEL_FOUR_BEGIN + WHEEL_THREE_LENGTH

#define WAVE_SIZE             40
#define WAVE_UPDATE_INTERVAL_ONE  16
#define WAVE_UPDATE_INTERVAL_TWO  9

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

int wave_indicies[]= { WAVE_SIZE, WAVE_SIZE, WAVE_SIZE, WAVE_SIZE};

void setWheel(int index, int begin, int end);
void setup() 
{
  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(255);

  // calculate wave values
  for(int i=0; i<WAVE_SIZE; ++i)
    wave[i] = CHSV(0, 0, 255*i/(WAVE_SIZE*2) );
    //wave[i] = CHSV(0, 0, pow(2,i)/WAVE_SIZE );
  Serial.begin(9600);
}

uint32_t index_timer = 0;
uint32_t index_timer_two = 0;
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
  
  // background color
  for( int i=0; i<NUM_LEDS_PER_STRIP*4; ++i)
    leds[i] = CRGB::Navy - CHSV(0, 0, 140);


  // do all wheels
  for( int i=0; i<4; ++i)
    setWheel( wave_indicies[i], wheel_begins[i], wheel_ends[i]);

  if( millis() > index_timer )
  {
    index_timer = millis() + WAVE_UPDATE_INTERVAL_ONE;
    for( int i=0; i<2; ++i)
    {
      if( wave_indicies[i] > 0 )
        wave_indicies[i] --;
      else
        wave_indicies[i] = WAVE_SIZE;
    }
  }
  if( millis() > index_timer_two )
  {
    index_timer_two = millis() + WAVE_UPDATE_INTERVAL_TWO;
    for( int i=2; i<4; ++i)
    {
      if( wave_indicies[i] > 0 )
        wave_indicies[i] --;
      else
        wave_indicies[i] = WAVE_SIZE;
    }
  }

  // set debug color
  if( some_int > 0 )
  {
    for( int i=0; i<some_int; ++i)
      leds[i] = CRGB::Navy;
  }

  // update all the things
  LEDS.show();
}

void setWheel(int index, int begin, int end)
{
  // try with this, should be close to what we want
  for( int i=begin; i<end; ++i)
    leds[i] += wave[(i + index)%WAVE_SIZE];
}
