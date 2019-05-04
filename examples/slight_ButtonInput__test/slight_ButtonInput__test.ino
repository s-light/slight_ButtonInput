/******************************************

    slight_slight_ButtonInput__test
        test the slight_slight_ButtonInput lib.
        debugport: serial interface 115200baud

    hardware:
        Arduino board of any typ.
        D9 --> LED connected to R to VCC
        A0 --> Pushbutton closing to GND
        A1 --> Pushbutton closing to GND

        OLIMEXINO-32U4
            on Board PushButton

    libraries used:
        ~ slight_ButtonInput

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/


    changelog / history
        12.11.2013 17:33 created.
        13.11.2013 17:41 add object ID
        15.03.2014 20:24 changed to use new instance technique
        15.03.2014 22:52 added demo for new events (event_ClickTriple, event_ClickMulti, event_holddown)


    TO DO:
        test lib

******************************************/
/******************************************
    license

    CC BY SA
        This work is licensed under the
        Creative Commons Attribution-ShareAlike 3.0 Unported License.
        To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.

    Apache License Version 2.0
        Copyright 2014 Stefan Krueger

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

    The MIT License (MIT)
        Copyright (c) 2014 stefan krueger
        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"),
        to deal in the Software without restriction, including without limitation
        the rights to use, copy, modify, merge, publish, distribute, sublicense,
        and/or sell copies of the Software, and to permit persons to whom the Software
        is furnished to do so, subject to the following conditions:
        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.
        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
        INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
        PARTICULAR PURPOSE AND NONINFRINGEMENT.
        IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
        OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
        http://opensource.org/licenses/mit-license.php

******************************************/
/******************************************/
/** Includes:  (must be at the begining of the file.)                                            **/
/******************************************/
// use "" for files in same directory as .ino
//#include "file.h"

#include <slight_ButtonInput.h>

