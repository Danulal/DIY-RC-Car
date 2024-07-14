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