/******************************************

    slight_ButtonInput__minimal_multiplebuttons
        minimal example for slight_ButtonInput lib usage.
        debugport: serial interface 115200baud

    hardware:
        Arduino board of any type.
        D2 --> Pushbutton closing to GND
        D3 --> Pushbutton closing to GND
        D4 --> Pushbutton closing to GND
        D5 --> Pushbutton closing to GND
        D6 --> Pushbutton closing to GND

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

Copyright (c) 2021 Stefan Krüger

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

#include <slight_ButtonInput_CallbackHelper.h>
#include <slight_ButtonInput.h>


// ------------------------------------------
// slight_ButtonInput things

bool mybutton_get_input(slight_ButtonInput *instance) {
    // read input + invert: button closes to GND.
    // return !digitalRead((*instance).pin);

    // read input : button closes to VCC.
    // use for seedstudion groove Pushbutton
    return digitalRead((*instance).pin);
}

void mybutton_event(slight_ButtonInput *instance) {
    Serial.print(F("instance:"));
    Serial.print((*instance).id);
    Serial.print(F(" - event: "));
    (*instance).printEventLast(Serial);
    Serial.println();


    switch ((*instance).getEventLast()) {
        case slight_ButtonInput::event_click : {
            // Serial.println(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            // Serial.println(F("click long"));
        } break;
        case slight_ButtonInput::event_click_double : {
            // Serial.println(F("click double"));
        } break;
    }
}



// using default values:
// slight_ButtonInput mybutton_1(button_ID, pin_nummber, mybutton_get_input, mybutton_event);
slight_ButtonInput mybutton_1(1, 2, mybutton_get_input, mybutton_event);
slight_ButtonInput mybutton_2(2, 3, mybutton_get_input, mybutton_event);
slight_ButtonInput mybutton_3(3, 4, mybutton_get_input, mybutton_event);
slight_ButtonInput mybutton_4(4, 5, mybutton_get_input, mybutton_event);
slight_ButtonInput mybutton_start(5, 6, mybutton_get_input, mybutton_event);


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
    Serial.println(F("slight_ButtonInput__minimal_multiplebuttons.ino sketch."));
    Serial.println(F("minimal example for library usage."));

    // ------------------------------------------
    // start slight_ButtonInput
    Serial.println(F("setup slight_ButtonInput:")); {
        Serial.println(F("  pinMode INPUT_PULLUP"));
        // pinMode(mybutton_1.pin, INPUT_PULLUP);
        // pinMode(mybutton_2.pin, INPUT_PULLUP);
        // pinMode(mybutton_3.pin, INPUT_PULLUP);
        // pinMode(mybutton_4.pin, INPUT_PULLUP);
        // pinMode(mybutton_start.pin, INPUT_PULLUP);
        pinMode(mybutton_1.pin, INPUT);
        pinMode(mybutton_2.pin, INPUT);
        pinMode(mybutton_3.pin, INPUT);
        pinMode(mybutton_4.pin, INPUT);
        pinMode(mybutton_start.pin, INPUT);
        Serial.println(F("  mybutton.begin();"));
        mybutton_1.begin();
        mybutton_2.begin();
        mybutton_3.begin();
        mybutton_4.begin();
        mybutton_start.begin();
    }
    Serial.println(F("  finished."));

    // ------------------------------------------
    Serial.println(F("Loop:"));
}


// ------------------------------------------
// main loop
// ------------------------------------------
void loop() {
    mybutton_1.update();
    mybutton_2.update();
    mybutton_3.update();
    mybutton_4.update();
    mybutton_start.update();
    // nothing else to do here...
}


// ------------------------------------------
// THE END :-)
// ------------------------------------------
