/******************************************
    slight_Button Library
        for more information see slight_Button.cpp
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

// #undef min
// #undef max
//
// // tries to fix
// // undefined reference to `std::__throw_bad_function_call()'
// // found at
// // https://forum.arduino.cc/index.php?topic=382211.msg2790687#msg2790687
// namespace std {
//     void __throw_bad_function_call() {
//         // Serial.println(F("STL ERROR - __throw_bad_function_call"));
//     }
// }
//
// #include <functional>

/** Class Definition: **/

class slight_ButtonInput {
    public:
        // typedefs:

        // call back functions
        typedef boolean (* tCallbackFunctionGetInput) (slight_ButtonInput *instance);
        typedef void (* tCallbackFunction) (slight_ButtonInput *instance);
        // using tCallbackFunction = std::function<void(slight_ButtonInput *instance)>;

        // definitions:

        // init
        static const uint8_t event_NoEvent =  0;

        // State
        static const uint8_t event_StateChanged = 10;

        static const uint8_t state_NotValid = 11;
        static const uint8_t state_Standby = 12;
        static const uint8_t state_Debouncing = 13;
        static const uint8_t state_Active = 14;
        static const uint8_t state_PostActive = 15;

        // Click
        static const uint8_t event_down = 20;
        static const uint8_t event_up = 21;
        static const uint8_t event_holddown = 22;
        static const uint8_t event_click = 30;
        static const uint8_t event_click_long = 31;
        static const uint8_t event_click_double = 32;
        static const uint8_t event_click_triple = 33;
        static const uint8_t event_click_multi = 34;

        //Constructor
        slight_ButtonInput(
            uint8_t id_new,
            uint8_t pin_new,
            tCallbackFunctionGetInput callbackGetInput_new,
            tCallbackFunction callbackOnEvent_new,
            const uint16_t duration_debounce_new = 20,
            const uint16_t duration_holddown_new = 1000,
            const uint16_t duration_click_long_new = 3000,
            const uint16_t duration_click_double_new = 200
        );



        //Destructor
        ~slight_ButtonInput();


        // initialize class
        void begin();

        // check if class is ready to operate.
        boolean isReady();

        // getState
        uint8_t getState();
        uint8_t printState(Print &out);
        uint8_t printState(Print &out, uint8_t state_ext);

        // event
        uint8_t getEventLast();
        uint8_t printEventLast(Print &out);
        uint8_t printEvent(Print &out, uint8_t state_temp);

        // main update
        uint8_t update();

        // enable / disable Input
        void enable();
        void disable();

        uint32_t getDurationActive();

        uint8_t getClickCount();

        const uint8_t id;
        const uint8_t pin;
        boolean flag_filter_multi_click_events = false;

    private:
        // flag to check if the begin function is already called and the class is ready to work.
        boolean ready;

        // internal state
        uint8_t state;

        // event
        uint8_t event;
        uint8_t event_last;
        const tCallbackFunction callbackOnEvent;
        const tCallbackFunctionGetInput callbackGetInput;

        // other things

        // events enabled?
        boolean enabled;


        // durations
        const uint16_t duration_debounce;
        const uint16_t duration_holddown;
        const uint16_t duration_click_long;
        const uint16_t duration_click_double;

        // input timing
        uint32_t timestamp_last_activity;
        uint32_t timestamp_last_release;
        uint32_t timestamp_last_holddown_event;

        //
        uint32_t duration_Active;

        uint8_t click_count;

        void generateEvent(uint8_t event_new);

        uint8_t handle_button();
        uint8_t handle_start_debouncing();
        uint8_t handle_debouncing();
        uint8_t handle_active();
        uint8_t handle_button_released();
        uint8_t handle_post_active();
};

#endif // ifndef slight_ButtonInput_h

/** the end **/
