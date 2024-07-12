#include "elrs.h"
#include <AlfredoCRSF.h>

AlfredoCRSF crsf;

void init_elrs(long int baud) {
    Serial.begin(baud);
    Serial.println("COM Serial initialized");

    if (!Serial)
        while (1)
            Serial.println("Invalid crsfSerial configuration");

    crsf.begin(Serial);
}

void update_elrs() {
    crsf.update();
}

int readCH(int CH) {
    return crsf.getChannel(CH);
}

void printChannels()
{
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