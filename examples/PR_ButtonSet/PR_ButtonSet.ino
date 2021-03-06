
#undefine	PR_BTN_ASYNC_MODE
#include <PR_Button.hpp>

#define	BTN1_PIN	1
#define	BTN2_PIN	2

PR_ButtonSetClass  btn1(BTN1_PIN, LOW, true);
PR_ButtonSetClass  btn2(BTN2_PIN, LOW, true);


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

	PR_ButtonSetClass::loop();
    //PR_ButtonSetClass.loop(); ???
	
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
