#ifndef FAP50READER_H
#define FAP50READER_H

#include "InterfaceImdFap50Method.h"

class Fap50Reader
{
public:
    Fap50Reader();

    bool Detect();
    bool Connect();

    bool get_flat_finger();

};

#endif // FAP50READER_H
