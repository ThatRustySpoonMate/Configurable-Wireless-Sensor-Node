#include "InputTask.hpp"

extern HardwareSerial Serial;

extern bool debug_log;

void serialTask() {
    char usrCmd[5]; // 4 Chars + null terminator
    
    if(Serial.available() > 3) {
        
        Serial.readBytes(usrCmd, 4);
        usrCmd[4] = '\0';  // Add null terminator

        Serial.print(">");Serial.println(usrCmd);

        Serial.flush();

        if( strcmp(usrCmd, "show") == 0) {
            Serial.println("Enabled debug logs");
            debug_log = true;
        }
        
        if (strcmp(usrCmd, "hide") == 0) {
            Serial.println("Disabled debug logs");
            debug_log = false;
        }


    }

}
