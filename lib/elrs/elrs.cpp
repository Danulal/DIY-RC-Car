#include "elrs.h"
#include <AlfredoCRSF.h>

AlfredoCRSF crsf;

void init_elrs(long int baud) { // initialise the elrs receiver (crsf protocol)
    Serial.begin(baud);
    Serial.println("COM Serial initialized");

    if (!Serial)
        while (1)
            Serial.println("Invalid crsfSerial configuration");

    crsf.begin(Serial);
}

void update_elrs() { // update function that needs to run every loop to process data
    crsf.update();
}

int readCH(int CH) { // wrapper to read a Channel
    return crsf.getChannel(CH);
}

bool elrs_2way_switch(int CH) { // function to get a bool from a 2 way switch
    if(readCH(CH) >= 1500) {
        return true;
    } else {
        return false;
    }
}

unsigned long start_millis_elrs = 0; // variables needed for the timer in is_elrs_link_up()
unsigned long current_millis_elrs = 0;
bool previous_link_state = 0;

bool is_elrs_link_up(unsigned long timeout) {

    current_millis_elrs = millis();

    if(crsf.isLinkUp() == 0 && previous_link_state == 1) {
        if(start_millis_elrs == current_millis_elrs) {
            start_millis_elrs = millis();
        }
        if(current_millis_elrs - start_millis_elrs >= timeout) {
            previous_link_state = 0;
            start_millis_elrs = current_millis_elrs;
        }
    } else if(crsf.isLinkUp() == 1) {
        previous_link_state = 1;
        start_millis_elrs = current_millis_elrs;
    }

    return previous_link_state;
}

crsf_sensor_battery_t crsfBatt = { 0 };

void batt_telemetry(float voltage) {
    // Values are MSB first (BigEndian)
    crsfBatt.voltage = htobe16((uint16_t)(voltage * 10.0));   //Volts

    crsf.queuePacket(CRSF_SYNC_BYTE, CRSF_FRAMETYPE_BATTERY_SENSOR, &crsfBatt, sizeof(crsfBatt));
}

void printChannels() { // debug for printing out all channels of radiomaster zorro (14 channels active)
    for (int ChannelNum = 1; ChannelNum <= 14; ChannelNum++)
    {
        Serial.print(ChannelNum);
        Serial.print(": ");
        Serial.print(readCH(ChannelNum));
        Serial.print(", ");
    }
    Serial.print(", ");
    Serial.println(" ");
}