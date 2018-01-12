#pragma once

#include <arduino.h>
#include "PR_Button_Config.h"


typedef	uint16_t	BntIntervals_t;

class PR_ButtonClass {
	public:
		PR_ButtonClass();
		PR_ButtonClass (const uint8_t pin, const bool valuePressed, const bool enablePull);
		
		void		setup(const uint8_t pin, const bool valuePressed, const bool enablePull);
		
		void		setupIntervals(const BntIntervals_t clickInterval, const BntIntervals_t dblClickInterval,  
								const BntIntervals_t longPressInterval, const BntIntervals_t longReliasedInterval);
		void		loop();				
		
		bool		isPressed()			{ return _state.b.pressed; 			}  
		bool		isReliased()		{ return !_state.b.pressed; 		}  
		bool		isLongPressed()		{ return _state.b.longPressed; 		}
		bool 		isLongReliased()	{ return _state.b.longReliased; 	}				
		bool		isOnClick();
		bool		isOnDblClick();		
		bool 		isOnFall();
		bool 		isOnRise();
		bool 		isOnLongPressed();
		bool 		isOnLongReliased();
		
		uint16_t	getState()			{ return _state.value;	}
		String		getVerboseState();	

	protected:
		uint8_t		_pin;
		bool 		_offValue;
									
		static BntIntervals_t 	_clickInterval;
		static BntIntervals_t 	_dblClickInterval;
		static BntIntervals_t 	_longPressInterval;
		static BntIntervals_t 	_longReliasedInterval;
		
		union State {
			uint16_t value;
			struct	part {
				uint8_t			cont;
				uint8_t			events;
			} part;
			struct b {
				uint16_t 		pressed			:1;
				uint16_t 		longPressed		:1;
				uint16_t 		longReliased	:1;
				uint16_t 						:0;	//unnamed to make viravnivanije
				
				uint16_t 		onRising		:1;
				uint16_t 		onFalling		:1;
				uint16_t 		onClick			:1;
				uint16_t 		onDblClick		:1;
				uint16_t 		onlongPressed	:1;
				uint16_t 		onlongReliased	:1;
			} b;
		};
		State		_state;

	private:
		uint8_t			__bounce;
		BntIntervals_t	_lastMillis;
		
		uint8_t			readDebounced();
		BntIntervals_t	_getDuration();
		
		enum	StateSM	 {	PR_RELIASED,	//0
			PR_LONGRELIASED, 		//1
			PR_ON_RISING, 			//2
			PR_PRESSED,				//3
			PR_ON_FALLING,			//4
			PR_LONGPRESSED, 		//5
			PR_ON_CLICK, 			//6
			PR_ON_DBLCLICK, 		//7
			PR_ON_LONGPRESSED,		//8
			PR_ON_LONGRELIASED		//9
		};	
		
		StateSM	_stateMachine = PR_RELIASED;
		
		
};
		

BntIntervals_t 	PR_ButtonClass::_clickInterval;
BntIntervals_t 	PR_ButtonClass::_dblClickInterval;
BntIntervals_t 	PR_ButtonClass::_longPressInterval;
BntIntervals_t 	PR_ButtonClass::_longReliasedInterval;
			
				
PR_ButtonClass::PR_ButtonClass()	{
		
		setupIntervals(PR_BTN_CLICK_INTERVAL, PR_BTN_DBLCLICK_INTERVAL, PR_BTN_LONG_PRESS_INTERVAL, 
				PR_BTN_LONG_RELIASED_INTERVAL);
		_pin = 0;										//unexisting pin number	
		_lastMillis = millis();
}

PR_ButtonClass::PR_ButtonClass (const uint8_t pin, const bool valuePressed, const bool enablePull) {
		
		setupIntervals(PR_BTN_CLICK_INTERVAL, PR_BTN_DBLCLICK_INTERVAL, PR_BTN_LONG_PRESS_INTERVAL, 
						PR_BTN_LONG_RELIASED_INTERVAL);
		setup(pin, valuePressed, enablePull);
		_lastMillis = millis();
}

void 	PR_ButtonClass::setup(const uint8_t pin, const bool valuePressed, const bool enablePull) {
		_pin = pin;
		_offValue = !valuePressed;				//offValue = HIGH = true means switch connected to GND	
		if (enablePull) pinMode(_pin, INPUT_PULLUP); else pinMode(_pin, INPUT);		
}

void	PR_ButtonClass::setupIntervals(const BntIntervals_t clickInterval, const BntIntervals_t dblClickInterval, 
										const BntIntervals_t longPressInterval, const BntIntervals_t longReliasedInterval)	{
		_clickInterval = clickInterval;
		_dblClickInterval = dblClickInterval;
		_longPressInterval = longPressInterval;
		_longReliasedInterval = longReliasedInterval;
}

inline uint8_t	PR_ButtonClass::readDebounced() {
		__bounce = __bounce << 1;
		__bounce |= digitalRead(_pin);
		switch (__bounce) {
			case	0:	return _offValue;	//last 8 times read "0"
			case  255:	return !_offValue;	//last 8 times read "1"
		}
		return _state.b.pressed;	//return current (unchanged) pressed state
}

