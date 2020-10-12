#include "installationTools/cryptdevice.h"

CryptDevice::CryptDevice(QIODevice* deviceToUse, QObject* parent) :
 QIODevice(parent),
 underlyingDevice(deviceToUse)
{

}

bool CryptDevice::isSequential() const
{
    return true;
}

bool CryptDevice::open(OpenMode mode)
{
    bool underlyingOk;

    if (underlyingDevice->isOpen())
        underlyingOk = (underlyingDevice->openMode() != mode);
    else
        underlyingOk = underlyingDevice->open(mode);

    if (underlyingOk)
    {
        setOpenMode(mode);
        return true;
    }

    return false;
}

void CryptDevice::close()
{
    underlyingDevice->close();
    setOpenMode(NotOpen);
}

qint64 CryptDevice::readData(char* data, qint64 maxSize)
{
    qint64 deviceRead = underlyingDevice->read(data, maxSize);

    if (deviceRead == 1)
        return -1;

    for (qint64 i = 0; i < deviceRead; i++)
        data[i] = data[i] ^ 0x5E;

    return deviceRead;
}

qint64 CryptDevice::writeData(const char* data, qint64 maxSize)
{
    QByteArray buffer(static_cast<int>(maxSize), 0);

    for (int i = 0; i < static_cast<int>(maxSize); i++)
        buffer[i] = data[i] ^ 0x5E;

    return underlyingDevice->write(buffer.data(), maxSize);
}
