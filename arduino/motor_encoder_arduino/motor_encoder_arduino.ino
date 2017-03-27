#include <digitalWriteFast.h>
#include <math.h>

//#define test_output
//#define raw_output
#define transmission_period 100

#define L_ENCODER_PIN_A 2
#define L_ENCODER_PIN_B 3

#define START_BYTE1 0XEF
#define START_BYTE2 0x02

volatile int prevStateA = -1;
volatile int prevStateB = -1;
volatile int readingA;
volatile int readingB;
volatile bool atHome = false;
volatile signed int ticks = 0;
volatile signed int lastTicks = 0;
volatile long lastTime;

void onPinChanged() {
  
  readingA = digitalReadFast(L_ENCODER_PIN_A);
  readingB = digitalReadFast(L_ENCODER_PIN_B);

  bool x = prevStateA ^ readingB;
  bool y = prevStateB ^ readingA;
  
  bool countUp = ((!y) && x);
  bool countDown = (y && (!x));
  long z_home = 0;
  
  if (countUp) {
      ticks = ticks + 1;
  } else if (countDown) {
      ticks = ticks - 1;
  }

    prevStateA = readingA;
    prevStateB = readingB;
    

  
}

void send_buffer() {
   int local_ticks = ticks; // to prevent interrupt issues
   byte buffer[4] = {
   START_BYTE1,
   START_BYTE2,
   (local_ticks >> 8) & 0xFF,
   local_ticks & 0xFF,
   };
   
  Serial.write(buffer,sizeof(buffer));
}

void setup() {


  Serial.begin(9600); //  setup serial baudrate
  pinMode(L_ENCODER_PIN_A, INPUT);
  pinMode(L_ENCODER_PIN_B, INPUT);
  
  //Set initial states for pin
  prevStateA = digitalRead(L_ENCODER_PIN_A);
  prevStateB = digitalRead(L_ENCODER_PIN_B);
  
  //Attach interrupts
  interrupts();
  attachInterrupt(digitalPinToInterrupt(L_ENCODER_PIN_A), onPinChanged, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_ENCODER_PIN_B), onPinChanged, CHANGE);
  lastTime = millis();
}


void loop() {
  long currentTime = millis();
  if( currentTime - lastTime > transmission_period) {
    send_buffer();
    lastTime = currentTime;
  }
  
}
