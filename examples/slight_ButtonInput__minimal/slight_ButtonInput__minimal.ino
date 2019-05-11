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

bool mybutton_get_input(byte id, byte pin) {
    // read input + invert: button closes to GND.
    return !digitalRead(pin);
}


void mybutton_event(slight_ButtonInput *instance, byte event) {
    Serial.print(F("Instance ID:"));
    Serial.println((*instance).getID());

    Serial.print(F("Event: "));
    (*instance).printEvent(Serial, event);
    Serial.println();

    // react on event
    switch (event) {
        case slight_ButtonInput::event_StateChanged : {
            Serial.print(F("\t state: "));
            (*instance).printState(Serial);
            Serial.println();
        } break;
        case slight_ButtonInput::event_down : {
            Serial.print(F("the button is pressed down! do something.."));
        } break;
        case slight_ButtonInput::event_holddown : {
            Serial.print(F("duration active: "));
            Serial.println((*instance).getDurationActive());
        } break;
        case slight_ButtonInput::event_up : {
            Serial.print(F("up"));
        } break;
        case slight_ButtonInput::event_click : {
            Serial.print(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long"));
        } break;
        case slight_ButtonInput::event_click_double : {
            Serial.print(F("click double"));
        } break;
        case slight_ButtonInput::event_click_triple : {
            Serial.print(F("click triple"));
        } break;
        case slight_ButtonInput::event_click_multi : {
            Serial.print(F("click count: "));
            Serial.println((*instance).getClickCount());
        } break;
    } //end switch
}

// using default values:
slight_ButtonInput mybutton1(1, A0, mybutton_get_input, mybutton_event);

slight_ButtonInput mybutton2(
    // byte id_new
    2,
    // byte pin_new,
    A1,
    // tCbfuncGetInput cbfuncGetInput_new,
    mybutton_get_input,
    // tcbfOnEvent cbfCallbackOnEvent_new,
    mybutton_event,
    // const uint16_t duration_debounce_new = 30,
      30,
    // const uint16_t duration_holddown_new = 1000,
    1000,
    // const uint16_t duration_click_single_new =   50,
      50,
    // const uint16_t duration_click_long_new =   3000,
    3000,
    // const uint16_t duration_click_double_new = 1000
     500
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
