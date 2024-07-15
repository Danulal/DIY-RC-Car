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
    if(readCH(CH) >= 2000) {
        return true;
    } else {
        return false;
    }
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

unsigned long start_millis_elrs = 0; // variables needed for the timer in is_elrs_link_up()
unsigned long current_millis_elrs = 0;
bool previous_link_state = 0;

bool is_elrs_link_up(unsigned long timeout) {

    // Serial.print("isLinkUp: "); // timer debug
    // Serial.print(crsf.isLinkUp());
    // Serial.print("\tprevious link state: ");
    // Serial.print(previous_link_state);
    // Serial.print("\tcurrent_millis: ");
    // Serial.print(current_millis_elrs);
    // Serial.print("\tstart_millis: ");
    // Serial.print(start_millis_elrs);
    // Serial.print("\n");

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