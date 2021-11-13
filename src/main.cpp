// Bouncing balls
#include <FastLED.h>

#define FASTLED_INTERNAL

#define LED_PIN     3
#define NUM_LEDS    288
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_ROWS    18
#define NUM_COLS    16
#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

CRGB leds[NUM_LEDS];

class Ball{
  public:
    // constructor 
    Ball(){
      randomSeed(analogRead(0));
      _x = random(NUM_COLS);
      _y = random(NUM_ROWS);
      while (_delta_x == 0){
        _delta_x = random(-20,30)/10;
      }
      while (_delta_y ==0){
        _delta_y = random(-20,30)/10;
      }
      _color = CHSV(random(1,255),255,255);
      leds[LEDArrayPosition(_x, _y)] = _color;
    };

    void Fade(uint8_t percentValue){
      leds[LEDArrayPosition(_x, _y)].fadeToBlackBy(percentValue);
    }

    void FadePrevious(uint8_t x, uint8_t y, uint8_t percentValue=200){
      leds[LEDArrayPosition(x, y)].fadeToBlackBy(percentValue);
    }

    void Clear(){
      leds[LEDArrayPosition(_x, _y)] = CRGB::Black;
    }
    
    void ClearPrevious(uint8_t x, uint8_t y){
      leds[LEDArrayPosition(x, y)] = CRGB::Black;
    }

    void Move(){
      // this moves the Ball by delta_x, delta_y for each execution of loop 
      // do bounds checking
      // do fading of previous position

      // clear the previous x, y pixel
      ClearPrevious(_previous2_x, _previous2_y);
      _previous2_x = _previous_x;
      _previous2_y = _previous_y;
      FadePrevious(_previous2_x, _previous2_y, 240);

      // store current x,y to previous x, y
      _previous_x = _x;
      _previous_y = _y;
      // dim brightness of previous x, y
      FadePrevious(_previous_x, _previous_y, 180);


      // compute new x, y position
      _x += _delta_x;
      _y += _delta_y;

      // check for boundary conditions
      if (_x < 1){
        _x = 1;
        // change X direction
        _delta_x = _delta_x * (-1);
      }
      if (_x > NUM_COLS){
        _x = NUM_COLS;
        // change X direction
        _delta_x = _delta_x * (-1);
      }

      if (_y < 1){
        _y = 1;
        _delta_y = _delta_y * (-1);
      }

      if (_y > NUM_ROWS){
        _y = NUM_ROWS;
        _delta_y = _delta_y * (-1);
      }

      // draw current x, y
      leds[LEDArrayPosition((uint8_t)_x, (uint8_t)_y)] = _color;   
    }

  private: 
    float _x;
    float _y;
    uint8_t _previous_x=0;
    uint8_t _previous_y=0;
    uint8_t _previous2_x=0;
    uint8_t _previous2_y=0;
    float _delta_x;   // this determines speed and direction of ball
    float _delta_y;   // this determines speed and direction of ball
    CRGB  _color;
    int led_pos;

    int LEDArrayPosition(int x, int y){
      // do some bounds checking 
      if (x>NUM_COLS) x=NUM_COLS;
      if (x<1) x=1;
      if (y>NUM_ROWS) y = NUM_ROWS;
      if (y<1) y=1;

      if (y%2==0){
        led_pos = ((y) * NUM_COLS) - x;  // even row
      } else {
        led_pos = x + ((y-1) * NUM_COLS) -1;  // odd row 
      }
      return led_pos;
    }

};

void setup() {
  Serial.begin(115200);
  delay( 500 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>
      (leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );  

  // clear whole display
  FastLED.clear(); 
}

Ball balls[4];

void loop() {
  // I like to move it, move it! 
  for (int i=0; i<ARRAYSIZE(balls); i++){
    balls[i].Move();
  }

  // light my fire
  FastLED.show(); 

  // slow everything 
  delay(35);
}
