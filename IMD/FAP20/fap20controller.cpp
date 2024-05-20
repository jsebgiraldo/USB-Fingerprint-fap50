#include "fap20controller.h"

#include <mutex>
#include <QLibrary>

typedef INT_PTR(CALLBACK* FPPROC)(WPARAM, LPARAM);

extern "C" {
__declspec(dllimport) int __stdcall OpenDevice();
__declspec(dllimport) int __stdcall CloseDevice();
__declspec(dllimport) int __stdcall ResetDevice();
__declspec(dllimport) int __stdcall LinkDevice(int password);
__declspec(dllimport) void __stdcall CaptureImage();
__declspec(dllimport) void __stdcall CaptureTemplate();
__declspec(dllimport) void __stdcall EnrollTemplate();
__declspec(dllimport) int __stdcall DeviceSoundBeep(int beep);
__declspec(dllimport) int __stdcall GetWorkMsg();
__declspec(dllimport) int __stdcall GetRetMsg();
__declspec(dllimport) int __stdcall GetImageSize(int * width, int * height);
__declspec(dllimport) int __stdcall GetRawImage(std::byte * imagedata, int * size);
__declspec(dllimport) int __stdcall GetBmpImage(std::byte * imagedata, int * size);
__declspec(dllimport) int __stdcall CreateTemplate(std::byte * imagedata, int * width, int * height, std::byte * tp, int * templateSize);
__declspec(dllimport) bool __stdcall  GetTemplateByEnl(std::byte * fpbuf, int * fpsize);
__declspec(dllimport) int __stdcall  MatchTemplate(std::byte * pSrcData, std::byte * pDstData);
__declspec(dllimport) bool __stdcall DeviceLedState(int type, int status);
}

Fap20Controller::Fap20Controller() {

}

int Fap20Controller::SafeOpenDevice()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return OpenDevice();
}

int Fap20Controller::SafeCloseDevice()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return CloseDevice();

}

int Fap20Controller::SafeResetDevice()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return ResetDevice();
}

int Fap20Controller::SafeLinkDevice(int password)
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return LinkDevice(password);
}

void Fap20Controller::SafeCaptureImage()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    CaptureImage();
    return;
}

void Fap20Controller::SafeCaptureTemplate()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    CaptureTemplate();
    return;
}

void Fap20Controller::SafeEnrollTemplate()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    EnrollTemplate();
    return;
}

int Fap20Controller::SafeDeviceSoundBeep(int beep)
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return DeviceSoundBeep(beep);
}

int Fap20Controller::SafeGetImageSize(int *width, int *height){
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetImageSize(width,height);
}


int Fap20Controller::SafeGetRawImage(std::byte * imagedata, int * size){
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetRawImage(imagedata,size);
}

int Fap20Controller::SafeGetBmpImage(std::byte * imagedata, int * size){
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetBmpImage(imagedata,size);
}

int Fap20Controller::SafeGetWorkMsg()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetWorkMsg();
}

int Fap20Controller::SafeGetRetMsg()
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetRetMsg();
}

int Fap20Controller::SafeCreateTemplate(std::byte * pImage, int width, int height, std::byte * tp, int * templateSize)
{
    std::lock_guard<std::mutex> lock(_syncLock);
    return CreateTemplate(pImage, &width, &height, tp, templateSize);
}

bool Fap20Controller::SafeGetTemplateByEnl(std::byte * fpbuf, int * fpsize){
    std::lock_guard<std::mutex> lock(_syncLock);
    return GetTemplateByEnl(fpbuf, fpsize);
}

int Fap20Controller::SafeMatchTemplate(std::byte * pSrcData, std::byte * pDstData){
    std::lock_guard<std::mutex> lock(_syncLock);
    return MatchTemplate(pSrcData, pDstData);
}

bool Fap20Controller::SafeDeviceLedState(int type, int status)
{
    return DeviceLedState(type,status);
}
