#include <FastLED.h>
#include <Wire.h>
#include "DS1307.h"

DS1307 clock;//define a object of DS1307 class

//INIT LED STRIP
#define LED_PIN     2
#define NUM_LEDS    37
int brightnessDay = 180;
int brightnessNight = 80;

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int minutesLed[12][3] = {{0,1,2},{5,4,3},{6,7,8},{11,10,9},{12,13,14},{17,16,15},{18,19,20}
,{23,22,21},{24,25,26},{29,28,27},{30,31,32},{35,34,33}} ;
int middleLed[1] = {36};
 int currentHour = 21 ;
 int currentMinutes  = 50;

CRGBPalette16 currentPalette = RainbowColors_p;

void setup() {
   Serial.begin(9600);
   clock.begin();
  // put your setup code here, to run once:
   FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
   FastLED.setBrightness(  getBrightness() );
   startDisplay();
    //setTime();

}
int colorIndex = 0;
void loop() {
 EVERY_N_MILLISECONDS( 20 ) { colorIndex++;
 if (colorIndex > 255) colorIndex= 0;
 } 
  FastLED.clear();
  updateTime();
  
  displayMinutes(currentMinutes);
  displayHour(currentHour);
  displayMiddleLed();

   //looptest(); //only for test
   

  
 FastLED.show();
 delay(200 );   
}

void updateTime(){
  // Get data from the DS1302
  clock.getTime();
   currentHour = clock.hour;
   currentMinutes = clock.minute;
  
  Serial.print(currentHour, DEC);
  Serial.print(":");
  Serial.println(currentMinutes, DEC);
   Serial.println(clock.second);
}
void setTime(){
  clock.fillByYMD(2019,9,30);//Jan 19,2013
  clock.fillByHMS(0,0,00);//15:28 30"
  clock.fillDayOfWeek(MON);//Saturday
  clock.setTime();//write time to the RTC chip
}
void looptest(){
  
  currentMinutes++;
  if (currentMinutes > 59){
    currentMinutes = 0;
    currentHour ++;
  }
  
  if ( currentHour> 23 ) {
    currentHour = 0;
  }
 
}


void displayHour(int hourN) { //0 -> 23
  if ( hourN >= 12 )  hourN -= 12;
       int range =  map(hourN,0,11,0,11) ;
      leds[minutesLed[range][0]] = ColorFromPalette( currentPalette, colorIndex + 200, getBrightness(), LINEARBLEND);   
      leds[minutesLed[range][1]] = ColorFromPalette( currentPalette, colorIndex + 200, getBrightness(), LINEARBLEND);   
    
}
void displayMinutes(int minutes) { // 0 -> 59
       int range =  map(minutes,0,60,0,12) ;
   for( int i = 0; i < 3; i++) {
         leds[minutesLed[range][i]] = ColorFromPalette( currentPalette, colorIndex, getBrightness(), LINEARBLEND);   
    }
}
void displayMiddleLed(){
  leds[middleLed[0]] =   ColorFromPalette( currentPalette, colorIndex + 100, getBrightness(), LINEARBLEND);   
}


int getBrightness(){
  if ( currentHour > 6 && currentHour < 21 ){
    return brightnessDay;
  }
  else {
    return brightnessNight;
  }
} 

void startDisplay()
{
  for( int i = 0; i < 1000; i++) {
  fill_rainbow( leds, NUM_LEDS, i, 7);
    FastLED.show();
    delay(1);
    
  }
  

   
}
