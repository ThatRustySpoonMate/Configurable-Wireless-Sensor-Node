/* Defines */
#define DHTPIN     25     // Digital pin connected to the DHT sensor - CAN'T BE AN ADC2 PIN
#define DHTTYPE    DHT11 // DHT 11

const char *MQTT_TOPIC_TEMP = "weather/location/temperature";
const char *MQTT_TOPIC_HUMI = "weather/location/humidity";
const char *MQTT_TOPIC_HEATINDEX = "weather/location/heat_index";
const char *MQTT_MANAGEMENT_TOPIC = "manage/weather";

/* Exported Data Types */

/* Exported Enumerators */
enum Payload_Map{
    Payload_Temperature = 0,
    Payload_Humidity,
    Payload_Heat_Index
};

/* Function Declarations */
void sampleTask();
void transmitTask();