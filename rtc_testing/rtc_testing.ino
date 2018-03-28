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
RTC.adjust(Year, 18);
RTC.adjust(month, 3);
RTC.adjust(day, 5);
RTC.adjust(hour, 6);
RTC.adjust(minute, 17);
RTC.adjust(second, 10);

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
