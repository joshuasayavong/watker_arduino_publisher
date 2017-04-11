#define ID 0x01
#define transmission_period 200

//#define test_output
//#define raw_output
#define XPIN 1
#define YPIN 2
#define GPIN 3
#define MAIN_LED 13
#define START_BYTE1 0XAB
#define START_BYTE2 0xCD
#define yscale -8.0
#define xscale 4.5
#define gscale -26.3


int datumx;
int datumy;
int datumg;

void calibrate(){
  
  // determines center values of unloaded stress sensor by calculating 
  // average raw value unloaded
  
  int x = 0;
  int y = 0;
  int g = 0;
  
  for (int i = 0; i<10; i++)
  {
    x += analogRead(XPIN);
    y += analogRead(YPIN);
    g += analogRead(GPIN);
  }
  datumx = x/10;
  datumy = y/10;
  datumg = g/10;
  
  return;
}



int get_data(byte pin){
  
  // Three modes of operation:
  // test_output is defined:
  //   returns constant value 0x12AD
  // raw_output is defined:
  //   returns raw analog read
  // neither of above is defined:
  //   returns difference from calibrated value, offset to center (1048/2)
  
  int raw_value, midpoint; 
  float scale;
  
  raw_value = analogRead(pin);
  
#ifdef test_output
  return 0x12AD;
#endif

#ifdef raw_output
  return raw_value;
#endif

#ifndef test_output

  switch (pin) {
    case XPIN:
      midpoint = datumx;
      scale = xscale*10;      
      break;
    case YPIN:
      midpoint = datumy;
      scale = yscale*10;
      break;
    case GPIN:
      midpoint = datumg;
      scale = gscale*10;
      break;
  }
  
  return int((midpoint - raw_value)*scale);
#endif

}

void send_buffer(){
  
  // Segment the data into a byte array formatted as:
  // [START_BYTE, START_BYTE, ID, X2, X1, Y2, Y1, G2, G1]
  // For example: x_data = 317 or 0x013D
  // then x2= 0x01, x1 = 0x3D  
  
  int x_data, y_data, g_data;
  byte x1, x2, y1,y2, g1, g2;
  
  x_data = get_data(XPIN);
  y_data = get_data(YPIN);
  g_data = get_data(GPIN);
  
  x2 =  (x_data >> 8) & 0xFF;
  x1 =  (x_data) & 0xFF;
  y2 =  (y_data >> 8) & 0xFF;
  y1 =  (y_data) & 0xFF;
  g2 =  (g_data >> 8) & 0xFF;
  g1 =  (g_data) & 0xFF;
  
  byte buffer[9] = {
   START_BYTE1,
   START_BYTE2,
   ID,
   x2,
   x1,
   y2,
   y1,
   g2,
   g1
   };
   
  Serial.write(buffer,sizeof(buffer));

}


void setup () {

  //Make sure to set the baud rate of the Bluetooth LE in Serial Monitor:
  // +++ (New Line)
  // AT+UART=9600 (CR+NL)
  // Close the Serial Monitor
  
  pinMode(MAIN_LED, OUTPUT);
  Serial.begin(9600);
  calibrate();
  
}

void loop() {

 send_buffer();
 delay(transmission_period); 

}
