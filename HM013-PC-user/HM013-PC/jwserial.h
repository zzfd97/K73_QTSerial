#ifndef JWSERIAL_H
#define JWSERIAL_H

#include <QObject>
#include <QSerialPort>        //Qt串口功能接口类
#include <QSerialPortInfo> //提供设备现有串行端口的信息
#include <QStringList>
#include <QThread>

class jwSerial:public QObject
{
    Q_OBJECT
public:
    jwSerial();
    bool finddevice(QStringList &str);
    QString ByteArrayToHexString(QByteArray data);
    QByteArray HexStringToByteArray(QString HexString);
    void convertStringToHex(const QString &str, QByteArray &byteData);
    void receiveInfo();
signals:
    void serial_signals_sendData(QString str);
public slots:
    void slot_startThread();
    void slot_OpenSerial();
    void slot_ReadSerialData();
    void slot_CloseThread();
private:
    QSerialPort  *serialPort;
    QThread      *mThread;
};

#endif // JWSERIAL_H
