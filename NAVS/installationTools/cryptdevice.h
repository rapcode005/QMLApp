#ifndef CRYPTDEVICE_H
#define CRYPTDEVICE_H

#include <QObject>
#include <QIODevice>

class CryptDevice : public QIODevice
{

    Q_OBJECT

public:
    CryptDevice(QIODevice* deviceToUse, QObject* parent = nullptr);
    bool open(OpenMode mode);
    void close();
    bool isSequential() const;

protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64 writeData(const char* data, qint64 maxSize);

private:
    QIODevice* underlyingDevice;
    Q_DISABLE_COPY(CryptDevice)

};

#endif // CRYPTDEVICE_H
