#include <Adafruit_NeoPixel.h>

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

// Display output pin assignments
#define MTEN   Display1=Display1 | (1<<0)  
#define HALF  Display1=Display1 | (1<<1)
#define QUARTER Display1=Display1 | (1<<2)
#define TWENTY  Display1=Display1 | (1<<3)
#define MFIVE Display1=Display1 | (1<<4)
#define MINUTES Display1=Display1 | (1<<5)
#define PAST  Display1=Display1 | (1<<6)
#define UNUSED1 Display1=Display1 | (1<<7)

#define TO  Display2=Display2 | (1<<0)
#define ONE Display2=Display2 | (1<<1)
#define TWO Display2=Display2 | (1<<2)
#define THREE Display2=Display2 | (1<<3)
#define FOUR  Display2=Display2 | (1<<4)
#define HFIVE Display2=Display2 | (1<<5)
#define SIX Display2=Display2 | (1<<6)
#define UNUSED2 Display2=Display2 | (1<<7)

#define SEVEN Display3=Display3 | (1<<0)
#define EIGHT Display3=Display3 | (1<<1)
#define NINE  Display3=Display3 | (1<<2)
#define HTEN  Display3=Display3 | (1<<3)
#define ELEVEN  Display3=Display3 | (1<<4)
#define TWELVE  Display3=Display3 | (1<<5)
#define OCLOCK  Display3=Display3 | (1<<6)
#define UNUSED3 Display3=Display3 | (1<<7)

char Display1=0, Display2=0, Display3=0;

// hardware constants
int LEDClockPin=6;
int LEDDataPin=7;
int LEDStrobePin=8;

//int MinuteButtonPin=2;
//int HourButtonPin=3;
int PWMPin = 9;


// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
void setup()
{
  Wire.begin();
  // initialise the hardware  
  // initialize the appropriate pins as outputs:
  pinMode(LEDClockPin, OUTPUT); 
  pinMode(LEDDataPin, OUTPUT); 
  pinMode(LEDStrobePin, OUTPUT); 
  
  //pinMode(BrightnessPin, INPUT);
  //pinMode(MinuteButtonPin, INPUT); 
  //pinMode(HourButtonPin, INPUT);
  
  pinMode(PWMPin, OUTPUT); 
  
  Serial.begin(19200);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(00,12,9,4,14,4,16);
  //displayTime();
  selftest();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void ledsoff(void)
{
 Display1=0;
 Display2=0;
 Display3=0;
}
void WriteLEDs(void) 
{
 // Now we write the actual values to the hardware
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display3);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display2);
 shiftOut(LEDDataPin, LEDClockPin, MSBFIRST, Display1);
 digitalWrite(LEDStrobePin,HIGH);
 delay(2);
 digitalWrite(LEDStrobePin,LOW); 
}
void displayTime()
{
  // start by clearing the display to a known state
  ledsoff();
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  Serial.print("It is ");
  // now we display the appropriate minute counter
  if ((minute>4) && (minute<10)) { 
    MFIVE; 
    MINUTES; 
    Serial.print("Five Minutes ");
  }
  if ((minute>9) && (minute<15)) { 
    MTEN; 
    MINUTES; 
    Serial.print("Ten Minutes ");
  }
  if ((minute>14) && (minute<20)) {
    QUARTER; 
      Serial.print("Quarter ");
  }
  if ((minute>19) && (minute<25)) { 
    TWENTY; 
    MINUTES; 
    Serial.print("Twenty Minutes ");
  }
  if ((minute>24) && (minute<30)) { 
    TWENTY; 
    MFIVE; 
    MINUTES;
    Serial.print("Twenty Five Minutes ");
  }  
  if ((minute>29) && (minute<35)) {
    HALF;
    Serial.print("Half ");
  }
  if ((minute>34) && (minute<40)) { 
    TWENTY; 
    MFIVE; 
    MINUTES;
    Serial.print("Twenty Five Minutes ");
  }  
  if ((minute>39) && (minute<45)) { 
    TWENTY; 
    MINUTES; 
    Serial.print("Twenty Minutes ");
  }
  if ((minute>44) && (minute<50)) {
    QUARTER; 
    Serial.print("Quarter ");
  }
  if ((minute>49) && (minute<55)) { 
    MTEN; 
    MINUTES; 
    Serial.print("Ten Minutes ");
  } 
  if (minute>54) { 
    MFIVE; 
    MINUTES; 
    Serial.print("Five Minutes ");
  }

  if ((minute <5))
  {
    switch (hour) {
    case 1: 
      ONE; 
      Serial.print("One ");
      break;
    case 2: 
      TWO; 
      Serial.print("Two ");
      break;
    case 3: 
      THREE; 
      Serial.print("Three ");
      break;
    case 4: 
      FOUR; 
      Serial.print("Four ");
      break;
    case 5: 
      HFIVE; 
      Serial.print("Five ");
      break;
    case 6: 
      SIX; 
      Serial.print("Six ");
      break;
    case 7: 
      SEVEN; 
      Serial.print("Seven ");
      break;
    case 8: 
      EIGHT; 
      Serial.print("Eight ");
      break;
    case 9: 
      NINE; 
      Serial.print("Nine ");
      break;
    case 10: 
      HTEN; 
      Serial.print("Ten ");
      break;
    case 11: 
      ELEVEN; 
      Serial.print("Eleven ");
      break;
    case 12: 
      TWELVE; 
      Serial.print("Twelve ");
      break;
    case 13: 
      ONE; 
      Serial.print("One ");
      break;
    case 14: 
      TWO; 
      Serial.print("Two ");
      break;
    case 15: 
      THREE; 
      Serial.print("Three ");
      break;
    case 16: 
      FOUR; 
      Serial.print("Four ");
      break;
    case 17: 
      HFIVE; 
      Serial.print("Five ");
      break;
    case 18: 
      SIX; 
      Serial.print("Six ");
      break;
    case 19: 
      SEVEN; 
      Serial.print("Seven ");
      break;
    case 20: 
      EIGHT; 
      Serial.print("Eight ");
      break;
    case 21: 
      NINE; 
      Serial.print("Nine ");
      break;
    case 22: 
      HTEN; 
      Serial.print("Ten ");
      break;
    case 23: 
      ELEVEN; 
      Serial.print("Eleven ");
      break;
    case 0: 
      TWELVE; 
      Serial.print("Twelve ");
      break;
    }
  OCLOCK;
  Serial.println("O'Clock");
  }
  else
    if ((minute < 35) && (minute >4))
    {
      PAST;
      Serial.print("Past ");
      switch (hour) {
    case 1: 
      ONE; 
      Serial.println("One ");
      break;
    case 2: 
      TWO; 
      Serial.println("Two ");
      break;
    case 3: 
      THREE; 
      Serial.println("Three ");
      break;
    case 4: 
      FOUR; 
      Serial.println("Four ");
      break;
    case 5: 
      HFIVE; 
      Serial.println("Five ");
      break;
    case 6: 
      SIX; 
      Serial.println("Six ");
      break;
    case 7: 
      SEVEN; 
      Serial.println("Seven ");
      break;
    case 8: 
      EIGHT; 
      Serial.println("Eight ");
      break;
    case 9: 
      NINE; 
      Serial.println("Nine ");
      break;
    case 10: 
      HTEN; 
      Serial.println("Ten ");
      break;
    case 11: 
      ELEVEN; 
      Serial.println("Eleven ");
      break;
    case 12: 
      TWELVE; 
      Serial.println("Twelve ");
      break;
    case 13: 
      ONE; 
      Serial.println("One ");
      break;
    case 14: 
      TWO; 
      Serial.println("Two ");
      break;
    case 15: 
      THREE; 
      Serial.println("Three ");
      break;
    case 16: 
      FOUR; 
      Serial.println("Four ");
      break;
    case 17: 
      HFIVE; 
      Serial.println("Five ");
      break;
    case 18: 
      SIX; 
      Serial.println("Six ");
      break;
    case 19: 
      SEVEN; 
      Serial.println("Seven ");
      break;
    case 20: 
      EIGHT; 
      Serial.println("Eight ");
      break;
    case 21: 
      NINE; 
      Serial.println("Nine ");
      break;
    case 22: 
      HTEN; 
      Serial.println("Ten ");
      break;
    case 23: 
      ELEVEN; 
      Serial.println("Eleven ");
      break;
    case 0: 
      TWELVE; 
      Serial.println("Twelve ");
      break;
      }
    }
    else
    {
      // if we are greater than 34 minutes past the hour then display
      // the next hour, as we will be displaying a 'to' sign
      TO;
      Serial.print("To ");
      switch (hour) {
      case 1: 
        TWO; 
       Serial.println("Two ");
       break;
      case 2: 
        THREE; 
      Serial.println("Three ");
        break;
      case 3: 
        FOUR; 
      Serial.println("Four ");
        break;
      case 4: 
        HFIVE; 
      Serial.println("Five ");
        break;
      case 5: 
        SIX; 
      Serial.println("Six ");
        break;
      case 6: 
        SEVEN; 
      Serial.println("Seven ");
        break;
      case 7: 
        EIGHT; 
      Serial.println("Eight ");
        break;
      case 8: 
        NINE; 
      Serial.println("Nine ");
        break;
      case 9: 
        HTEN; 
      Serial.println("Ten ");
        break;
      case 10: 
        ELEVEN; 
      Serial.println("Eleven ");
        break;
      case 11: 
        TWELVE; 
      Serial.println("Twelve ");
        break;
      case 12: 
        ONE; 
      Serial.println("One ");
        break;
      case 13: 
        TWO; 
      Serial.println("Two ");
        break;
      case 14: 
        THREE; 
      Serial.println("Three ");
        break;
      case 15: 
        FOUR; 
      Serial.println("Four ");
        break;
      case 16: 
        HFIVE; 
      Serial.println("Five ");
        break;
      case 17: 
        SIX; 
      Serial.println("Six ");
        break;
      case 18: 
        SEVEN; 
      Serial.println("Seven ");
        break;
      case 19: 
        EIGHT; 
      Serial.println("Eight ");
        break;
      case 20: 
        NINE; 
      Serial.println("Nine ");
        break;
      case 21: 
        HTEN; 
      Serial.println("Ten ");
        break;
      case 22: 
        ELEVEN; 
      Serial.println("Eleven ");
        break;
      case 23: 
        TWELVE; 
      Serial.println("Twelve ");
        break;
      case 0:
        ONE;
      Serial.println("One ");
      }
    }
  WriteLEDs();
}
void selftest(void){
  Serial.print("TEST");
  analogWrite(PWMPin,255);
  
  ledsoff(); MTEN; WriteLEDs(); delay(500); 
  ledsoff(); HALF; WriteLEDs(); delay(500); 
  ledsoff(); QUARTER; WriteLEDs(); delay(500); 
  ledsoff(); TWENTY; WriteLEDs(); delay(500); 
  ledsoff(); MFIVE; WriteLEDs(); delay(500); 
  ledsoff(); MINUTES; WriteLEDs(); delay(500); 
  ledsoff(); PAST; WriteLEDs(); delay(500); 
  ledsoff(); TO; WriteLEDs(); delay(500); 
  ledsoff(); ONE; WriteLEDs(); delay(500); 
  ledsoff(); TWO; WriteLEDs(); delay(500); 
  ledsoff(); THREE; WriteLEDs(); delay(500); 
  ledsoff(); FOUR; WriteLEDs(); delay(500); 
  ledsoff(); HFIVE; WriteLEDs(); delay(500); 
  ledsoff(); SIX; WriteLEDs(); delay(500); 
  ledsoff(); SEVEN; WriteLEDs(); delay(500); 
  ledsoff(); EIGHT; WriteLEDs(); delay(500); 
  ledsoff(); NINE; WriteLEDs(); delay(500); 
  ledsoff(); HTEN; WriteLEDs(); delay(500); 
  ledsoff(); ELEVEN; WriteLEDs(); delay(500); 
  ledsoff(); TWELVE; WriteLEDs(); delay(500); 
  ledsoff(); OCLOCK; WriteLEDs(); delay(500); 
}
void loop()
{
  displayTime(); // display the real-time clock data on the Serial Monitor,
  //selftest();
  delay(1000); // every second
}
