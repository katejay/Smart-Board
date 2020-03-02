#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include "HX711.h"
#define calibration_factor -7050.0 
#define DOUT 3
#define CLK 4

HX711 scale;
byte dayOfWeek=4;
byte dayOfMonth=1;
int Alarm = 2;
int AlarmSet = 0;
const int buttonPin = 5;   //d
int buttonState = 0;  //d

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

// Assumes you're passing in valid numbers
void setDateDS3231(
byte second,        // 0-59
byte minute,        // 0-59
byte hour,          // 1-23
byte dayOfWeek,     // 1-7
byte dayOfMonth,    // 1-28/29/30/31
byte month,         // 1-12
byte year)          // 0-99

{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second));    // 0 to bit 7 starts the clock
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));     
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(00010000); // sends 0x10 (hex) 00010000 (binary) to control register - turns on square wave
  Wire.endTransmission();
}

// Gets the date and time from the DS3231
void getDateDS3231(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(Wire.read() & 0x7f);
  *minute     = bcdToDec(Wire.read());
  *hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month      = bcdToDec(Wire.read());
  *year       = bcdToDec(Wire.read());
}

//Fire up the DMD library as dmd
SoftDMD dmd(1,1);  // DMD controls the entire display

void setup()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  dmd.setBrightness(255);
  dmd.clearScreen();
  dmd.selectFont(System5x7);
  dmd.begin();
  Wire.begin();
  pinMode(Alarm, OUTPUT);
  digitalWrite(Alarm, HIGH);
  pinMode(buttonPin, INPUT); // d

  dmd.clearScreen();
  dmd.drawString( 1, 0, "Smart");
  dmd.drawString( 2, 9, "Board");
  delay(6000);
  dmd.clearScreen();
  dmd.drawString( 10, 5, "BY");
  delay(3000);
  dmd.clearScreen();
  dmd.drawString( 1, 0, "BE-IT");
  dmd.drawString( 4, 9, "G:10");
  delay(6000);
}

void drawDay()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDS3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfWeek)
  {
  case 1:
    dmd.drawString( 1,0, "Sun,");    
    break;
  case 2:
    dmd.drawString( 1,0, "Mon,");        
    break;
  case 3:
    dmd.drawString( 1,0, "Tue,");        
    break;
  case 4:
    dmd.drawString( 1,0, "Wed,");        
    break;
  case 5:
    dmd.drawString( 1,0, "Thu,");        
    break;
  case 6:
    dmd.drawString( 1,0, "Fri,");        
    break;
  case 7:
    dmd.drawString( 1,0, "Sat,");        
    break;
  }
}

void drawDate()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDS3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(dayOfMonth)
  {
  case 1:
    dmd.drawString( 20,0, "01");
    break;
  case 2:
    dmd.drawString( 20,0, "02");
    break;
  case 3:
    dmd.drawString( 20,0, "03");
    break;
  case 4:
    dmd.drawString( 20,0, "04");
    break;
  case 5:
    dmd.drawString( 20,0, "05");
    break;
  case 6:
    dmd.drawString( 20,0, "06");
    break;
  case 7:
    dmd.drawString( 20,0, "07");
    break;
  case 8:
    dmd.drawString( 20,0, "08");
    break;
  case 9:
    dmd.drawString( 20,0, "09");
    break;
  case 10:
    dmd.drawString( 20,0, "10");
    break;
  case 11:
    dmd.drawString( 20,0, "11");
    break;
  case 12:
    dmd.drawString( 20,0, "12");
    break;
  case 13:
    dmd.drawString( 20,0, "13");
    break;
  case 14:
    dmd.drawString( 20,0, "14");
    break;
  case 15:
    dmd.drawString( 20,0, "15");
    break;
  case 16:
    dmd.drawString( 20,0, "16");
    break;
  case 17:
    dmd.drawString( 20,0, "17");
    break;
  case 18:
    dmd.drawString( 20,0, "18");
    break;
  case 19:
    dmd.drawString( 20,0, "19");
    break;
  case 20:
    dmd.drawString( 20,0, "20");
    break;
  case 21:
    dmd.drawString( 20,0, "21");
    break;
  case 22:
    dmd.drawString( 20,0, "22");
    break;
  case 23:
    dmd.drawString( 20,0, "23");
    break;
  case 24:
    dmd.drawString( 20,0, "24");
    break;
  case 25:
    dmd.drawString( 20,0, "25");
    break;
  case 26:
    dmd.drawString( 20,0, "26");
    break;
  case 27:
    dmd.drawString( 20,0, "27");
    break;
  case 28:
    dmd.drawString( 20,0, "28");
    break;
  case 29:
    dmd.drawString( 20,0, "29");
    break;
  case 30:
    dmd.drawString( 20,0, "30");
    break;
  case 31:
    dmd.drawString( 20,0, "31");
    break;
  }
}

