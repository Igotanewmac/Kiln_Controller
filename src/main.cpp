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




// some global variables

// the switch debounce value
uint8_t oldinput = 1;

// is the heater enabled?
uint8_t heatertoggle = 0;

// sgould the heater be turned on right now?
uint8_t heaterstate = 0;





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // encoder pins
  pinMode( PIN_ENCODER_SW , INPUT_PULLUP );
  pinMode( PIN_ENCODER_CLK , INPUT );
  pinMode( PIN_ENCODER_DT , INPUT );

  // heater pin
  pinMode( PIN_HEATER , OUTPUT );
  digitalWrite( PIN_HEATER , LOW );
  
  // displau
  obj_leddisplay.shutdown( 0 , false );
  obj_leddisplay.setIntensity( 0 , 1 );
  obj_leddisplay.clearDisplay(0);

}







uint8_t updatecount = 0;


void loop() {
  // put your main code here, to run repeatedly:
  

  // get current temperature
  uint16_t current_temp;
  updatecount++;
  if ( updatecount == 5 ) { updatecount = 0; }
  if ( updatecount == 0 ) {
    current_temp = obj_thermocouple.readCelsius();
  }

  // get current target temperature
  if ( obj_encoder.read() < 0 ) {
    obj_encoder.write(0);
  }
  uint16_t target_temp = obj_encoder.read() / 4;


  // check switch
  uint8_t newinput = digitalRead( PIN_ENCODER_SW );

  if ( ( newinput == 0 ) && ( oldinput == 1 ) ) {
    // switch has been pressed!
    heatertoggle = heatertoggle ? 0 : 1;
  }

  oldinput = newinput;


    // check for heater status
  if ( ( current_temp < target_temp ) && ( heatertoggle ) ) {
    digitalWrite( PIN_HEATER , HIGH );
    heaterstate = 1;
  }
  else {
    digitalWrite( PIN_HEATER , LOW );
    heaterstate = 0;
  }

  // update display
  uint16_t temptemp;
  //actual temp
  temptemp = current_temp;
  obj_leddisplay.setDigit( 0 , 2 , temptemp / 100 , heaterstate );
  temptemp -= (temptemp/100)*100;
  obj_leddisplay.setDigit( 0 , 1 , temptemp / 10 , heaterstate );
  temptemp -= (temptemp/10)*10;
  obj_leddisplay.setDigit( 0 , 0 , temptemp , heaterstate );
  // targettemp
  temptemp = target_temp;
  obj_leddisplay.setDigit( 0 , 6 , temptemp / 100 , heatertoggle );
  temptemp -= (temptemp/100)*100;
  obj_leddisplay.setDigit( 0 , 5 , temptemp / 10 , heatertoggle );
  temptemp -= (temptemp/10)*10;
  obj_leddisplay.setDigit( 0 , 4 , temptemp , heatertoggle );
  



  delay( 100 );


}








