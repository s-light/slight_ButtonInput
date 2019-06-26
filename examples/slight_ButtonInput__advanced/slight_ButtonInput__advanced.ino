/******************************************

    slight_ButtonInput__advanced
        minimal example for slight_ButtonInput lib usage.
        debugport: serial interface 115200baud

    hardware:
        Arduino board of any typ.
        A3 --> Pushbutton closing to GND
        A4 --> Pushbutton closing to GND
        A5 --> Pushbutton closing to GND

    libraries used:
        ~ slight_ButtonInput

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************/
/******************************************
CC BY SA
This work is licensed under the
Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.


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


// fix
// undefined reference to `std::__throw_bad_function_call()'
// found at
// https://forum.arduino.cc/index.php?topic=382211.msg2790687#msg2790687
namespace std {
    void __throw_bad_function_call() {
        Serial.println(F("STL ERROR - __throw_bad_function_call"));
    }
}
// but results in
// warning: 'noreturn' function does return [enabled by default
#include <slight_ButtonInput.h>


// ------------------------------------------
// slight_ButtonInput things

boolean mybutton_get_input(slight_ButtonInput *instance) {
    // read input + invert: button closes to GND.
    return !digitalRead((*instance).pin);
}


void mybutton_event(slight_ButtonInput *instance) {
    Serial.print(F("instance:"));
    Serial.print((*instance).id);
    Serial.print(F(" - event: "));
    (*instance).printEventLast(Serial);
    Serial.println();

    // react on event
    switch ((*instance).getEventLast()) {
        case slight_ButtonInput::event_down : {
            // Serial.println(F("down"));
        } break;
        case slight_ButtonInput::event_holddown : {
            Serial.print(F("duration active: "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_up : {
            // Serial.println(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            // Serial.println(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_click_double : {
            // Serial.println(F("click double"));
        } break;
        case slight_ButtonInput::event_click_triple : {
            // Serial.println(F("click triple"));
        } break;
        case slight_ButtonInput::event_click_multi : {
            Serial.print(F("click multi - count: "));
            Serial.println((*instance).getClickCount());
        } break;
    } //end switch
}


const uint8_t mybuttons_count = 4;
slight_ButtonInput mybuttons[mybuttons_count] = {
    slight_ButtonInput(0, A0, mybutton_get_input, mybutton_event),
    slight_ButtonInput(
        // uint8_t id_new
        1,
        // uint8_t pin_new,
        A3,
        // tCallbackFunctionGetInput callbackGetInput_new,
        mybutton_get_input,
        // tCallbackFunction callbackOnEvent_new,
        mybutton_event,
        // const uint16_t duration_debounce_new = 20,
        10,
        // const uint16_t duration_holddown_new = 1000,
        1000,
        // const uint16_t duration_click_long_new =   3000,
        500,
        // const uint16_t duration_click_double_new = 250
        250
    ),
    // using default values:
    slight_ButtonInput(2, A4, mybutton_get_input, mybutton_event, 1),
    slight_ButtonInput(3, A5, mybutton_get_input, mybutton_event, 1),
};


// ------------------------------------------
// setup
// ------------------------------------------
void setup() {
    // ------------------------------------------
    // init serial
    // wait for arduino IDE to release all serial ports after upload.
    delay(1000);
    Serial.begin(115200);
    delay(500);
    Serial.println();

    // ------------------------------------------
    // print short welcome text
    Serial.println(F("slight_ButtonInput__advanced.ino sketch."));
    Serial.println(F("minimal example for library usage."));

    // ------------------------------------------
    // start slight_ButtonInput
    Serial.println(F("setup slight_ButtonInput:")); {
        for (size_t i = 0; i < mybuttons_count; i++) {
            pinMode(mybuttons[i].pin, INPUT_PULLUP);
            mybuttons[i].begin();
            mybuttons[i].flag_filter_multi_click_events = true;
        }
    }
    Serial.println(F("  finished."));

    // ------------------------------------------
    Serial.println(F("Loop:"));
}


// ------------------------------------------
// main loop
// ------------------------------------------
void loop() {
    for (size_t i = 0; i < mybuttons_count; i++) {
        mybuttons[i].update();
    }
    // nothing else to do here...
}


// ------------------------------------------
// THE END :-)
// ------------------------------------------
