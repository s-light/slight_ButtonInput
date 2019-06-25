
/******************************************
    slight_ButtonInput
        get button input as events

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

    old history (newer things are in git commits.)
        12.11.2013 17:18 created.
        12.11.2013 20:48 finished. system is working!
        13.11.2013 18:00 added ID
        10.01.2014 09:29 changed name to slight_ButtonInput
        10.01.2014 20:51 added Double Click detection
        11.01.2014 00:59 added printState(Print &out) function
        15.03.2014 15:55 changed to event system with Instance callback
        15.03.2014 22:48 redesigned events: click events are generated after a event_up.
        15.03.2014 22:48 added event_click_triple, event_click_multi, event_holddown
*******************************************/

/******************************************
license

------------------------------------------
Apache License Version 2.0
Copyright 2019 Stefan Krueger
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

------------------------------------------
The MIT License (MIT)

Copyright 2019 Stefan Krüger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
https://opensource.org/licenses/mit-license.php
******************************************/

/** Includes Core Arduino functionality **/
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// Include yourself s header file
#include "slight_ButtonInput.h"
// use "" for files in same directory


/******************************************/
// constructor
/******************************************/

/** Constructor **/
// initialize const http://forum.arduino.cc/index.php?topic=188261.msg1393390#msg1393390
slight_ButtonInput::slight_ButtonInput(
    byte id_new,
    byte pin_new,
    tCbfuncGetInput cbfuncGetInput_new,
    tcbfOnEvent cbfCallbackOnEvent_new,
    const uint16_t duration_debounce_new,
    const uint16_t duration_holddown_new,
    const uint16_t duration_click_single_new,
    const uint16_t duration_click_long_new,
    const uint16_t duration_click_double_new
) :
    id(id_new),
    cbfCallbackOnEvent(cbfCallbackOnEvent_new),
    pin(pin_new),
    cbfuncGetInput(cbfuncGetInput_new),
    duration_debounce(duration_debounce_new),
    duration_holddown(duration_holddown_new),
    duration_click_single(duration_click_single_new),
    duration_click_long(duration_click_long_new),
    duration_click_double(duration_click_double_new)
{
    //do some internal inits

    timestamp_last_activity        = 0;
    timestamp_last_release            = 0;
    timestamp_last_holddown_event    = 0;

    click_count = 0;

    event        = event_NoEvent;
    event_last    = event_NoEvent;

    enabled = false;

    state = state_NotValid;
    ready = false;
}

// Destructor
slight_ButtonInput::~slight_ButtonInput() {
  //
}

// begin (activate object/instance)
void slight_ButtonInput::begin() {
    if (ready == false) {
        #ifdef debug_slight_ButtonInput
            Serial.println(F("***************************************************************"));
            Serial.println(F("** Welcome to slight_ButtonInput library       initialise... **"));
            Serial.println(F("***************************************************************"));
            Serial.println(F("slight_ButtonInput::begin: "));
        #endif

        timestamp_last_activity        = millis();
        timestamp_last_release            = millis();
        timestamp_last_holddown_event    = millis();

        event        = event_NoEvent;
        event_last    = event_NoEvent;
        state        = state_Standby;
        enabled    = true;

        ready        = true;
    }
}


/******************************************/
// Public methods
/******************************************/

