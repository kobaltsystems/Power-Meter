/*
  Part 1 - Measuring current with a single CT
  
  The code below is the code needed to measure how much current is flowing through a CT plugged into the emontx.
  
  For details on the rms current calculations that occur in EmonLib.h see this page on:
  AC Power Theory - Arduino maths for an overview. http://openenergymonitor.org/emon/buildingblocks/ac-power-arduino-maths
  -----------------------------------------
  Part of the openenergymonitor.org project
  Licence: GNU GPL V3
*/

#include "EmonLib.h"                   // Include Emon Library:  https://github.com/openenergymonitor/EmonLib
EnergyMonitor ct1;                     // Create an instance

// On-board emonTx LED
const int LEDpin = 9;   

void setup()
{  
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("emonTX Shield Single CT"); 
  Serial.println("OpenEnergyMonitor.org");

// Calibration factor = CT ratio / burden resistance = (100A / 0.05A) / 33 Ohms = 60.606
  ct1.current(1, 60.606);
  
// (ADC input, calibration, phase_shift)
  ct1.voltage(0, 300.6, 1.7);        

  // Setup indicator LED
  pinMode(LEDpin, OUTPUT);                                              
  digitalWrite(LEDpin, HIGH); 

}

void loop()
{

    // Calculate all. No.of crossings, time-out 
  ct1.calcVI(20,2000); 

  //M Print power 
  Serial.print("ct1.realPower: "); 
  Serial.print(ct1.realPower);     

  Serial.print(" ct1.apparentPower: "); 
  Serial.print(ct1.apparentPower);     

  Serial.print(" ct1.powerFactor: "); 
  Serial.print(ct1.powerFactor);     

  Serial.print(" ct1.Irms: "); 
  Serial.print(ct1.Irms);

  Serial.print(" ct1.Vrms: "); 
  Serial.print(ct1.Vrms);
  Serial.println();

  delay(1000);
}
