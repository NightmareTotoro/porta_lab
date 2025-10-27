#ifndef RFID_H
#define RFID_H

#include <MFRC522.h>

class RFID {
public:
    RFID(byte ss_pin, byte rst_pin);
    void init();
    void read_card();

private:
    MFRC522* mfrc522;
    byte ss_pin;
    byte rst_pin;
};

#endif // RFID_H