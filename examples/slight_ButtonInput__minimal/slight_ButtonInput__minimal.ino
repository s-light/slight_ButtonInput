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

The MIT License
Copyright 2019 Stefan Krüger
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
https://opensource.org/licenses/mit-license.php
******************************************/

#include <slight_ButtonInput.h>


/*slight_ButtonInput(
    byte cbID_New,
    byte cbPin_New,
    tCbfuncGetInput cbfuncGetInput_New,
    tcbfOnEvent cbfCallbackOnEvent_New,
    const uint16_t cwDuration_Debounce_New = 30,
    const uint16_t cwDuration_HoldingDown_New = 1000,
    const uint16_t cwDuration_ClickSingle_New = 50,
    const uint16_t cwDuration_ClickLong_New = 3000,
    const uint16_t cwDuration_ClickDouble_New = 1000
);
*/
slight_ButtonInput myButtonLeft(
    // byte cbID_New
    42,
    // byte cbPin_New,
    A2,
    // tCbfuncGetInput cbfuncGetInput_New,
    myInputLeft_callback_GetInput,
    // tcbfOnEvent cbfCallbackOnEvent_New,
    myCallback_onEvent,
    // const uint16_t cwDuration_Debounce_New = 30,
      30,
    // const uint16_t cwDuration_HoldingDown_New = 1000,
    1000,
    // const uint16_t cwDuration_ClickSingle_New =   50,
      50,
    // const uint16_t cwDuration_ClickLong_New =   3000,
    3000,
    // const uint16_t cwDuration_ClickDouble_New = 1000
     500
);
// using default values:
//slight_ButtonInput myButtonLeft(42, A3, myInputLeft_callback_GetInput, myCallback_onEvent, 50, 3000);



// ******************************************
// slight_ButtonInput functions

boolean myInputLeft_callback_GetInput(byte bID, byte bPin) {
    // read input invert reading - button closes to GND.
    // check HWB
    return ! (PINE & B00000100);
    //return ! digitalRead(bPin);
}


void myCallback_onEvent(slight_ButtonInput *pInstance, byte bEvent) {
    // Serial.print(F("Instance ID:"));
    // Serial.println((*pInstance).getID());

    Serial.print(F("Event: "));
    (*pInstance).printEvent(Serial, bEvent);
    Serial.println();

    // show event additional infos:
    switch (bEvent) {
        case slight_ButtonInput::event_StateChanged : {
            Serial.print(F("\t state: "));
            (*pInstance).printState(Serial);
            Serial.println();
        } break;
        case slight_ButtonInput::event_Down : {
            Serial.print(F("the button is pressed down! do something.."));
        } break;
        case slight_ButtonInput::event_HoldingDown : {
            Serial.print(F("duration active: "));
            Serial.println((*pInstance).getDurationActive());
        } break;
        case slight_ButtonInput::event_Up : {
            Serial.print(F("up"));
        } break;
        case slight_ButtonInput::event_Click : {
            Serial.print(F("click"));
        } break;
        case slight_ButtonInput::event_ClickLong : {
            Serial.print(F("click long"));
        } break;
        case slight_ButtonInput::event_ClickDouble : {
            Serial.print(F("click double"));
        } break;
        case slight_ButtonInput::event_ClickTriple : {
            Serial.print(F("click triple"));
        } break;
        case slight_ButtonInput::event_ClickMulti : {
            Serial.print(F("click count: "));
            Serial.println((*pInstance).getClickCount());
        } break;
    } //end switch

}


// ------------------------------------------
// setup
// ------------------------------------------
void setup() {

    // ------------------------------------------
    // init serial

        //wait for arduino IDE to release all serial ports after upload.
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

        Serial.println(F("setup slight_ButtonInput:"));
        {
            Serial.println(F("  pinMode INPUT_PULLUP"));
            pinMode(myButtonLeft.getPin(), INPUT_PULLUP);

            Serial.println(F("  myButtonLeft.begin();"));
            myButtonLeft.begin();

        }
        Serial.println(F("  finished."));


    // ------------------------------------------
    Serial.println(F("Loop:"));

}
// end setup


// ------------------------------------------
// main loop
// ------------------------------------------
void loop() {

    myButtonLeft.update();

}


// ------------------------------------------
// THE END :-)
// ------------------------------------------
