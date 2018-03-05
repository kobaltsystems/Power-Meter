#include <Wire.h>
#include <RTClib.h>
void printDateTime(DateTime dateTime);
RTC_DS1307 RTC;
void setup (void){
Serial.begin(9600);
Wire.begin();
RTC.begin();
}
void loop() {
if (Serial.available() > 0) {
int instruct = Serial.read();
switch (instruct) {
case 'D': {
DateTime now = RTC.now();
printDateTime(now);
break;
} case 'S':
RTC.set(RTC_YEAR, 18);
RTC.set(RTC_MONTH, 3);
RTC.set(RTC_DAY, 5);
RTC.set(RTC_HOUR, 17);
RTC.set(RTC_MINUTE, 35);
RTC.set(RTC_SECOND, 10);

break;
}
}
}
void printDateTime(DateTime dateTime) {
Serial.print(dateTime.year(), DEC);
Serial.print('/');
Serial.print(dateTime.month(), DEC);
Serial.print('/');
Serial.print(dateTime.day(), DEC);
Serial.print(' ');
Serial.print(dateTime.hour(), DEC);
Serial.print(':');
Serial.print(dateTime.minute(), DEC);
Serial.print(':');
Serial.print(dateTime.second(), DEC);
Serial.println();
}
