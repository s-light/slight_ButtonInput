/******************************************

    slight_ButtonInput__minimal
        minimal example for slight_ButtonInput lib usage.
        debugport: serial interface 115200baud

    hardware:
        Arduino board of any typ.
        A0 --> Pushbutton closing to GND

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

#include <slight_ButtonInput.h>


// ------------------------------------------
// slight_ButtonInput things

bool mybutton_get_input(uint8_t id, uint8_t pin) {
    // read input + invert: button closes to GND.
    return !digitalRead(pin);
}


void mybutton_event(slight_ButtonInput *instance, uint8_t event) {
    // Serial.print(F("instance:"));
    // Serial.print((*instance).id);
    // Serial.print(F(" - event: "));
    // (*instance).printEvent(Serial, event);
    // Serial.println();

    // react on event
    switch (event) {
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
            Serial.println(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_click_double : {
            Serial.println(F("click double"));
        } break;
        case slight_ButtonInput::event_click_triple : {
            Serial.println(F("click triple"));
        } break;
        case slight_ButtonInput::event_click_multi : {
            Serial.print(F("click multi - count: "));
            Serial.println((*instance).getClickCount());
        } break;
    } //end switch
}

// using default values:
slight_ButtonInput mybutton1(1, A0, mybutton_get_input, mybutton_event);

slight_ButtonInput mybutton2(
    // uint8_t id_new
    2,
    // uint8_t pin_new,
    A3,
    // tCbfuncGetInput cbfuncGetInput_new,
    mybutton_get_input,
    // tcbfOnEvent cbfCallbackOnEvent_new,
    mybutton_event,
    // const uint16_t duration_debounce_new = 20,
    10,
    // const uint16_t duration_holddown_new = 1000,
    1000,
    // const uint16_t duration_click_long_new =   3000,
    500,
    // const uint16_t duration_click_double_new = 250
    250
);

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
    Serial.println(F("slight_ButtonInput__minimal.ino sketch."));
    Serial.println(F("minimal example for library usage."));

    // ------------------------------------------
    // start slight_ButtonInput
    Serial.println(F("setup slight_ButtonInput:")); {
        Serial.println(F("  pinMode INPUT_PULLUP"));
        pinMode(mybutton1.getPin(), INPUT_PULLUP);
        pinMode(mybutton2.getPin(), INPUT_PULLUP);
        Serial.println(F("  mybutton.begin();"));
        mybutton1.begin();
        mybutton2.begin();
        // mybutton1.flag_filter_multi_click_events = true;
        // mybutton2.flag_filter_multi_click_events = true;
    }
    Serial.println(F("  finished."));

    // ------------------------------------------
    Serial.println(F("Loop:"));
}


// ------------------------------------------
// main loop
// ------------------------------------------
void loop() {
    mybutton1.update();
    mybutton2.update();
    // nothing else to do here...
}


// ------------------------------------------
// THE END :-)
// ------------------------------------------
