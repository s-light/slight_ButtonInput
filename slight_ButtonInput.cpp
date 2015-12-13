#ifdef debug__slight_ButtonInput
void print_info(Print &pOut) {
    pOut.println();
    //             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
    pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    pOut.println(F("|                       ^ ^                      |"));
    pOut.println(F("|                      (0,0)                     |"));
    pOut.println(F("|                      ( _ )                     |"));
    pOut.println(F("|                       \" \"                      |"));
    pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    pOut.println(F("| slight_ButtonInput.cpp"));
    pOut.println(F("|   library for Button Input system"));
    pOut.println(F("|"));
    pOut.println(F("| dream on & have fun :-)"));
    pOut.println(F("|"));
    pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    pOut.println(F("|"));
    //pOut.println(F("| Version: Nov 11 2013  20:35:04"));
    pOut.print(F("| version: "));
    pOut.print(F(__DATE__));
    pOut.print(F("  "));
    pOut.print(F(__TIME__));
    pOut.println();
    pOut.println(F("|"));
    pOut.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    pOut.println();

    //pOut.println(__DATE__); Nov 12 2013
    //pOut.println(__TIME__); 20:35:04
}
#endif
/**************************************************************************************************

    slight_ButtonInput
        slight_ButtonInput as event based system

    written by stefan krueger (s-light),
        stefan@s-light.eu, http://s-light.eu, https://github.com/s-light/


    changelog / history
        12.11.2013 17:18 created.
        12.11.2013 20:48 finished. system is working!
        13.11.2013 18:00 added ID
        10.01.2014 09:29 changed name to slight_ButtonInput
        10.01.2014 20:51 added Double Click detection
        11.01.2014 00:59 added printState(Print &pOut) function
        15.03.2014 15:55 changed to event system with Instance callback
        15.03.2014 22:48 redesigned events: click events are generated after a event_Up.
        15.03.2014 22:48 added event_ClickTriple, event_ClickMulti, event_HoldingDown


    TO DO:
        ~ test robustness


***************************************************************************************************/
/**************************************************************************************************
    license

    CC BY SA
        This work is licensed under the
        Creative Commons Attribution-ShareAlike 3.0 Unported License.
        To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.

    Apache License Version 2.0
        Copyright 2014 Stefan Krueger

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

    The MIT License (MIT)
        Copyright (c) 2014 stefan krueger
        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"),
        to deal in the Software without restriction, including without limitation
        the rights to use, copy, modify, merge, publish, distribute, sublicense,
        and/or sell copies of the Software, and to permit persons to whom the Software
        is furnished to do so, subject to the following conditions:
        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
        INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
        PARTICULAR PURPOSE AND NONINFRINGEMENT.
        IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
        OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
        http://opensource.org/licenses/mit-license.php

**************************************************************************************************/

/** Serial.print to Flash: Notepad++ Replace RegEx
    Find what:        Serial.print(.*)\("(.*)"\);
    Replace with:    Serial.print\1(F("\2"));
**/


/** Includes Core Arduino functionality **/
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

/** Include yourself s header file **/
#include "slight_ButtonInput.h"
// use "" for files in same directory


/**************************************************************************************************/
/**** constructor                                                                              ****/
/**************************************************************************************************/

/** Constructor **/
// initialize const http://forum.arduino.cc/index.php?topic=188261.msg1393390#msg1393390
slight_ButtonInput::slight_ButtonInput(
    byte cbID_New,
    byte cbPin_New,
    tCbfuncGetInput cbfuncGetInput_New,
    tcbfOnEvent cbfCallbackOnEvent_New,
    const uint16_t cwDuration_Debounce_New,
    const uint16_t cwDuration_HoldingDown_New,
    const uint16_t cwDuration_ClickSingle_New,
    const uint16_t cwDuration_ClickLong_New,
    const uint16_t cwDuration_ClickDouble_New
) :
    cbID                    ( cbID_New ),
    cbfCallbackOnEvent        ( cbfCallbackOnEvent_New),
    cbPin                    ( cbPin_New ),
    cbfuncGetInput            ( cbfuncGetInput_New ),
    cwDuration_Debounce        ( cwDuration_Debounce_New ),
    cwDuration_HoldingDown    ( cwDuration_HoldingDown_New ),
    cwDuration_ClickSingle    ( cwDuration_ClickSingle_New ),
    cwDuration_ClickLong    ( cwDuration_ClickLong_New ),
    cwDuration_ClickDouble    ( cwDuration_ClickDouble_New )
{
    //do some internal inits

    ulTimeStamp_LastActivity        = 0;
    ulTimeStamp_LastRelease            = 0;
    ulTimeStamp_LastHoldDownEvent    = 0;

    bClick_Count = 0;

    bEvent        = event_NoEvent;
    bEventLast    = event_NoEvent;

    bEnabled = false;

    bState = state_NotValid;
    bReady = false;
}