/******************************************/
/** info                                                                                         **/
/******************************************/
void print_info(Print &out) {
    out.println();
    //             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|                       ^ ^                      |"));
    out.println(F("|                      (0,0)                     |"));
    out.println(F("|                      ( _ )                     |"));
    out.println(F("|                       \" \"                      |"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("| slight_ButtonInput__test.ino"));
    out.println(F("|   Sketch to test, develop and showcase functions of slight_ButtonInput library"));
    out.println(F("|"));
    out.println(F("| This Sketch has a debug-menu:"));
    out.println(F("| send '?'+Return for help"));
    out.println(F("|"));
    out.println(F("| dream on & have fun :-)"));
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println(F("|"));
    //out.println(F("| Version: Nov 11 2013  20:35:04"));
    out.print(F("| version: "));
    out.print(F(__DATE__));
    out.print(F("  "));
    out.print(F(__TIME__));
    out.println();
    out.println(F("|"));
    out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
    out.println();

    //out.println(__DATE__); Nov 11 2013
    //out.println(__TIME__); 20:35:04
}


/** Serial.print to Flash: Notepad++ Replace RegEx
    Find what:        Serial.print(.*)\("(.*)"\);
    Replace with:    Serial.print\1(F("\2"));
**/

/******************************************/
/** libraries                                                                                    **/
/******************************************/

/**************************************************/
/**  slight ButtonInput                          **/
/**************************************************/

/*slight_ButtonInput(
    byte id_new,
    byte pin_new,
    tCbfuncGetInput cbfuncGetInput_new,
    tcbfOnEvent cbfCallbackOnEvent_new,
    const uint16_t duration_debounce_new = 30,
    const uint16_t duration_holddown_new = 1000,
    const uint16_t duration_click_single_new = 50,
    const uint16_t duration_click_long_new = 3000,
    const uint16_t duration_click_double_new = 1000
);
*/
slight_ButtonInput myButtonLeft(
    42,  // byte id_new
    A2,  // byte pin_new,
    myInputLeft_callback_GetInput,  // tCbfuncGetInput cbfuncGetInput_new,
    myCallback_onEvent,  // tcbfOnEvent cbfCallbackOnEvent_new,
      30,  // const uint16_t duration_debounce_new = 30,
    1000,  // const uint16_t duration_holddown_new = 1000,
      50,  // const uint16_t duration_click_single_new =   50,
    3000,  // const uint16_t duration_click_long_new =   3000,
     500   // const uint16_t duration_click_double_new = 1000
);
// using default values:
//slight_ButtonInput myButtonLeft(42, A3, myInputLeft_callback_GetInput, myCallback_onEvent, 50, 3000);
//slight_ButtonInput myButtonLeft(42, A4, myInputLeft_callback_GetInput, myCallback_onEvent, 50);
//slight_ButtonInput myButtonLeft(42, A5, myInputLeft_callback_GetInput, myCallback_onEvent);

/******************************************/
/** definitions (gloabl)                                                                         **/
/******************************************/

boolean bLEDState = 0;
const byte id_LED_Info = 9; //D9

unsigned long ulDebugOut_LiveSign_TimeStamp_LastAction    = 0;
const uint16_t cwDebugOut_LiveSign_UpdateInterval            = 1000; //ms

boolean bDebugOut_LiveSign_Serial_Enabled    = 0;
boolean bDebugOut_LiveSign_LED_Enabled    = 1;


/**************************************************/
/**  led and serial debug                        **/
/**************************************************/
// myFaderOne output on:
byte pin_LED = 9;

byte bDebugOut_myFaderOne_Output_Enable        = 0;


//unsigned long timestamp_LastAction    = 0;
//const uint16_t cwUpdateInterval            = 0; //ms




/**************************************************/
/** Serial Receiver  (debug control)             **/
/**************************************************/

// a string to hold new data
char  sMenu_Input_new[]                = "[ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ]";
// flag if string is complete
boolean bMenu_Input_new_FlagComplete    = false;

// string for Currently to process Command
char  sMenu_Command_Current[]        = "[ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, ]";


/******************************************/
/** functions                                                                                    **/
/******************************************/

/************************************************/
/**  Debug things                              **/
/************************************************/

// http://forum.arduino.cc/index.php?topic=183790.msg1362282#msg1362282
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


void printBinary8(byte bIn)  {

    for (unsigned int mask = 0b10000000; mask; mask >>= 1) {
        if (mask & bIn) {
            Serial.print('1');
        }
        else {
            Serial.print('0');
        }
    }
}

void printBinary12(uint16_t bIn)  {
    //                       B12345678   B12345678
    //for (unsigned int mask = 0x8000; mask; mask >>= 1) {
    for (unsigned int mask = 0b100000000000; mask; mask >>= 1) {
        if (mask & bIn) {
            Serial.print('1');
        }
        else {
            Serial.print('0');
        }
    }
}

void printBinary16(uint16_t wIn)  {

    for (unsigned int mask = 0b1000000000000000; mask; mask >>= 1) {
        if (mask & wIn) {
            Serial.print('1');
        }
        else {
            Serial.print('0');
        }
    }
}



void printArray_Byte(byte *array, byte bCount) {
    Serial.print(F(" "));
    byte bIndex = 0;
    printAlignRight_Byte(array[bIndex]);
    for(bIndex = 1; bIndex < bCount; bIndex++){
        Serial.print(F(", "));
        printAlignRight_Byte(array[bIndex]);
    }
}

void printAlignRight_Byte(byte bValue) {
    //byte bOffset = 0;
    if (bValue < 100) {
        if (bValue < 10) {
            //bOffset = 2;
            Serial.print(F("  "));
        } else {
            //bOffset = 1;
            Serial.print(F(" "));
        }
    }
    Serial.print(bValue);
}


void printArray_uint16_t(uint16_t *array, byte bCount) {
    Serial.print(F(" [ "));
    byte bIndex = 0;
    printAlignRight_uint16_t(array[bIndex]);
    for(bIndex = 1; bIndex < bCount; bIndex++){
        Serial.print(F(", "));
        printAlignRight_uint16_t(array[bIndex]);
    }
    Serial.print(F("]"));
}

byte printAlignRight_uint16_t(uint16_t wValue) {
    byte bLeadingZeros = 0;
    if (wValue < 10000) {
        bLeadingZeros = bLeadingZeros + 1;
        Serial.print(F(" "));
        if (wValue < 1000) {
            bLeadingZeros = bLeadingZeros + 1;
            Serial.print(F(" "));
            if (wValue < 100) {
                bLeadingZeros = bLeadingZeros + 1;
                Serial.print(F(" "));
                if (wValue < 10) {
                    bLeadingZeros = bLeadingZeros + 1;
                    Serial.print(F(" "));
                }
            }
        }
    }
    Serial.print(wValue);
    return bLeadingZeros;
}



/************************************************/
/**  Menu System                               **/
/************************************************/

// Modes for Menu Switcher
const uint8_t cbMenuMode_MainMenu    = 1;
const uint8_t cbMenuMode_SubMenu1    = 2;
uint8_t bMenuMode = cbMenuMode_MainMenu;


// SubMenu SetValues
void handle_SubMenu1(Print &out, char *caCommand) {
    out.println(F("SubMenu1:"));
    out.println(F("\t nothing here."));
    out.println(F("\t finished."));
    // exit submenu
    // reset state manschine of submenu
    // jump to main
    bMenuMode = cbMenuMode_MainMenu;
    bMenu_Input_new_FlagComplete = true;
}


// Main Menu
void handle_MainMenu(Print &out, char *caCommand) {
    /* out.print("sCommand: '");
    out.print(sCommand);
    out.println("'"); */
    switch (caCommand[0]) {
        case 'h':
        case 'H':
        case '?': {
            // help
            out.println(F("____________________________________________________________"));
            out.println();
            out.println(F("Help for Commands:"));
            out.println();
            out.println(F("\t '?': this help"));
            out.println(F("\t 'i': sketch info"));
            out.println(F("\t 'y': toggle DebugOut livesign print"));
            out.println(F("\t 'Y': toggle DebugOut livesign LED"));
            out.println(F("\t 'x': tests"));
            out.println();
            out.println(F("\t 'A': Show 'HelloWorld' "));
            out.println(F("\t 'e': enable input"));
            out.println(F("\t 'E': disable input"));
            out.println();
            out.println(F("\t 'set:' enter SubMenu1"));
            out.println();
            out.println(F("____________________________________________________________"));
        } break;
        case 'i': {
            print_info(out);
        } break;
        case 'y': {
            out.println(F("\t toggle DebugOut livesign Serial:"));
            bDebugOut_LiveSign_Serial_Enabled = !bDebugOut_LiveSign_Serial_Enabled;
            out.print(F("\t bDebugOut_LiveSign_Serial_Enabled:"));
            out.println(bDebugOut_LiveSign_Serial_Enabled);
        } break;
        case 'Y': {
            out.println(F("\t toggle DebugOut livesign LED:"));
            bDebugOut_LiveSign_LED_Enabled = !bDebugOut_LiveSign_LED_Enabled;
            out.print(F("\t bDebugOut_LiveSign_LED_Enabled:"));
            out.println(bDebugOut_LiveSign_LED_Enabled);
        } break;
        case 'x': {
            // get state
            out.println(F("__________"));
            out.println(F("Tests:"));

            out.println(F("nothing to do."));

            // uint16_t wTest = 65535;
            uint16_t wTest = atoi(&caCommand[1]);
            out.print(F("wTest: "));
            out.print(wTest);
            out.println();

            out.print(F("1: "));
            out.print((byte)wTest);
            out.println();

            out.print(F("2: "));
            out.print((byte)(wTest>>8));
            out.println();

            out.println();

            out.println(F("__________"));
        } break;
        //--------------------------------------------------------------------------------
        case 'A': {
            out.println(F("\t Hello World! :-)"));
        } break;
        case 'e': {
                Serial.println(F("\t enabled input."));
                myButtonLeft.enable();
            } break;
        case 'E': {
                Serial.println(F("\t disabled input."));
                myButtonLeft.disable();
            } break;
        //--------------------------------------------------------------------------------
        case 's': {
            // SubMenu1
            if ( (caCommand[1] == 'e') && (caCommand[2] == 't') && (caCommand[3] == ':') ) {
                //if full command is 'set:' enter submenu
                bMenuMode = cbMenuMode_SubMenu1;
                if(1){    //if ( caCommand[4] != '\0' ) {
                    //full length command
                    //handle_SetValues(out, &caCommand[4]);
                } else {
                    bMenu_Input_new_FlagComplete = true;
                }
            }
        } break;
        //--------------------------------------------------------------------------------
        default: {
            out.print(F("command '"));
            out.print(caCommand);
            out.println(F("' not recognized. try again."));
            sMenu_Input_new[0] = '?';
            bMenu_Input_new_FlagComplete = true;
        }
    } //end switch

    //end Command Parser
}


// Menu Switcher
void menuSwitcher(Print &out, char *caCommand) {
    switch (bMenuMode) {
            case cbMenuMode_MainMenu: {
                handle_MainMenu(out, caCommand);
            } break;
            case cbMenuMode_SubMenu1: {
                handle_SubMenu1(out, caCommand);
            } break;
            default: {
                // something went wronge - so reset and show MainMenu
                bMenuMode = cbMenuMode_MainMenu;
            }
        } // end switch bMenuMode
}

// Check for NewLineComplete and enter menuSwitcher
// sets Menu Output channel (out)
void check_newLineComplete() {
    // if SMenuCurrent is a full Line (terminated with \n) than parse things
    if (bMenu_Input_new_FlagComplete) {
        /*
        Serial.print(F("bMenu_Input_new_FlagComplete: sMenu_Input_new: '"));
        Serial.print(sMenu_Input_new);
        Serial.println(F("'"));
        Serial.print(F("   state_UI: '"));
        Serial.print(state_UI);
        Serial.println(F("'"));/**/

        // coppy to current buffer
        strcpy (sMenu_Command_Current, sMenu_Input_new);

        // reset input
        memset(sMenu_Input_new, '\0', sizeof(sMenu_Input_new)-1);
        bMenu_Input_new_FlagComplete = false;

        // run command
        menuSwitcher(Serial, sMenu_Command_Current);

    } // if bMenu_Input_new_FlagComplete
}

/************************************************/
/**  Serial Receive Handling                   **/
/************************************************/

void handle_SerialReceive() {
    // collect next input text
    while (Serial.available()) {
        // get the new byte:
        char charNew = (char)Serial.read();
        /*Serial.print(F("charNew '"));
        Serial.print(charNew);
        Serial.print(F("' : "));
        Serial.println(charNew, DEC);
        Serial.print(F("'\\n' : "));
        Serial.println('\n', DEC);
        Serial.print(F("'\\r' : '"));
        Serial.println('\r', DEC);*/

        /* http://forums.codeguru.com/showthread.php?253826-C-String-What-is-the-difference-between-n-and-r-n
            '\n' == 10 == LineFeed == LF
            '\r' == 13 == Carriag Return == CR
            Windows: '\r\n'
            Linux: '\n'
            Apple: '\r'
        */
        // if the incoming character is a linefeed '\r' or newline '\n'
        //if ((charNew == '\r') || (charNew == '\n')) {
        if (charNew == '\n') {
            //clean up last received char if it is a '\r'
            if (sMenu_Input_new[strlen(sMenu_Input_new) - 1] == '\r') {
                sMenu_Input_new[strlen(sMenu_Input_new) - 1] = '\0';
            }
            //set complete flag
            bMenu_Input_new_FlagComplete = true;
        } else {
            // add it to the sMenu_Input_new:
            //sMenu_Input_new = sMenu_Input_new + charNew;
            // char array version
            //check for length..
            if (strlen(sMenu_Input_new) < sizeof(sMenu_Input_new) ) {
                sMenu_Input_new[strlen(sMenu_Input_new)] = charNew;
            } else {
                Serial.println(F("inputstring to long!"));
                // reset
                memset(sMenu_Input_new,'\0',sizeof(sMenu_Input_new)-1);
                bMenu_Input_new_FlagComplete = false;
            }
        }
    }
}






/************************************************/
/**  Serial Receive Handling                   **/
/************************************************/

void commandParser(char *caCommand) {
    Serial.println("__________________________________________________");
    Serial.println("commandParser:");
    /*
    Serial.print("sCommand: '");
    Serial.print(sCommand);
    Serial.println("'");
    */

    char bFirstChar = caCommand[0];

    switch (bFirstChar) {
        case 'h':
        case 'H':
        case '?': {
                // help
                Serial.println(F("___________________________________"));
                Serial.println(F("Help for Serial Commands:"));
                Serial.println(F("\t '?': this help"));
                Serial.println(F("\t 'o': toggle DebugOut livesign Serial"));
                Serial.println(F("\t 'O': toggle DebugOut livesign LED"));
                Serial.println(F("\t 't': tests"));
                Serial.println(F("\t 'e': enable  input"));
                Serial.println(F("\t 'E': disable input"));
                Serial.println(F("___________________________________"));
            } break;
        case 'o': {
                Serial.println(F("\t toggle DebugOut livesign Serial:"));
                bDebugOut_LiveSign_Serial_Enabled = !bDebugOut_LiveSign_Serial_Enabled;
                Serial.print(F("\t bDebugOut_LiveSign_Serial_Enabled:"));
                Serial.println(bDebugOut_LiveSign_Serial_Enabled);
            } break;
        case 'O': {
                Serial.println(F("\t toggle DebugOut livesign LED:"));
                bDebugOut_LiveSign_LED_Enabled = !bDebugOut_LiveSign_LED_Enabled;
                Serial.print(F("\t bDebugOut_LiveSign_LED_Enabled:"));
                Serial.println(bDebugOut_LiveSign_LED_Enabled);
            } break;
        case 't': {
                // get state
                Serial.println(F("___________________________________"));
                Serial.println(F("Tests:"));

                Serial.print(F("myButtonLeft State: "));
                //myButtonLeft.printState();
                Serial.println();

                //timeMeasurement();
                //Serial.println();

                Serial.println(F("___________________________________"));
            } break;
        case 'e': {
                Serial.println(F("\t enabled input."));
                myButtonLeft.enable();
            } break;
        case 'E': {
                Serial.println(F("\t disabled input."));
                myButtonLeft.disable();
            } break;
        /*case 'r': {
                Serial.println(F("\t Fader2 drivte to value"));
                uint16_t wValue = atoi(&caCommand[1]);
                Serial.print(F("\t myMFC_Fader2.driveTo("));
                Serial.print(wValue);
                Serial.println(F(");"));
                myMFC_Fader2.driveTo(wValue);

            } break;
        /*case 'd': {
                Serial.println(F("\t example for dispatching: dC:V C=channel V=value"));

                //strchr(
                //strstr(
                //char strTemp[sizeof(caCommand)];
                //strncpy(strTemp, caCommand[1], ptrSeparation );

                byte bChannel = atoi(&caCommand[1]);
                Serial.print(F("\t bChannel:"));
                Serial.println(bChannel);

                //http://forum.arduino.cc/index.php/topic,129836.0.html
                //char *ptrSeparation = strchr(caCommand,':');
                //Serial.print(F("\t ptrSeparation:"));
                //Serial.println(*ptrSeparation);

                //byte bValue = atoi(&caCommand[(*ptrSeparation)+1]);
                //Serial.print(F("\t bValue:"));
                //Serial.println(bValue);

                byte bValue = atoi(&caCommand[3]);
                Serial.print(F("\t bValue:"));
                Serial.println(bValue);

                Serial.print(F("\t do something with the values."));

            } break;
        /**/
        default: {
                Serial.println(F("?? unknown command:"));
                Serial.print(F("\t caCommand: '"));
                Serial.print(caCommand);
                Serial.println(F("'"));
                Serial.print(F("\t bFirstChar: '"));
                Serial.print(bFirstChar);
                Serial.println(F("'"));
            }
    } //end switch

    //end Command Parser
    Serial.println("__________________________________________________");
}


void handle_SerialReceive() {

    while (Serial.available()) {
        // get the new byte:
        char charNew = (char)Serial.read();
        // if the incoming character is a newline
        if (charNew == '\n') {
            //set complete flag
            sSerialNewInput_bComplete = true;
        } else {
            // add it to the sSerialNewInput:
            //sSerialNewInput = sSerialNewInput + charNew;
            // char array version
            //check for length..
            if (strlen(sSerialNewInput) < sizeof(sSerialNewInput) ) {
                sSerialNewInput[strlen(sSerialNewInput)] = charNew;
            } else {
                Serial.println(F("inputstring to long!"));
            }
        }
    }

    // parse the string when a newline arrives:
    if (sSerialNewInput_bComplete) {
        /*Serial.print(F("Received String: '"));
        Serial.print(sSerialNewInput);
        Serial.println(F("'"));/**/

        commandParser(sSerialNewInput);

        // clear the string:
        //bSerialNewInput_Index = 0;
        //sSerialNewInput = "";
        memset(sSerialNewInput,'\0',sizeof(sSerialNewInput)-1);

        sSerialNewInput_bComplete = false;
    }

}


/************************************************/
/**  slight_ButtonInput things                 **/
/************************************************/


boolean myInputLeft_callback_GetInput(byte id, byte pin) {
    // read input invert reading - button closes to GND.
    // check HWB
    return ! (PINE & B00000100);
    //return ! digitalRead(pin);
}


void myCallback_onEvent(slight_ButtonInput *instance, byte event) {

    // Serial.print(F("Instance ID:"));
    // Serial.println((*instance).getID());

    Serial.print(F("Event: "));
    (*instance).printEvent(Serial, event);
    Serial.println();

    // show event additional infos:
    switch (event) {
        /*case slight_ButtonInput::event_StateChanged : {
            Serial.print(F("\t state: "));
            (*instance).printState(Serial);
            Serial.println();
        } break;
        // click
        /*case slight_ButtonInput::event_Down : {
            Serial.print(F("the button is pressed down! do something.."));
        } break;*/
        case slight_ButtonInput::event_holddown : {
            Serial.print(F("duration active: "));
            Serial.println((*instance).getDurationActive());
        } break;
        /*case slight_ButtonInput::event_Up : {
            Serial.print(F("up"));
        } break;
        case slight_ButtonInput::event_Click : {
            Serial.print(F("click"));
        } break;
        case slight_ButtonInput::event_click_long : {
            Serial.print(F("click long"));
        } break;
        case slight_ButtonInput::event_click_double : {
            Serial.print(F("click double"));
        } break;
        case slight_ButtonInput::event_ClickTriple : {
            Serial.print(F("click triple"));
        } break;*/
        case slight_ButtonInput::event_ClickMulti : {
            Serial.print(F("click count: "));
            Serial.println((*instance).getClickCount());
        } break;
    } //end switch

}



/**************************************************/
/** Time Measurement                             **/
/**************************************************/
void timeMeasurement() {
    Serial.println(F("\t time Measurement (can need some time): "));
    unsigned long ulTimeStamp = 0;
    unsigned long ulDuration = 0;

    /** **/
    uint16_t wLoopCount = 10000;
    Serial.print(F("\t   xxx(): "));
    ulTimeStamp = millis();
    for (uint16_t wLoop = 0; wLoop < wLoopCount; wLoop++) {
        //myFaderOne.update();
    }
    ulDuration = millis() - ulTimeStamp;
    Serial.print(ulDuration);
    Serial.print(F("ms for "));
    Serial.print(wLoopCount);
    Serial.print(F(" calls"));
    Serial.println();
    /** **
    ulTimeStamp = millis();
    for (uint16_t wLoop = 0; wLoop < wLoopCount; wLoop++) {
        //myFaderOne.update();
    }
    ulDuration = millis() - ulTimeStamp;
    Serial.print(ulDuration);
    Serial.print(F("ms for "));
    Serial.print(wLoopCount);
    Serial.print(F(" calls"));
    Serial.println();
    /** **/
}

/************************************************/
/**                                            **/
/************************************************/


/********************************************/
/** Setup                                                                                          **/
/********************************************/
void setup() {

    /************************************************/
    /** Initialise PINs                            **/
    /************************************************/

        //LiveSign
        pinMode(id_LED_Info, OUTPUT);
        digitalWrite(id_LED_Info, HIGH);

        // as of arduino 1.0.1 you can use INPUT_PULLUP

    /************************************************/
    /** init serial                                **/
    /************************************************/

        // for ATmega32U4 devices:
        #if defined (__AVR_ATmega32U4__)
            //wait for arduino IDE to release all serial ports after upload.
            delay(2000);
        #endif

        Serial.begin(115200);

        // for ATmega32U4 devices:
        #if defined (__AVR_ATmega32U4__)
            // Wait for Serial Connection to be Opend from Host or 6second timeout
            unsigned long timestamp_Start = millis();
            while( (! Serial) && ( (millis() - timestamp_Start) < 6000 ) ) {
                1;
            }
        #endif

        Serial.println();

        Serial.print(F("# Free RAM = "));
        Serial.println(freeRam());


    /************************************************/
    /** Welcom                                     **/
    /************************************************/

        print_info(Serial);


    /************************************************/
    /** start slight_ButtonInput                   **/
    /************************************************/

        Serial.println(F("slight_ButtonInput:"));
        {
            Serial.println(F("\t pinMode INPUT_PULLUP"));
            //pinMode(myButtonLeft.getPin(), INPUT_PULLUP);
            // Initialize the port for HWB
            DDRE &= B11111011;

            Serial.println(F("\t myButtonLeft.begin();"));
            myButtonLeft.begin();

        }
        Serial.println(F("\t finished."));

    /************************************************/
    /** show Serial Commands                       **/
    /************************************************/

        // reset Serial Debug Input
        memset(sMenu_Input_new, '\0', sizeof(sMenu_Input_new)-1);
        //print Serial Options
        sMenu_Input_new[0] = '?';
        bMenu_Input_new_FlagComplete = true;


    /************************************************/
    /** GO                                         **/
    /************************************************/

        Serial.println(F("Loop:"));



} /** setup **/


/********************************************/
/** Main Loop                                                                                      **/
/********************************************/
void loop() {

    /**************************************************/
    /** Menu Input                                   **/
    /**************************************************/
        // Serial
        handle_SerialReceive();
        check_newLineComplete();

    /**************************************************/
    /** my Button                                    **/
    /**************************************************/
        myButtonLeft.update();

    /**************************************************/
    /** Timed things                                 **/
    /**************************************************/

        /*
        // every Nms
        if ( ( millis() - timestamp_LastAction ) > cwUpdateInterval) {
            timestamp_LastAction =  millis();

        }
        */


    /**************************************************/
    /** Debug Out                                    **/
    /**************************************************/

        if ( (millis() - ulDebugOut_LiveSign_TimeStamp_LastAction) > cwDebugOut_LiveSign_UpdateInterval) {
            ulDebugOut_LiveSign_TimeStamp_LastAction = millis();

            if ( bDebugOut_LiveSign_Serial_Enabled ) {
                Serial.print(millis());
                Serial.print(F("ms;"));
                Serial.print(F("  free RAM = "));
                Serial.println(freeRam());
            }

            if ( bDebugOut_LiveSign_LED_Enabled ) {
                bLEDState = ! bLEDState;
                if (bLEDState) {
                    //set LED to HIGH
                    digitalWrite(id_LED_Info, HIGH);
                } else {
                    //set LED to LOW
                    digitalWrite(id_LED_Info, LOW);
                }
            }

        }

    /**************************************************/
    /**                                              **/
    /**************************************************/

} /** loop **/


/********************************************/
/** THE END                                                                                        **/
/********************************************/