// main function
// should be called at least one time per main loop
// returns state
byte slight_ButtonInput::update() {
    byte state_temp = state_NotValid;
    if (ready == true) {
        // if system is enabled
        if (enabled == true) {
            #ifdef debug_slight_ButtonInput
                //Serial.println(F("slight_ButtonInput::update():"));
            #endif

            // read input with callbackfunction
            boolean bInputRawActive = cbfuncGetInput(id, pin);

            if ( bInputRawActive == true) {
                if ( state == state_Standby ) {
                    // start debouncing
                    timestamp_last_activity = millis();
                    state_temp = state_Debouncing;
                    #ifdef debug_slight_ButtonInput
                        Serial.println(F("slight_ButtonInput::update():"));
                        Serial.println(F("\t started debouncing."));
                        Serial.print  (F("\t   state:"));
                        Serial.println(state);
                        Serial.print  (F("\t   state_temp:"));
                        Serial.println(state_temp);
                    #endif
                } else {
                    // system is debouncing or active
                    // so new default state is current state
                    state_temp = state;

                    // calc duration
                    unsigned long ulLastDuration = ( millis() - timestamp_last_activity );

                    // check state:
                    if ( (state == state_Debouncing ) && ( ulLastDuration > duration_debounce ) ) {
                        // set system to 'active'
                        timestamp_last_activity = millis();
                        timestamp_last_holddown_event = millis();
                        state_temp = state_Active;
                        generateEvent(event_down);
                    } else if (state == state_Active) {
                        // check for HoldingDown event
                        duration_Active = ulLastDuration;
                        unsigned long ulActiveDuration = ( millis() - timestamp_last_holddown_event );
                        if( ulActiveDuration > duration_holddown ) {
                            timestamp_last_holddown_event = millis();
                            generateEvent(event_holddown);
                        }
                    }
                }
            } else if ( ( bInputRawActive == false) && ( state == state_Active ) ) {
                // Button Released
                duration_Active = ( millis() - timestamp_last_activity );
                //state_temp = state_Released;
                generateEvent(event_up);
                state_temp = state_Standby;

                ////////////////////////////////////////////////////////////////////////////////
                // generate Click event
                // check which event to generate (Click, ClickLong, ClickDouble, ClickTriple)

                //check if DoubleClick Time is not over
                unsigned long ulLastReleaseDuration = ( millis() - timestamp_last_release );
                if (ulLastReleaseDuration < duration_click_double) {
                    // ClickMulti detected
                        click_count = click_count + 1;
                    // check for ClickCount:
                    if (click_count == 2) {
                        generateEvent(event_click_double);
                    } else if (click_count == 3) {
                        generateEvent(event_click_triple);
                    } else if (click_count > 3) {
                        generateEvent(event_click_multi);
                    }
                } else {
                    // reset ClickMulti
                        click_count = 1;
                    // check for Click or ClickLong
                    if (duration_Active > duration_click_long) {
                        generateEvent(event_click_long);
                    } else {
                        generateEvent(event_click);
                    }
                }
                ////////////////////////////////////////////////////////////////////////////////

                // reset system
                timestamp_last_release    = millis();
                timestamp_last_activity = millis();
                timestamp_last_holddown_event = millis();

            } else {  // means bInputRawActive==false
                // system is in standby waiting for the next button press.
                state_temp = state_Standby;
            } // end if (bInputRawActive == true) else

        } else {
            // system is disabled -nothing to do - so its Standby.
            state_temp = state_Standby;
        } // end if enabled

        // check if State has Changed.
        if (state != state_temp) {
            state = state_temp;
            // generate state changed event
            //generateEvent(event_StateChanged);
        }

    } // end if ready
    return state;
}



byte slight_ButtonInput::getID() {
    return id;
};

boolean slight_ButtonInput::isReady() {
    return ready;
}


byte slight_ButtonInput::getState() {
    return state;
};

byte slight_ButtonInput::printState(Print &out, byte state_temp) {
    switch (state_temp) {
        case slight_ButtonInput::state_Standby : {
            out.print(F("standby"));
        } break;
        case slight_ButtonInput::state_NotValid : {
            out.print(F("NotValid"));
        } break;
        case slight_ButtonInput::state_Active : {
            out.print(F("active"));
        } break;
        case slight_ButtonInput::state_Debouncing : {
            out.print(F("debouncing"));
        } break;
        default: {
            out.print(F("error: '"));
            out.print(state);
            out.print(F(" ' is not a know state."));
        }
    } //end switch
    return state_temp;
};

byte slight_ButtonInput::printState(Print &out) {
    printState(out, state);
    return state;
};


byte slight_ButtonInput::getLastEvent() {
    return event_last;
};

byte slight_ButtonInput::printEvent(Print &out, byte eventTemp) {
    switch (eventTemp) {
        case slight_ButtonInput::event_NoEvent : {
            out.print(F("no event"));
        } break;

        case slight_ButtonInput::event_StateChanged : {
            out.print(F("state changed"));
        } break;

        // click
        case slight_ButtonInput::event_down : {
            out.print(F("down"));
        } break;
        case slight_ButtonInput::event_holddown : {
            out.print(F("holding down"));
        } break;
        case slight_ButtonInput::event_up : {
            out.print(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            out.print(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            out.print(F("click long"));
        } break;
        case slight_ButtonInput::event_click_double : {
            out.print(F("click double"));
        } break;
        case slight_ButtonInput::event_click_triple : {
            out.print(F("click triple"));
        } break;
        case slight_ButtonInput::event_click_multi : {
            out.print(F("click multi"));
        } break;
        default: {
            out.print(F("error: '"));
            out.print(state);
            out.print(F(" ' is not a know event."));
        }
    } //end switch
    return state;
};

byte slight_ButtonInput::printEventLast(Print &out) {
    printEvent(out, event_last);
    return event_last;
};




void slight_ButtonInput::enable() {
    if (ready == true) {
        enabled = true;
    }
}

void slight_ButtonInput::disable() {
    if (ready == true) {
        enabled = false;
    }
}

byte slight_ButtonInput::getPin() {
    return pin;
};

unsigned long slight_ButtonInput::getDurationActive() {
    return duration_Active;
}

byte slight_ButtonInput::getClickCount() {
    return click_count;
}



/******************************************/
/****                                     Private methods                                      ****/
/******************************************/


void slight_ButtonInput::generateEvent(byte event_new) {
    event = event_new;
    // call event
    if (event != event_NoEvent) {
        cbfCallbackOnEvent(this, event);
    }
    event_last = event;
    event = event_NoEvent;
}



/********************************************/
/** THE END                                                                                        **/
/********************************************/
