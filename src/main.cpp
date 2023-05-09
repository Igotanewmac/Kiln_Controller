#include <Arduino.h>


/*

Pins

2,3 Encoder
4 Encoder Switch
5,6,7 Thermometer
8 Heaterpin
9,10,11 Display

*/

#define PIN_ENCODER_CLK 2
#define PIN_ENCODER_DT 3
#define PIN_ENCODER_SW 4

#define PIN_THERMOCOUPLE_DO 5
#define PIN_THERMOCOUPLE_CS 6
#define PIN_THERMOCOUPLE_CLK 7

#define PIN_HEATER 8

#define PIN_LED_DIN 9
#define PIN_LED_CS 10
#define PIN_LED_CLK 11



// create the encoder object
#include <Encoder.h>
Encoder obj_encoder( PIN_ENCODER_CLK , PIN_ENCODER_DT );



// create the led display object
#include "LedControl.h"
LedControl obj_leddisplay = LedControl( PIN_LED_DIN , PIN_LED_CLK , PIN_LED_CS , 1 );



// create the thermocouple object
#include "max6675.h"
MAX6675 obj_thermocouple( PIN_THERMOCOUPLE_CLK , PIN_THERMOCOUPLE_CS , PIN_THERMOCOUPLE_DO );




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( heaterpin , OUTPUT );

  pinMode( PIN_ENCODER_SW , INPUT_PULLUP );
  pinMode( PIN_ENCODER_CLK , INPUT );
  pinMode( PIN_ENCODER_DT , INPUT );

    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,1);
  /* and clear the display */
  lc.clearDisplay(0);


}

void loop() {
  // put your main code here, to run repeatedly:
  
  // lc.setChar(0,0,'1',true);
  
  
  float temperature = thermocouple.readCelsius();

  Serial.print("C = "); 
  Serial.println( temperature );

  float testtemp = myEnc.read()/4;


  int temptemp = (int)testtemp;
  
  // 100s digit
  lc.setDigit( 0 , 6 , temptemp/100 , 0 );
  temptemp -= (temptemp/100)*100;
  
  // 10s digit
  lc.setDigit( 0 , 5 , temptemp/10 , 0 );
  temptemp -= (temptemp/10)*10;
  
  // 1s digit
  lc.setDigit( 0 , 4 , temptemp , 0 );


  lc.setDigit( 0 , 0 , digitalRead( PIN_ENCODER_SW ) ? 1 : 0 , 0 );



  if ( temperature <= MAXTEMP ) {
    digitalWrite( heaterpin , HIGH );
  }
  else {
    digitalWrite( heaterpin , LOW );
  }

  delay(500);

}








