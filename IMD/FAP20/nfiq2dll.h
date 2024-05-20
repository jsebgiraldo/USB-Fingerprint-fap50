#ifndef NFIQ2DLL_H
#define NFIQ2DLL_H

#include <QByteArray>


class Nfiq2Dll
{
public:

    Nfiq2Dll();

    QByteArray InitNfiq(QByteArray &hash);
    void FreeIntPtr(char* ptr);
    char* ConvertToIntPtr(std::byte array[]);

    int SafeComputeNfiq2Score(int fpos, char *pixels,int size, int width, int height, int ppi);

    bool isInit = false;
};

#endif // NFIQ2DLL_H
