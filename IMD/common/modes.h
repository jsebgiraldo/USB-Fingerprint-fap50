#ifndef MODES_H
#define MODES_H
/*
#define FPM_DEVICE  0x01 //Device
#define FPM_PLACE   0x02   //Place Finger
#define FPM_LIFT   0x03    //Lift Finger
#define FPM_CAPTURE   0x04 //Capute Image
#define FPM_GENCHAR   0x05 //Capture Feature
#define FPM_ENRFPT   0x06  //Enrol Feature
#define FPM_NEWIMAGE   0x07//New Image
#define FPM_TIMEOUT   0x08 //Time Out
#define FPM_BUSY   0xFF    //Device Busy

#define SOUND_BEEP   0
#define SOUND_OK   1
#define SOUND_FAIL   2

#define TEMPLATE_SIZE   1024*/

enum class e_capture_modes{
    captureStart,
    captureAuto,
    captureForced,
    enrollStart,
    enrollAuto,
    authenticationStart,
    authenticationAuto,
    verificationStart,
    verificationAuto,
    noMode
};

#endif // MODES_H
