// some simple tests for callback things..


// experiments with std:function
// https://stackoverflow.com/questions/14189440/c-callback-using-class-member#14189561

// fix  "error: macro "min" passed 3 arguments, but takes just 2"
#undef min
#undef max

// this tries to fix
// undefined reference to `std::__throw_bad_function_call()'
// found at
// https://forum.arduino.cc/index.php?topic=382211.msg2790687#msg2790687
namespace std {
    void __throw_bad_function_call() {
        Serial.println(F("STL ERROR - __throw_bad_function_call"));
    }
}

#include <functional>


using tCallbackFunction = std::function<void(uint8_t)>;

tCallbackFunction callmeifyoucan;


void print_something(uint8_t value) {
    Serial.print("print_something:");
    Serial.println(value);
}



void setup() {
    // ------------------------------------------
    // init serial
    // wait for arduino IDE to release all serial ports after upload.
    delay(1000);
    Serial.begin(115200);
    delay(500);
    Serial.println();

    Serial.println(F("callback_test.ino sketch."));
    Serial.println(F("minimal example for C++ Callback Functions."));

    // ------------------------------------------
    callmeifyoucan = print_something;

    // ------------------------------------------
    Serial.println(F("Loop:"));
}

void loop() {
    // nothing else to do here...
    callmeifyoucan(42);
    delay(1000);
}
