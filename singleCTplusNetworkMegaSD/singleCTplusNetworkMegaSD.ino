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
#include <PubSubClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Losant.h>
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

// create an ethernet instance
EthernetClient EthClient;

// create an SD card instance
File myFile;

// Losant credentials.
const char* LOSANT_DEVICE_ID = "5a7d08ee5672f90006c7afed";
const char* LOSANT_ACCESS_KEY = "d7e843d2-c53e-4085-965b-5e0b0b86c543";
const char* LOSANT_ACCESS_SECRET = "24ce13d733312a385c43028fe230d3f0bfd8076bad2d2dff9ac2d3cce4c2fcef";

// Create an instance of a Losant device.
LosantDevice device(LOSANT_DEVICE_ID);

// On-board emonTx LED
const int LEDpin = 9;   

const int WAIT_TIME = 3 * 1000;
int status;
int failedConnectionAttempCounter;

void setup()
{  
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("emonTX Shield Single CT with networking connected to losat"); 
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
  

  // call the connect to stuff voids
delay(500);
connectToInternet();

  }


void connectToInternet(){
if (EthClient.connected())
{
EthClient.stop();
}
Serial.println("Connecting to the internet via ethernet...");
// the media access control (ethernet hardware) address for the shield
// Leave this as is if your MAC address is not labelled on your ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");

}
  Serial.println(Ethernet.localIP());
// Connect the device instance to Losant using TLS encryption.
  device.connect(EthClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);

  while(!device.connected()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected!");

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
ReportToLosant(ct1.realPower, ct1.apparentPower,ct1.powerFactor, ct1.Irms,ct1.Vrms);
//AddRecordSD(now, ct1.realPower, ct1.apparentPower,ct1.powerFactor, ct1.Irms,ct1.Vrms);

   delay(WAIT_TIME);
}

//void AddRecordSD(DateTime dateTime,float rP, int aP, float Pf, float I, float V)
//{
//// start a file
//     myFile = SD.open("test.txt", FILE_WRITE);
//  //make sure the object exists
//  if (myFile) {    
//    myFile.print(dateTime.year(), DEC);
//    myFile.print(dateTime.month(), DEC);
//    myFile.print(dateTime.day(), DEC);
//    myFile.print(dateTime.hour(), DEC);
//    myFile.print(dateTime.minute(), DEC);
//    myFile.print(",");
//    myFile.print(rP);
//    myFile.print(",");    
//    myFile.print(aP);
//    myFile.print(",");    
//    myFile.print(Pf);
//    myFile.print(",");    
//    myFile.print(I);
//    myFile.print(",");    
//    myFile.print(V);
//    myFile.print(",");    
//    myFile.println();
//    myFile.close(); // close the file
//  }
//    // if the file didn't open, print an error:
//  else {
//    Serial.println("error opening test.txt");
//  }
// 
//}

void ReportToLosant(float rP, int aP, float pF, float I, float V)
{
 
    // Build a JSON object with the state to report.
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject& state = jsonBuffer.createObject();
    state["realPower"] = rP;
    state["apparentPower"] = aP;
    state["powerFactor"] = pF;
    state["Current"] = I;
    state["Voltage"] = V;
    // Report the state to Losant.
    device.sendState(state);
}


void ReportToSerialOut(DateTime dateTime, float rP, int aP, float pF, float I, float V)
{
  Serial.print("Time");
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


