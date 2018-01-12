

#include <PR_Button.hpp>

#define	BTN1_PIN	9
#define	BTN2_PIN	2

PR_ButtonClass  btn1(BTN1_PIN, LOW, true);
PR_ButtonClass  btn2;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

btn2.setup(BTN2_PIN, LOW, true);

}

void loop() {
  // put your main code here, to run repeatedly:

	btn1.loop();
	//btn2.loop();

	Serial.println( btn1.getVerboseState() );
	
	//Serial.print(btn1.getState(), BIN);										Serial.print("\t");
	
    if ( btn1.isOnLongPressed() )   Serial.print( "OnLongPressed " );     //  Serial.print("\t");
    if ( btn1.isOnLongReliased() )  Serial.print( "OnLongReliased " );  //      Serial.print("\t");  
    if ( btn1.isOnClick() ) 	Serial.print( "OnClick " );		//	Serial.print("\t");
    if ( btn1.isOnDblClick() ) 	Serial.print( "OnDblClick " );	//	    Serial.print("\t");
    if ( btn1.isOnFall() ) 		Serial.print( "OnFall " );		//	    Serial.print("\t");
    if ( btn1.isOnRise() )		Serial.print( "OnRise " );		//	    Serial.print("\t");
 
    if ( btn1.isPressed() )     Serial.print( "Pressed " );         //Serial.print("\t");
    if ( btn1.isReliased() )    Serial.print( "Reliased " );         //Serial.print("\t");
	if ( btn1.isLongPressed() ) Serial.print( "LongPressed " );     //Serial.print("\t");
    if ( btn1.isLongReliased() )Serial.print( "LongReliased " );       // Serial.print("\t");

	Serial.println();//
/*	
	Serial.print(btn2.getState());Serial.print("\t");
	
    if ( btn2.isPressed() ) 	Serial.print( "btn2.isPressed()" );			Serial.print("\t");
    if ( btn2.isLongPressed() ) Serial.print( "btn2.isLongPressed()" );		Serial.print("\t");
	if ( btn2.isLongReliased() )Serial.print( "btn2.isLongReliased()" );	Serial.print("\t");
    if ( btn2.isOnClick() ) 	Serial.print( "btn2.isOnClick()" );			Serial.print("\t");
    if ( btn2.isOnDblClick() ) 	Serial.print( "btn2.isOnDblClick()" );		Serial.print("\t");
    if ( btn2.isOnFall() ) 		Serial.print( "btn2.isOnFall()" );			Serial.print("\t");
    if ( btn2.isOnRise() )		Serial.print( "btn2.isOnRise()" );			Serial.print("\t");
	Serial.println();
 */   
    
    delay(10);
}
