//Packages
#include <Servo.h>
#include <math.h>
#include <digitalWriteFast.h>


//Motor Setup
int motorSpeed=1500;
Servo gimbal;

/* Pin Setup */
#define L_ENCODER_PIN_A 18
#define L_ENCODER_PIN_B 19
#define L_ENCODER_PIN_Z 20
#define INST_VEL
#define DEBUG_MODE
#define START_BIT_1 0xEF
#define START_BIT_2 0X02
#define ID 0X01
#define WHEEL_RADIUS 5 // in cm

/* Variables for encoder reading */

// reading to serial every 500ms = 0.5s
int time_step = 200 ; 

volatile long l_rev = 0;
volatile long l_ticks = 0;
volatile float l_wheelAngle = 0;
volatile long distLast = 0;

volatile int l_prevStateA = -1;
volatile int l_prevStateB = -1;
volatile int l_readingA;
volatile int l_readingB;
volatile bool l_atHome = false;

int sensorValue = 0;

/*  Four States -> 8 Transitions

  00 -> 01 = +1 tick        00 -> 10 = -1 tick
  01 -> 11 = +1 tick        01 -> 00 = -1 tick
  10 -> 00 = +1 tick        10 -> 11 = -1 tick
  11 -> 10 = +1 tick        11 -> 01 = -1 tick

*/

volatile long l_lastTicks = 0;
volatile long lastTime= 0;

volatile long vel_lastTime= 0;
volatile long vel_currentTime= 0;
volatile long prevT = 0;
volatile long currT = 0;
#define TICKS_PER_ROTATION 2048

//Rolling velocity
#define RV_SIZE 100
volatile double RV[RV_SIZE];
volatile double avgV = 0;
volatile int index = 0;
volatile long num_ticks=0;

void increment_index(){
  if (index < RV_SIZE)
    index++;
  else 
    index = 0;
}

void update_rv(double new_v)
{
  RV[index] = new_v;
  increment_index();
}

double get_rv () 
{
  double sumV = 0;
  for (int i = 0; i < RV_SIZE; i++)
    sumV += RV[i];
  return (float)(sumV/RV_SIZE)/TICKS_PER_ROTATION;
}

double get_dist (long x) 
{
  return ((double)(x-distLast)/TICKS_PER_ROTATION) * double(2*M_PI*WHEEL_RADIUS*0.1) ;
}


void onPinChanged() {
  
  l_readingA = digitalReadFast(L_ENCODER_PIN_A);
  l_readingB = digitalReadFast(L_ENCODER_PIN_B);
  l_atHome = !digitalReadFast(L_ENCODER_PIN_Z);

  bool x = l_prevStateA ^ l_readingB;
  bool y = l_prevStateB ^ l_readingA;
  
  bool countUp = ((!y) && x);
  bool countDown = (y && (!x));
  long z_home = 0;
  
  if (countUp) {
      l_ticks = l_ticks + 1;
      num_ticks++;
  } else if (countDown) {
      l_ticks = l_ticks - 1;
      num_ticks--;
  }

#ifdef INST_VEL
  vel_currentTime= micros();
  if (num_ticks < 100 || num_ticks > 100) {
    update_rv((double)num_ticks*1000000.0/((vel_currentTime-vel_lastTime)));
    num_ticks=0;
  }
  vel_lastTime = vel_currentTime;
#endif
  
    l_prevStateA = l_readingA;
    l_prevStateB = l_readingB;
    
    if(l_atHome && abs(l_wheelAngle) > 300) {
        l_wheelAngle = 0;
        l_lastTicks = l_ticks;
        z_home = l_ticks;
    }
    
}

//intial Code to Run Once.
void setup() {

  // Set stepper motor speed
  gimbal.attach(2);
  gimbal.write(1500);
  gimbal.writeMicroseconds(1500);
  delay(5000);

  Serial.begin(9600); //  setup serial baudrate
  pinMode(L_ENCODER_PIN_A, INPUT);
  pinMode(L_ENCODER_PIN_B, INPUT);
  pinMode(L_ENCODER_PIN_Z, INPUT);
  
  //Set initial states for pin
  l_prevStateA = digitalRead(L_ENCODER_PIN_A);
  l_prevStateB = digitalRead(L_ENCODER_PIN_B);
  
  //Attach interrupts
  interrupts();
  attachInterrupt(digitalPinToInterrupt(L_ENCODER_PIN_A), onPinChanged, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_ENCODER_PIN_B), onPinChanged, CHANGE);

  //setup velocity
  for (int j = 0; j < RV_SIZE; j++)
    RV[j] = 0;
  distLast = 0;
  vel_currentTime= micros();
  vel_lastTime = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
   motorSpeed=1900;
   gimbal.writeMicroseconds(motorSpeed);

  //To prevent interrupts from affecting this
  long lT = l_lastTicks, t = l_ticks;
  l_lastTicks = l_ticks;
  l_wheelAngle = l_wheelAngle + 360.0f * ((float)(t - lT) / TICKS_PER_ROTATION);
  l_rev = l_ticks/TICKS_PER_ROTATION;
  
  long currentTime = millis();
  sensorValue = analogRead(A0);
  if (micros() - vel_lastTime > 200) 
    update_rv(0);
  
  if(currentTime - lastTime > time_step) {

#ifndef INST_VEL
      update_rv((double)(l_ticks - prevT) / (float)(currentTime - lastTime)*1000);
      prevT = l_ticks;
#endif

#ifdef DEBUG_MODE
      Serial.print("Angle: ");
      Serial.print(l_wheelAngle);
      Serial.print(", Ticks: ");
      Serial.print(l_ticks);
      Serial.print(", last: ");
      Serial.print(distLast);
      Serial.print(", Dist: ");
      Serial.print(get_dist(l_lastTicks));
      Serial.print(", Revolutions: ");
      Serial.print(l_rev);      
      Serial.print(", Average Speed: ");
      Serial.print(get_rv(),10);
      Serial.print(", Current: ");
      Serial.println(sensorValue);
#endif

#ifndef DEBUG_MODE
//print pose, inst velocity
      byte x [11] = {
          START_BIT_1,
          START_BIT_2,
          0xFF & ID,
          0xFF & (get_dist(l_lastTicks)*10000 >> 24),
          0xFF & (get_dist(l_lastTicks)*10000 >> 16),
          0xFF & (get_dist(l_lastTicks)*10000 >> 8),// velocity
          0xFF & (get_dist(l_lastTicks)*10000)
          0xFF & (get_rv()*10000 >> 24),
          0xFF & (get_rv()*10000 >> 16),
          0xFF & (get_rv()*10000 >> 8),// velocity
          0xFF & (get_rv()*10000)
      }
      Serial.write(x, sizeof(x));
      
#endif
      distLast = 5;
      lastTime = currentTime; 
      
  }
}
