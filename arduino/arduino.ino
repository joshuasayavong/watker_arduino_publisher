#define test_output
#define XPIN 1
#define YPIN 2
#define GPIN 3
#define MAIN_LED 13
#define ID 0x01
#define START_BYTE1 0XAB
#define START_BYTE2 0xCD


void calibrate(){
  return;
}

void setup () {
  pinMode(MAIN_LED, OUTPUT);
  calibrate();
}

byte get_data(byte pin){

#ifdef test_output
  byte strain = 0xAD;
#endif

#ifndef test_output
  byte strain = byte(anlaogRead(pin)/4);
#endif

  return strain;
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
 delay(100);
 
}
