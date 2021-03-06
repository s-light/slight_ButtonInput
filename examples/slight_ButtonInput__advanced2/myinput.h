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



#ifndef myinput_H_
#define myinput_H_

// include Core Arduino functionality
#include <Arduino.h>

#include <slight_ButtonInput.h>


class MyInput {
public:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // constructor

    // MyInput(Stream &out);
    MyInput();
    ~MyInput();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // public functions

    // basic library api
    void begin(Stream &out);
    void update();
    void end();

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // button
    boolean mybutton_get_input(slight_ButtonInput *instance);
    void mybutton_event(slight_ButtonInput *instance);

    slight_ButtonInput mybutton = slight_ButtonInput(
        // uint8_t id_new
        1,
        // uint8_t pin_new,
        A3,
        // tCallbackFunctionGetInput callbackGetInput_new,
        std::bind(&MyInput::mybutton_get_input, this, std::placeholders::_1),
        // tCallbackFunction callbackOnEvent_new,
        std::bind(&MyInput::mybutton_event, this, std::placeholders::_1),
        // const uint16_t duration_debounce_new = 20,
        10,
        // const uint16_t duration_holddown_new = 1000,
        1000,
        // const uint16_t duration_click_long_new =   3000,
        500,
        // const uint16_t duration_click_double_new = 250
        250
    );


    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // configurations

private:

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // private functions

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // attributes
    bool ready;
    // Stream &out;

    uint8_t state = 0;
    uint8_t state_last = 0;

};  // class MyInput

#endif  // myinput_H_