void	PR_ButtonClass::loop() {
		
		#ifndef PR_BTN_ASYNC_MODE 	//clear "event" statuses
			_state.part.events = 0;
		#endif

		switch (_stateMachine) {			
			case	PR_RELIASED: {	// 0 действия в этом состоянии
					_state.part.cont = 0;
				}
				{	// условия перехода в  др состояния
					if ( _getDuration() > _longReliasedInterval ) _stateMachine = PR_ON_LONGRELIASED;
					if ( readDebounced() ) _stateMachine = PR_ON_RISING;
				} 
				break;
			case	PR_ON_LONGRELIASED: {	// 1 действия в этом состоянии
					_state.b.longReliased = 1;
					_state.b.onlongReliased = 1;
					readDebounced();
				}
				{	// условия перехода в  др состояния
					_stateMachine = PR_LONGRELIASED;
				} 
				break;				
			case	PR_LONGRELIASED: {	// 9 действия в этом состоянии
					//_state.b.longReliased = 1;				
				}
				{	// условия перехода в  др состояния
					if ( readDebounced() ) _stateMachine = PR_ON_RISING;
				} 
				break;				
			case	PR_ON_RISING: {	// 2 действия в этом состоянии
					_state.b.longReliased = 0;
					_state.b.pressed = 1;
					_state.b.onRising = 1;
					readDebounced();		
				}
				{ 	// условия перехода в  др состояния
					if ( _getDuration() < _dblClickInterval ) _stateMachine = PR_ON_DBLCLICK;
					else { 
						_lastMillis = millis();	
						_stateMachine = PR_PRESSED;
					}
				}
				break; 
			case	PR_PRESSED: {	// 3 действия в этом состоянии    
					//_state.b.pressed = 1;	
				}
				{ 	// условия перехода в  др состояния
					if ( _getDuration() > _longPressInterval ) _stateMachine = PR_ON_LONGPRESSED;
					else if ( !readDebounced() )	_stateMachine = PR_ON_FALLING;
				}
				break; 
			case	PR_ON_FALLING: {	// 4 действия в этом состоянии
					_state.b.pressed = 0;
					_state.b.onFalling = 1;
					readDebounced();
				}
				{ 	// условия перехода в  др состояния
					if ( _getDuration() < _clickInterval ) _stateMachine = PR_ON_CLICK;
					else {
						_lastMillis = millis();
						_stateMachine = PR_RELIASED;
					}	
				}			
				break;
			case	PR_ON_LONGPRESSED: {	// 8 действия в этом состоянии
					_state.b.longPressed = 1;
					_state.b.onlongPressed = 1;
					readDebounced();	
				}
				{	// условия перехода в  др состояния
					_stateMachine = PR_LONGPRESSED;
				} 
				break;		
			case	PR_LONGPRESSED: {		// 5 действия в этом состоянии
					_state.b.longPressed = 1;
				}
				{ 	// условия перехода в  др состояния
					if ( !readDebounced() ) 	_stateMachine = PR_ON_FALLING;
				}
				break; 			
			case	PR_ON_CLICK: {			// 6 действия в этом состоянии
					//_state.b.longPressed = 0;
					_state.b.onClick = 1;
					readDebounced();
				}
				{ 	// условия перехода в  др состояния
					_stateMachine = PR_RELIASED;
				}
				break; 			
			case	PR_ON_DBLCLICK: {	// 7 действия в этом состоянии
					_state.b.onDblClick = 1;
					readDebounced();
				}
				{ 	// условия перехода в  др состояния
					_stateMachine = PR_PRESSED;
				}
				break; 					
		} //switch	
}// update()

bool	PR_ButtonClass::isOnClick()	{
	bool	val = _state.b.onClick;
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onClick = 0;
	#endif
	return	val;							
}
bool	PR_ButtonClass::isOnDblClick()	{ 
	bool	val = _state.b.onDblClick;
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onDblClick = 0;
	#endif
	return	val;									
}			
bool 	PR_ButtonClass::isOnFall()	{
	bool	val = _state.b.onFalling;
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onFalling = 0;
	#endif
	return	val;							
}
bool 	PR_ButtonClass::isOnRise()	{
	bool	val = _state.b.onRising; 
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onRising = 0;
	#endif
	return	val;	
}
bool 	PR_ButtonClass::isOnLongPressed()	{
	bool	val = _state.b.onlongPressed; 
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onlongPressed = 0;
	#endif
	return	val;	
}
bool 	PR_ButtonClass::isOnLongReliased()	{
	bool	val = _state.b.onlongReliased; 
	#ifdef PR_BTN_ASYNC_MODE //clear flag			
		_state.b.onlongReliased = 0;
	#endif
	return	val;	
}

String		PR_ButtonClass::getVerboseState() {
	String	str;
	str = "state: ";
	if ( isPressed() ) str += "PRSD "; else str += "RLSD ";
	if ( isLongPressed() ) str += "LPRSD "; 
	else if ( isLongReliased() ) str += "LRLSD ";
		else str += "    ";
	
	str += "event: ";
	if ( isOnRise() ) 			str += "RISE  "; else str += "      ";
	if ( isOnFall() ) 			str += "FALL  "; else str += "      ";
	if ( isOnClick() ) 			str += "CLCK  "; else str += "      ";
	if ( isOnDblClick() ) 		str += "DCLCK "; else str += "      ";
	if ( isOnLongPressed() ) 	str += "LPRDD "; else str += "      ";
	if ( isOnLongReliased() ) 	str += "LRLSD "; else str += "      ";
	
	return str;
}


BntIntervals_t PR_ButtonClass::_getDuration()	{
	uint32_t tmp = millis();
	if ( tmp > _lastMillis )  // Check if the system timer has wrapped.
		return (BntIntervals_t)(tmp - _lastMillis);  // No wrap.
	else
		return (BntIntervals_t)(0xFFFFFFFF - _lastMillis + tmp);  // Has wrapped.
}





		