void drawHour()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDS3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  if (hour>12) //Remove this if command for 24 hour time
  {
    hour=hour-12; // This is a 12 hour Clock 
  }
  switch(hour)
  {
  case 1:
    dmd.drawString( 1,9, "01");
    break;
  case 2:
    dmd.drawString( 1,9, "02");
    break;
  case 3:
    dmd.drawString( 1,9, "03");
    break;
  case 4:
    dmd.drawString( 1,9, "04");
    break;
  case 5:
    dmd.drawString( 1,9, "05");
    break;
  case 6:
    dmd.drawString( 1,9, "06");
    break;
  case 7:
    dmd.drawString( 1,9, "07");
    break;
  case 8:
    dmd.drawString( 1,9, "08");
    break;
  case 9:
    dmd.drawString( 1,9, "09");
    break;
  case 10:
    dmd.drawString( 1,9, "10");
    break;
  case 11:
    dmd.drawString( 1,9, "11");
    break;
  case 12:
    dmd.drawString( 1,9, "12");
    break;
  case 13:
    dmd.drawString( 1,9, "13");
    break;
  case 14:
    dmd.drawString( 1,9, "14");
    break;
  case 15:
    dmd.drawString( 1,9, "15");
    break;
  case 16:
    dmd.drawString( 1,9, "16");
    break;
  case 17:
    dmd.drawString( 1,9, "17");
    break;
  case 18:
    dmd.drawString( 1,9, "18");
    break;
  case 19:
    dmd.drawString( 1,9, "19");
    break;
  case 20:
    dmd.drawString( 1,9, "20");
    break;
  case 21:
    dmd.drawString( 1,9, "21");
    break;
  case 22:
    dmd.drawString( 1,9, "22");
    break;
  case 23:
    dmd.drawString( 1,9, "23");
    break;
  case 24:
    dmd.drawString( 1,9, "24");
    break;
  }
}

