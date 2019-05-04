/******************************************
    slight_Button Library
        for more information changelog / history see slight_Button.cpp
    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/
******************************************/
/******************************************
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

******************************************/

//Check if this class is already there or if it must be created...
#ifndef slight_ButtonInput_h
#define slight_ButtonInput_h


/** Includes Core Arduino functionality **/
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

/** Class Definition: **/

class slight_ButtonInput {
    public:
        // public typedefs:

            // call back functions
            typedef void (* tcbfOnEvent) (slight_ButtonInput *pInstance, byte bEvent);
            //typedef void (* tCbfuncStateChanged) (byte bID, byte bState);

            typedef boolean (* tCbfuncGetInput) (byte bID, byte bPin);

        // public definitions:

            // init
            static const byte event_NoEvent                =  0;

            // State
            static const byte event_StateChanged        = 10;

            static const byte state_NotValid            = 11;
            static const byte state_Standby                = 12;
            static const byte state_Active                = 13;
            static const byte state_Debouncing            = 14;

            // Click
            static const byte event_Down                = 20;
            static const byte event_Up                    = 21;
            static const byte event_HoldingDown            = 22;
            static const byte event_Click                = 30;
            static const byte event_ClickLong            = 31;
            static const byte event_ClickDouble            = 32;
            static const byte event_ClickTriple            = 33;
            static const byte event_ClickMulti            = 34;



        // public methods

            //Constructor
            slight_ButtonInput(
                byte cbID_New,
                byte cbPin_New,
                tCbfuncGetInput cbfuncGetInput_New,
                tcbfOnEvent cbfCallbackOnEvent_New,
                const uint16_t cwDuration_Debounce_New = 30,
                const uint16_t cwDuration_HoldingDown_New = 1000,
                const uint16_t cwDuration_ClickSingle_New = 50,
                const uint16_t cwDuration_ClickLong_New = 3000,
                const uint16_t cwDuration_ClickDouble_New = 250
            );



            //Destructor
            ~slight_ButtonInput();


            // initialize class
            void begin();

            // check if class is ready to operate.
            boolean isReady();

            // get ID
            byte getID();

            // getState
            byte getState();
            byte printState(Print &pOut);
            byte printState(Print &pOut, byte bState_Ext);

            // event
            byte getLastEvent();
            byte printEventLast(Print &pOut);
            byte printEvent(Print &pOut, byte bStateTemp);

            // main update
            byte update();

            // enable / disable Input
            void enable();
            void disable();

            // get Pin
            byte getPin();

            unsigned long getDurationActive();

            byte getClickCount();

    private:
        // private definitions:

            // ID
            const byte cbID;

            // flag to check if the begin function is already called and the class is ready to work.
            boolean bReady;

            // internal state
            byte bState;

            // event
            byte bEvent;
            byte bEventLast;
            const tcbfOnEvent cbfCallbackOnEvent;

            // other things

            // events enabled?
            bool bEnabled;

            // input pin
            const byte cbPin;
            // get input state
            const tCbfuncGetInput cbfuncGetInput;

            // durations
            const uint16_t cwDuration_Debounce;
            const uint16_t cwDuration_HoldingDown;
            const uint16_t cwDuration_ClickSingle;
            const uint16_t cwDuration_ClickLong;
            const uint16_t cwDuration_ClickDouble;

            // input timing
            unsigned long ulTimeStamp_LastActivity;
            unsigned long ulTimeStamp_LastRelease;
            unsigned long ulTimeStamp_LastHoldDownEvent;

            //
            unsigned long ulDuration_Active;

            byte bClick_Count;


        // private methods

            void generateEvent(byte bEventNew);
};

#endif //ifndef slight_ButtonInput_h

/** the end **/
