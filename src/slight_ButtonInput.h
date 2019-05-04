/******************************************
    slight_Button Library
        for more information changelog / history see slight_Button.cpp
    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/
******************************************/

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

Copyright (c) 2019 Stefan Krüger

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
            typedef void (* tcbfOnEvent) (slight_ButtonInput *instance, byte event);
            //typedef void (* tCbfuncStateChanged) (byte id, byte state);

            typedef boolean (* tCbfuncGetInput) (byte id, byte pin);

        // public definitions:

            // init
            static const byte event_NoEvent =  0;

            // State
            static const byte event_StateChanged = 10;

            static const byte state_NotValid = 11;
            static const byte state_Standby = 12;
            static const byte state_Active = 13;
            static const byte state_Debouncing = 14;

            // Click
            static const byte event_Down = 20;
            static const byte event_Up = 21;
            static const byte event_holddown = 22;
            static const byte event_Click = 30;
            static const byte event_click_long = 31;
            static const byte event_click_double = 32;
            static const byte event_ClickTriple = 33;
            static const byte event_ClickMulti = 34;



        // public methods

            //Constructor
            slight_ButtonInput(
                byte id_new,
                byte pin_new,
                tCbfuncGetInput cbfuncGetInput_new,
                tcbfOnEvent cbfCallbackOnEvent_new,
                const uint16_t duration_debounce_new = 30,
                const uint16_t duration_holddown_new = 1000,
                const uint16_t duration_click_single_new = 50,
                const uint16_t duration_click_long_new = 3000,
                const uint16_t duration_click_double_new = 250
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
            byte printState(Print &out);
            byte printState(Print &out, byte state_ext);

            // event
            byte getLastEvent();
            byte printEventLast(Print &out);
            byte printEvent(Print &out, byte state_temp);

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
            const byte id;

            // flag to check if the begin function is already called and the class is ready to work.
            boolean ready;

            // internal state
            byte state;

            // event
            byte event;
            byte event_last;
            const tcbfOnEvent cbfCallbackOnEvent;

            // other things

            // events enabled?
            bool enabled;

            // input pin
            const byte pin;
            // get input state
            const tCbfuncGetInput cbfuncGetInput;

            // durations
            const uint16_t duration_debounce;
            const uint16_t duration_holddown;
            const uint16_t duration_click_single;
            const uint16_t duration_click_long;
            const uint16_t duration_click_double;

            // input timing
            unsigned long timestamp_last_activity;
            unsigned long timestamp_last_release;
            unsigned long timestamp_last_holddown_event;

            //
            unsigned long duration_Active;

            byte click_count;


        // private methods

            void generateEvent(byte event_new);
};

#endif //ifndef slight_ButtonInput_h

/** the end **/