void drawMinute()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDS3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  switch(minute)
  {
  case 0:
    dmd.drawString( 20,9, "00");
    break;
  case 1:
    dmd.drawString( 20,9, "01");
    break;
  case 2:
    dmd.drawString( 20,9, "02");
    break;
  case 3:
    dmd.drawString( 20,9, "03");
    break;
  case 4:
    dmd.drawString( 20,9, "04");
    break;
  case 5:
    dmd.drawString( 20,9, "05");
    break;
  case 6:
    dmd.drawString( 20,9, "06");
    break;
  case 7:
    dmd.drawString( 20,9, "07");
    break;
  case 8:
    dmd.drawString( 20,9, "08");
    break;
  case 9:
    dmd.drawString( 20,9, "09");
    break;
  case 10:
    dmd.drawString( 20,9, "10");
    break;
  case 11:
    dmd.drawString( 20,9, "11");
    break;
  case 12:
    dmd.drawString( 20,9, "12");
    break;
  case 13:
    dmd.drawString( 20,9, "13");
    break;
  case 14:
    dmd.drawString( 20,9, "14");
    break;
  case 15:
    dmd.drawString( 20,9, "15");
    break;
  case 16:
    dmd.drawString( 20,9, "16");
    break;
  case 17:
    dmd.drawString( 20,9, "17");
    break;
  case 18:
    dmd.drawString( 20,9, "18");
    break;
  case 19:
    dmd.drawString( 20,9, "19");
    break;
  case 20:
    dmd.drawString( 20,9, "20");
    break;
  case 21:
    dmd.drawString( 20,9, "21");
    break;
  case 22:
    dmd.drawString( 20,9, "22");
    break;
  case 23:
    dmd.drawString( 20,9, "23");
    break;
  case 24:
    dmd.drawString( 20,9, "24");
    break;
  case 25:
    dmd.drawString( 20,9, "25");
    break;
  case 26:
    dmd.drawString( 20,9, "26");
    break;
  case 27:
    dmd.drawString( 20,9, "27");
    break;
  case 28:
    dmd.drawString( 20,9, "28");
    break;
  case 29:
    dmd.drawString( 20,9, "29");
    break;
  case 30:
    dmd.drawString( 20,9, "30");
    break;
  case 31:
    dmd.drawString( 20,9, "31");
    break;
  case 32:
    dmd.drawString( 20,9, "32");
    break;
  case 33:
    dmd.drawString( 20,9, "33");
    break;
  case 34:
    dmd.drawString( 20,9, "34");
    break;
  case 35:
    dmd.drawString( 20,9, "35");
    break;
  case 36:
    dmd.drawString( 20,9, "36");
    break;
  case 37:
    dmd.drawString( 20,9, "37");
    break;
  case 38:
    dmd.drawString( 20,9, "38");
    break;
  case 39:
    dmd.drawString( 20,9, "39");
    break;
  case 40:
    dmd.drawString( 20,9, "40");
    break;
  case 41:
    dmd.drawString( 20,9, "41");
    break;
  case 42:
    dmd.drawString( 20,9, "42");
    break;
  case 43:
    dmd.drawString( 20,9, "43");
    break;
  case 44:
    dmd.drawString( 20,9, "44");
    break;
  case 45:
    dmd.drawString( 20,9, "45");
    break;
  case 46:
    dmd.drawString( 20,9, "46");
    break;
  case 47:
    dmd.drawString( 20,9, "47");
    break;
  case 48:
    dmd.drawString( 20,9, "48");
    break;
  case 49:
    dmd.drawString( 20,9, "49");
    break;
  case 50:
    dmd.drawString( 20,9, "50");
    break;
  case 51:
    dmd.drawString( 20,9, "51");
    break;
  case 52:
    dmd.drawString( 20,9, "52");
    break;
  case 53:
    dmd.drawString( 20,9, "53");
    break;
  case 54:
    dmd.drawString( 20,9, "54");
    break;
  case 55:
    dmd.drawString( 20,9, "55");
    break;
  case 56:
    dmd.drawString( 20,9, "56");
    break;
  case 57:
    dmd.drawString( 20,9, "57");
    break;
  case 58:
    dmd.drawString( 20,9, "58");
    break;
  case 59:
    dmd.drawString( 20,9, "59");
    break;
  }
}

void drawCol()
{
  dmd.drawString( 14,9, ":");
}


void runClock()
{
  drawDay();  
  drawDate();
  drawHour();
  drawCol();
  drawMinute();
  checkAlarm();
}

void alarmSequence() {
  drawDay();  
  drawDate();
  drawHour();
  drawCol();
  drawMinute();
  digitalWrite(Alarm, HIGH);
  delay(1000);
  dmd.clearScreen();
  digitalWrite(Alarm, LOW);
  delay(1000);
  digitalWrite(Alarm, HIGH); // alaram will be off after 1 minute
}

void checkAlarm() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDS3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 
  if(dayOfWeek == 1 || dayOfWeek == 2 || dayOfWeek == 3 || dayOfWeek == 4 || dayOfWeek == 5 ||dayOfWeek == 6  ||dayOfWeek == 7) { //This is days ALLOWED to ring alarm
    if(hour == 4 && minute == 28 ) { //Set Time Here (In 24 hour Time) 
      if(AlarmSet == LOW) { //If alarm pin is on
        {
          alarmSequence();
        }
      }
    }
  }
}

void airPressure(){
  Serial.begin(9600);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  float a =  (scale.get_units());
  dmd.clearScreen();
  dmd.drawString( 0, 0, "Air P.");
  dmd.drawString( 0, 9, String(a));
  //box.println(a);
  Serial.println(a);
  //delay(2000);
}

void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn airPressure
    airPressure();
  } else {
    // turn runClock();
    runClock();
  }

  //runClock();
  //delay(5000);
  //dmd.clearScreen();
  //airPressure();
  //delay(1000);
  //dmd.clearScreen();
}
