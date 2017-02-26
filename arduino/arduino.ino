

#define test_output
#define XPIN 1
#define YPIN 2
#define GPIN 3
#define MAIN_LED = 13;
#define ID 0x01
#define START_BYTE 0XAB


void calibrate(){
  return;
}

void setup () {
  pinMode(MAIN_LED, OUTPUT);
  calibrate();
}

byte get_data(byte pin){
  byte strain = byte(anlaogRead(pin)/4);
  return
}


void loop() {
  
  #ifdef test_output
  for (int i = 0; i<10; i++){
    byte buffer = {
       START_BYTE,
       ID,
       i,
       i+1,
       i+2};
    Serial.write(buffer, sizeof(buffer));
    delay(500);
  }
  delay(5000);
  
  #endif
  
  #ifndef testoutput
  byte buffer[5] = {
   START_BYTE,
   ID,
   get_data(XPIN),
   get_data(YPIN),
   get_data(GPIN)
 };
 
 Serial.write(buffer,sizeof(buffer));
 delay(500);
 #endif
 
}
