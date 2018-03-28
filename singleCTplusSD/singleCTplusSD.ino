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
#include <SPI.h> //SD Bus
#include <SD.h> //SD Card
#include <Wire.h>
#include <RTClib.h>

// create a EM instance
EnergyMonitor ct1;

// RTC instance
RTC_DS1307 RTC;

// create an SD card instance
const int chipSelect = 4;

// create an SD card instance
File myFile;

// On-board emonTx LED
const int LEDpin = 9;   

const int WAIT_TIME = 3 * 1000;
int status;
int failedConnectionAttempCounter;

void setup()
{  
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("OpenEnergyMonitor.org");

// setup RTC 
Wire.begin();
RTC.begin();

// Calibration factor = CT ratio / burden resistance = (100A / 0.05A) / 33 Ohms = 60.606
  ct1.current(1, 60.606);
  
// (ADC input, calibration, phase_shift)
  ct1.voltage(0, 300.6, 1.7);        

  // Setup indicator LED
  pinMode(LEDpin, OUTPUT);                                              
  digitalWrite(LEDpin, HIGH); 


ConnectToSD();

}
void ConnectToSD(){
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
 
  }

void loop()
{

//   Calculate all. No.of crossings, time-out 
  ct1.calcVI(20,2000);  
delay(500);

RTC_DS1307 RTC;
DateTime now = RTC.now();
delay(500);

// call voids
ReportToSerialOut(now, ct1.realPower, ct1.apparentPower,ct1.powerFactor, ct1.Irms,ct1.Vrms);
AddRecordSD(now, ct1.realPower, ct1.apparentPower,ct1.powerFactor, ct1.Irms,ct1.Vrms);

   delay(WAIT_TIME);
}

void AddRecordSD(DateTime dateTime,float rP, int aP, float Pf, float I, float V)
{
// start a file
     myFile = SD.open("test.txt", FILE_WRITE);
  //make sure the object exists
  if (myFile) {    
    myFile.print(dateTime.year(), DEC);
    myFile.print(dateTime.month(), DEC);
    myFile.print(dateTime.day(), DEC);
    myFile.print(dateTime.hour(), DEC);
    myFile.print(dateTime.minute(), DEC);
    myFile.print(",");
    myFile.print(rP);
    myFile.print(",");    
    myFile.print(aP);
    myFile.print(",");    
    myFile.print(Pf);
    myFile.print(",");    
    myFile.print(I);
    myFile.print(",");    
    myFile.print(V);
    myFile.print(",");    
    myFile.println();
    myFile.close(); // close the file
  }
    // if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
 
}

void ReportToSerialOut(DateTime dateTime, float rP, int aP, float pF, float I, float V)
{
  Serial.print("Time ");
  Serial.print(dateTime.year(), DEC);
  Serial.print(dateTime.month(), DEC);
  Serial.print(dateTime.day(), DEC);
  Serial.print(dateTime.hour(), DEC);
  Serial.print(dateTime.minute(), DEC);
  Serial.print(dateTime.second(), DEC);

  Serial.print("|||| ");
   Serial.print(rP);
  Serial.print("W ");
  Serial.print(aP);
  Serial.print("W ");
  Serial.print(V);
  Serial.print("V ");
  Serial.print(I);
  Serial.print("Amps ");
  Serial.print(pF);
  Serial.println("pF " );
  }


