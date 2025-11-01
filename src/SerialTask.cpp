#include "SerialTask.hpp"
#include "configuration.h"
#include "main.hpp"

//extern HardwareSerial Serial;

extern bool debug_log;
static bool isFirstReading;


void serial_setup() {
    
    Serial.begin(SERIAL_BAUD_RATE);

    MY_DEBUG_PRINTLN("Serial output initialized");

    return;
}

void serial_handleInput() {
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


// Human optimized output
void serial_transmit_plain(const char* topic, const char* value) {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_HUMAN)
    
    // Format: [timestamp] topic: value
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(value);
    
    #endif
    
    return;
}


// CSV formatted output
void serial_transmit_csv_start() {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_CSV)
    Serial.print(millis());
    isFirstReading = true;
    #endif
}

void serial_transmit_csv(const char* topic, const char* value) {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_CSV)
    
    Serial.print(",");
    Serial.print(value);
    
    #endif
    
    return;
}

void serial_transmit_csv_end() {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_CSV)
    Serial.println();  // End the line
    #endif
}

// JSON formatted output
void serial_transmit_json_start() {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_JSON)
    Serial.print("{\"millis\":");
    Serial.print(millis());
    Serial.print(",\"readings\":{");
    isFirstReading = true;
    #endif
}

void serial_transmit_json(const char* topic, const char* value) {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_JSON)
    
    if (!isFirstReading) {
        Serial.print(",");
    }
    Serial.print("\"");
    Serial.print(topic);
    Serial.print("\":\"");
    Serial.print(value);
    Serial.print("\"");
    isFirstReading = false;
    
    #endif
    
    return;
}

void serial_transmit_json_end() {
    #if defined(DATA_OUTPUT_OVER_SERIAL) && defined(SERIAL_DATA_OUT_FORMAT_JSON)
    Serial.println("}}");
    #endif
}


// Function that takes data and decides how to output it 

void serial_transmit_start() {
    serial_transmit_csv_start();
    serial_transmit_json_start();

    return;
}

void serial_transmit(const char* topic, const char* value) {

    #ifdef DATA_OUTPUT_OVER_SERIAL

        #ifdef SERIAL_DATA_OUT_FORMAT_CSV
            serial_transmit_csv(topic, value);
        #endif

        #ifdef SERIAL_DATA_OUT_FORMAT_JSON
            serial_transmit_json(topic, value);
        #endif

        #ifdef SERIAL_DATA_OUT_FORMAT_HUMAN
            serial_transmit_plain(topic, value);
        #endif

    #endif 

    return;

}


void serial_transmit_end() {
    serial_transmit_csv_end();
    serial_transmit_json_end();

    return;
}

