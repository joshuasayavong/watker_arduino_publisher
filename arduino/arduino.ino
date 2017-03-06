//#define test_output
#define raw_output
#define XPIN 1
#define YPIN 2
#define GPIN 3
#define MAIN_LED 13
#define ID 0x02
#define START_BYTE1 0XAB
#define START_BYTE2 0xCD

int datumx;
int datumy;
int datumg;

void calibrate(){
  int x = 0;
  int y = 0;
  int g = 0;
  for (int i = 0; i<10; i++)
  {
    x += analogRead(XPIN);
    y += analogRead(YPIN);
    g += analogRead(GPIN);
  }
  x /= 10;
  y /= 10;
  g /= 10;
  return;
}

void setup () {
  pinMode(MAIN_LED, OUTPUT);
  Serial.begin(9600);
  calibrate();
}

byte get_data(byte pin){
  
#ifdef test_output
  byte stress = 0xAD;
#endif

#ifdef raw_output
  return byte(analogRead(pin)/4);
#endif

#ifndef test_output

  int midpoint;
  switch (pin) {
    case XPIN:
      midpoint = datumx;
      break;
    case YPIN:
      midpoint = datumy;
      break;
    case GPIN:
      midpoint = datumg;
      break;
  }
  
  byte bmidpoint = 0xFF/2;
  
  byte stress = bmidpoint + byte((midpoint - analogRead(pin)));
#endif

  return stress;
}


void loop() {
 
  byte buffer[6] = {
   START_BYTE1,
   START_BYTE2,
   ID,
   get_data(XPIN),
   get_data(YPIN),
   get_data(GPIN)
 };
 
 Serial.write(buffer,sizeof(buffer));
 delay(500);
 
}
