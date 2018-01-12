
#ifdef	AVR
	#include <MsTimer2.h>
#endif

#ifdef	ESP8266
	#include <Ticker.h>
	Ticker shedule;
#endif

/* use async mode, means buttons state update  async to readout them 
*/ 
#define	PR_BTN_ASYNC_MODE
#include <PR_Button.hpp>

#define	BTN1_PIN	1
#define	BTN2_PIN	2

PR_ButtonClass  btn1(BTN1_PIN, LOW, true);
PR_ButtonClass  btn2();


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

btn2.setup(BTN2_PIN, LOW, true);

#ifdef	AVR
	MsTimer2::set(500, ButtonsLoop); // 500ms period
	MsTimer2::start();
#endif
#ifdef	ESP8266
	shedule.attach(0.1, ButtonsLoop);
#endif

}

void	ButtonsLoop() {
	btn1.loop();
	btn2.loop();
}

void loop() {
  // put your main code here, to run repeatedly:

	Serial.print(btn1.getState(), BIN);										Serial.print("\t");
	
    if ( btn1.isPressed() ) 	Serial.print( "btn1.isPressed()" );			Serial.print("\t");
    if ( btn1.isLongPressed() ) Serial.print( "btn1.isLongPressed()" );		Serial.print("\t");
	if ( btn1.isLongReliased() )Serial.print( "btn1.isLongReliased()" );	Serial.print("\t");
    if ( btn1.isOnClick() ) 	Serial.print( "btn1.isOnClick()" );			Serial.print("\t");
    if ( btn1.isOnDblClick() ) 	Serial.print( "btn1.isOnDblClick()" );		Serial.print("\t");
    if ( btn1.isOnFall() ) 		Serial.print( "btn1.isOnFall()" );			Serial.print("\t");
    if ( btn1.isOnRise() )		Serial.print( "btn1.isOnRise()" );			Serial.print("\t");
	Serial.println();
	
	Serial.print(btn2.getState());Serial.print("\t");
	
    if ( btn2.isPressed() ) 	Serial.print( "btn2.isPressed()" );			Serial.print("\t");
    if ( btn2.isLongPressed() ) Serial.print( "btn2.isLongPressed()" );		Serial.print("\t");
	if ( btn2.isLongReliased() )Serial.print( "btn2.isLongReliased()" );	Serial.print("\t");
    if ( btn2.isOnClick() ) 	Serial.print( "btn2.isOnClick()" );			Serial.print("\t");
    if ( btn2.isOnDblClick() ) 	Serial.print( "btn2.isOnDblClick()" );		Serial.print("\t");
    if ( btn2.isOnFall() ) 		Serial.print( "btn2.isOnFall()" );			Serial.print("\t");
    if ( btn2.isOnRise() )		Serial.print( "btn2.isOnRise()" );			Serial.print("\t");
	Serial.println();
    //delay(10);
}
