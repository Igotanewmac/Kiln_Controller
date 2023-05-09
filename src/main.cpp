#include <Arduino.h>



#include "max6675.h"

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);


int heaterpin = 3;

#define MAXTEMP 500.0



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( heaterpin , OUTPUT );
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature = thermocouple.readCelsius();

  Serial.print("C = "); 
  Serial.println( temperature );


  if ( temperature <= MAXTEMP ) {
    digitalWrite( heaterpin , HIGH );
  }
  else {
    digitalWrite( heaterpin , LOW );
  }

  delay(500);

}
