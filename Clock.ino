#include <DS3231.h>
#include "LedControlMS_mine.h"
#include <Time.h>
#include <TimeLib.h>

#define NBR_MTX 4 

int h1, h2, m1, m2, d1, d2, M1, M2;
int m2_old = 0;
int m1_old = 0;
int h1_old = 0;
int h2_old = 0;
int brightness = 255;
bool dispOn = true;
DS3231 clock;
RTCDateTime dt;
LedControl lc=LedControl(12,11,10, NBR_MTX);
int hourPin = 2;
int minPin = 3;
int datePin = 4;

void setup() {
	clock.begin();
	//clock.setDateTime(__DATE__, __TIME__);
	pinMode(hourPin, INPUT_PULLUP);  
	pinMode(minPin, INPUT_PULLUP);  
	pinMode(datePin, INPUT_PULLUP);
	for (int i=0; i< NBR_MTX; i++){
	lc.shutdown(i,false);
	/* Set the brightness to a low value */
	lc.setIntensity(i,1);
	/* and clear the display */
	lc.clearDisplay(i);
	}
	delay(100);
	lc.clearAll();
}

void addHour(){
	dt = clock.getDateTime();
	dt.hour++;
	if(dt.hour > 23){
		dt.hour = 0;
	}
	clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);

}

void addMin(){
	dt = clock.getDateTime();
	dt.minute++;
	if(dt.minute > 60){
		dt.minute = 0;
	}
	clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);

}

void setTime(){

	dt = clock.getDateTime();

	lc.setLed(2,2,7,true);
	lc.setLed(2,5,7,true);

	if (dt.hour <10){
	  h1 = 0;
	  h2 = dt.hour;
	}
	else{
	  h1=dt.hour/10;
	  h2=dt.hour % 10;
	}

	if (dt.minute < 10){
	  m1=0; //First minute LED d1splays zero 
	  m2=dt.minute; //2nd digit of minutes
	}
	else{
	  m1=dt.minute/10;
	  m2=dt.minute % 10;
	}
	 
	lc.displayChar(0,h1,-1); //Hours 1 digit to left-most (#0) LED.
	lc.displayChar(1,h2,1); //Hours 2 digit to LED #1
	lc.displayChar(2,m1,-1); //Minutes 1 digit to LED #3. (Recall that #2 is used to mark off the seconds.)
	lc.displayChar(3,m2,1);

	if (m1_old != m1){
	  lc.clearDisplay(2);
	  m1_old = m1;
	}
	if (m2_old != m2){
	  lc.clearDisplay(3);
	  m2_old = m2;
	}
	if (h1_old != h1){
	  lc.clearDisplay(0);
	  h1_old = h1;
	}
	if (h2_old != h2){
	  lc.clearDisplay(1);
	  h2_old = h2;
	}
	
}

void showSlash(bool onOff){

	lc.setLed(2,7,6,onOff);
	lc.setLed(2,6,6,onOff);
	lc.setLed(2,5,7,onOff);
	lc.setLed(2,4,7,onOff);
	lc.setLed(2,3,7,onOff);
	lc.setLed(2,2,7,onOff);
	lc.setLed(1,1,0,onOff);
	lc.setLed(1,0,0,onOff);

}

void showDate(){
	
	dt = clock.getDateTime();
	if (dt.day <10){
	  d1 = 0;
	  d2 = dt.day;
	}
	else{
	  d1=dt.day/10;
	  d2=dt.day % 10;
	}

	if (dt.month < 10){
	  M1=0; //First minute LED d1splays zero 
	  M2=dt.month; //2nd digit of minutes
	}
	else{
	  M1=dt.month/10;
	  M2=dt.month % 10;
	}
	for (int i=0; i< NBR_MTX; i++){
		lc.clearDisplay(i);
	}
	showSlash(true);
	lc.displayChar(0,d1,-1); //Hours 1 digit to left-most (#0) LED.
	lc.displayChar(1,d2,1); //Hours 2 digit to LED #1
	lc.displayChar(2,M1,-1); //Minutes 1 digit to LED #3. (Recall that #2 is used to mark off the seconds.)
	lc.displayChar(3,M2,1);
	
	delay(5000);
	showSlash(false);
}

void loop(){
	
    if (digitalRead(hourPin) == LOW){
		addHour();	
		delay(200);
	}
    if (digitalRead(minPin) == LOW){
		addMin();	
		delay(200);
	}
    if (digitalRead(datePin) == LOW){
		showDate();	
		delay(200);
	}

	setTime();
	  
}
