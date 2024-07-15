#ifndef ELRS
#define ELRS
void init_elrs(long int baudrate);
void update_elrs();
int readCH(int channel);
bool elrs_2way_switch(int channel);
bool is_elrs_link_up(unsigned long timeout);
void printChannels();
#endif // ELRS