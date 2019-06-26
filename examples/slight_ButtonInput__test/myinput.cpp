// NOLINT(legal/copyright)
/******************************************************************************

    some description...

    libraries used:
        ~ slight_ButtonInput
            written by stefan krueger (s-light),
                git@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
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
******************************************************************************/



// include Core Arduino functionality
#include <Arduino.h>

#include <slight_ButtonInput.h>

// include own headerfile
// NOLINTNEXTLINE(build/include)
#include "./myinput.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions

MyInput::MyInput() {
    ready = false;
}

MyInput::~MyInput() {
    end();
}

void MyInput::begin(Stream &out) {
    // clean up..
    end();
    // start up...
    if (ready == false) {
        // setup

        // enable
        ready = true;
    }
}

void MyInput::end() {
    if (ready) {
        // nothing to do..
    }
}

void MyInput::update() {
    if (ready) {
        // do it :-)
    }
}



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
    }  // end switch
}


// const uint8_t mybuttons_count = 4;
// slight_ButtonInput mybuttons[mybuttons_count] = {
//     slight_ButtonInput(0, A0, mybutton_get_input, mybutton_event),
//     slight_ButtonInput(
//         // uint8_t id_new
//         1,
//         // uint8_t pin_new,
//         A3,
//         // tCallbackFunctionGetInput callbackGetInput_new,
//         mybutton_get_input,
//         // tCallbackFunction callbackOnEvent_new,
//         mybutton_event,
//         // const uint16_t duration_debounce_new = 20,
//         10,
//         // const uint16_t duration_holddown_new = 1000,
//         1000,
//         // const uint16_t duration_click_long_new =   3000,
//         500,
//         // const uint16_t duration_click_double_new = 250
//         250
//     ),
//     // using default values:
//     slight_ButtonInput(2, A4, mybutton_get_input, mybutton_event, 1),
//     slight_ButtonInput(3, A5, mybutton_get_input, mybutton_event, 1),
// };







// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