// Destructor
slight_ButtonInput::~slight_ButtonInput() {
  //
}

// begin (activate object/instance)
void slight_ButtonInput::begin() {
    if (bReady == false) {
        #ifdef debug_slight_ButtonInput
            Serial.println(F("***************************************************************"));
            Serial.println(F("** Welcome to slight_ButtonInput library       initialise... **"));
            Serial.println(F("***************************************************************"));
            Serial.println(F("slight_ButtonInput::begin: "));
        #endif

        ulTimeStamp_LastActivity        = millis();
        ulTimeStamp_LastRelease            = millis();
        ulTimeStamp_LastHoldDownEvent    = millis();

        bEvent        = event_NoEvent;
        bEventLast    = event_NoEvent;
        bState        = state_Standby;
        bEnabled    = true;

        bReady        = true;
    }
}


/**************************************************************************************************/
/****                                      Public methods                                      ****/
/**************************************************************************************************/

// main function
// should be called at least one time per main loop
// returns state
byte slight_ButtonInput::update() {
    byte bStateTemp = state_NotValid;
    if (bReady == true) {
        // if system is bEnabled
        if (bEnabled == true) {
            #ifdef debug_slight_ButtonInput
                //Serial.println(F("slight_ButtonInput::update():"));
            #endif

            // read input with callbackfunction
            boolean bInputRawActive = cbfuncGetInput(cbID, cbPin);

            if ( bInputRawActive == true) {
                if ( bState == state_Standby ) {
                    // start debouncing
                    ulTimeStamp_LastActivity = millis();
                    bStateTemp = state_Debouncing;
                    #ifdef debug_slight_ButtonInput
                        Serial.println(F("slight_ButtonInput::update():"));
                        Serial.println(F("\t started debouncing."));
                        Serial.print  (F("\t   bState:"));
                        Serial.println(bState);
                        Serial.print  (F("\t   bStateTemp:"));
                        Serial.println(bStateTemp);
                    #endif
                } else {
                    // system is debouncing or active
                    // so new default state is current state
                    bStateTemp = bState;

                    // calc duration
                    unsigned long ulLastDuration = ( millis() - ulTimeStamp_LastActivity );

                    // check state:
                    if ( (bState == state_Debouncing ) && ( ulLastDuration > cwDuration_Debounce ) ) {
                        // set system to 'active'
                        ulTimeStamp_LastActivity = millis();
                        ulTimeStamp_LastHoldDownEvent = millis();
                        bStateTemp = state_Active;
                        generateEvent(event_Down);
                    } else if (bState == state_Active) {
                        // check for HoldingDown event
                        ulDuration_Active = ulLastDuration;
                        unsigned long ulActiveDuration = ( millis() - ulTimeStamp_LastHoldDownEvent );
                        if( ulActiveDuration > cwDuration_HoldingDown ) {
                            ulTimeStamp_LastHoldDownEvent = millis();
                            generateEvent(event_HoldingDown);
                        }
                    }
                }
            } else if ( ( bInputRawActive == false) && ( bState == state_Active ) ) {
                // Button Released
                ulDuration_Active = ( millis() - ulTimeStamp_LastActivity );
                //bStateTemp = state_Released;
                generateEvent(event_Up);
                bStateTemp = state_Standby;

                ////////////////////////////////////////////////////////////////////////////////
                // generate Click event
                // check which event to generate (Click, ClickLong, ClickDouble, ClickTriple)

                //check if DoubleClick Time is not over
                unsigned long ulLastReleaseDuration = ( millis() - ulTimeStamp_LastRelease );
                if (ulLastReleaseDuration < cwDuration_ClickDouble) {
                    // ClickMulti detected
                        bClick_Count = bClick_Count + 1;
                    // check for ClickCount:
                    if (bClick_Count == 2) {
                        generateEvent(event_ClickDouble);
                    } else if (bClick_Count == 3) {
                        generateEvent(event_ClickTriple);
                    } else if (bClick_Count > 3) {
                        generateEvent(event_ClickMulti);
                    }
                } else {
                    // reset ClickMulti
                        bClick_Count = 1;
                    // check for Click or ClickLong
                    if (ulDuration_Active > cwDuration_ClickLong) {
                        generateEvent(event_ClickLong);
                    } else {
                        generateEvent(event_Click);
                    }
                }
                ////////////////////////////////////////////////////////////////////////////////

                // reset system
                ulTimeStamp_LastRelease    = millis();
                ulTimeStamp_LastActivity = millis();
                ulTimeStamp_LastHoldDownEvent = millis();

            } else {  // means bInputRawActive==false
                // system is in standby waiting for the next button press.
                bStateTemp = state_Standby;
            } // end if (bInputRawActive == true) else

        } else {
            // system is disabled -nothing to do - so its Standby.
            bStateTemp = state_Standby;
        } // end if bEnabled

        // check if State has Changed.
        if (bState != bStateTemp) {
            bState = bStateTemp;
            // generate state changed event
            //generateEvent(event_StateChanged);
        }

    } // end if bReady
    return bState;
}



