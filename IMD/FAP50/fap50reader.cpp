#include "Fap50reader.h"
#include "qdebug.h"
#include "qlogging.h"


Fap50Reader::Fap50Reader() {




}

bool Fap50Reader::Detect(){
    IMD_RESULT res = device_reset();
    return res == IMD_RLT_SUCCESS ? true : false;
}

bool Fap50Reader::Connect(){
    SystemProperty property = get_system_property();
    qDebug() << property.chip_id;
    return property.chip_id == 100992006 ? true : false;
}