byte slight_ButtonInput::getID() {
    return cbID;
};

boolean slight_ButtonInput::isReady() {
    return bReady;
}


byte slight_ButtonInput::getState() {
    return bState;
};

byte slight_ButtonInput::printState(Print &pOut, byte bStateTemp) {
    switch (bStateTemp) {
        case slight_ButtonInput::state_Standby : {
            pOut.print(F("standby"));
        } break;
        case slight_ButtonInput::state_NotValid : {
            pOut.print(F("NotValid"));
        } break;
        case slight_ButtonInput::state_Active : {
            pOut.print(F("active"));
        } break;
        case slight_ButtonInput::state_Debouncing : {
            pOut.print(F("debouncing"));
        } break;
        default: {
            pOut.print(F("error: '"));
            pOut.print(bState);
            pOut.print(F(" ' is not a know state."));
        }
    } //end switch
    return bStateTemp;
};

byte slight_ButtonInput::printState(Print &pOut) {
    printState(pOut, bState);
    return bState;
};


byte slight_ButtonInput::getLastEvent() {
    return bEventLast;
};

byte slight_ButtonInput::printEvent(Print &pOut, byte bEventTemp) {
    switch (bEventTemp) {
        case slight_ButtonInput::event_NoEvent : {
            pOut.print(F("no event"));
        } break;

        case slight_ButtonInput::event_StateChanged : {
            pOut.print(F("state changed"));
        } break;

        // click
        case slight_ButtonInput::event_Down : {
            pOut.print(F("down"));
        } break;
        case slight_ButtonInput::event_HoldingDown : {
            pOut.print(F("holding down"));
        } break;
        case slight_ButtonInput::event_Up : {
            pOut.print(F("up"));
        } break;
        case slight_ButtonInput::event_Click : {
            pOut.print(F("click"));
        } break;
        case slight_ButtonInput::event_ClickLong : {
            pOut.print(F("click long"));
        } break;
        case slight_ButtonInput::event_ClickDouble : {
            pOut.print(F("click double"));
        } break;
        case slight_ButtonInput::event_ClickTriple : {
            pOut.print(F("click triple"));
        } break;
        case slight_ButtonInput::event_ClickMulti : {
            pOut.print(F("click multi"));
        } break;
        default: {
            pOut.print(F("error: '"));
            pOut.print(bState);
            pOut.print(F(" ' is not a know event."));
        }
    } //end switch
    return bState;
};

byte slight_ButtonInput::printEventLast(Print &pOut) {
    printEvent(pOut, bEventLast);
    return bEventLast;
};




void slight_ButtonInput::enable() {
    if (bReady == true) {
        bEnabled = true;
    }
}

void slight_ButtonInput::disable() {
    if (bReady == true) {
        bEnabled = false;
    }
}

byte slight_ButtonInput::getPin() {
    return cbPin;
};

unsigned long slight_ButtonInput::getDurationActive() {
    return ulDuration_Active;
}

byte slight_ButtonInput::getClickCount() {
    return bClick_Count;
}



/**************************************************************************************************/
/****                                     Private methods                                      ****/
/**************************************************************************************************/


void slight_ButtonInput::generateEvent(byte bEventNew) {
    bEvent = bEventNew;
    // call event
    if (bEvent != event_NoEvent) {
        cbfCallbackOnEvent(this, bEvent);
    }
    bEventLast = bEvent;
    bEvent = event_NoEvent;
}



/****************************************************************************************************/
/** THE END                                                                                        **/
/****************************************************************************************************/
