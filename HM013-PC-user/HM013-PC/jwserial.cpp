#include "jwserial.h"
#include <QSerialPort>        //Qt串口功能接口类
#include <QSerialPortInfo>   //提供设备现有串行端口的信息
#include <QDebug>

jwSerial::jwSerial()
{
    serialPort=new QSerialPort;
    mThread   =new QThread();

    this->moveToThread(mThread);
    serialPort->moveToThread(mThread);
    connect(mThread,SIGNAL(startd()),this,SLOT(slot_OpenSerial()));
    qDebug()<<"创建串口线程"<<QThread::currentThread();
}
bool jwSerial::finddevice(QStringList &str)
{
    QStringList list_avail_serialport;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //ui->portComboBox->addItem(qspinfo.portName());
        qDebug()<<"串口名:"<<info.portName();
        qDebug()<<"描述信息:"<<info.description();
        qDebug()<<"制造商:"<<info.manufacturer();
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
        list_avail_serialport.append(serial.portName());
        serial.close();
        }

    }

    #if 0
    serialPort->setPortName(PortName);
    serialPort->setBaudRate(QSerialPort::Baud19200); //设置波特率
    serialPort->setParity(QSerialPort::NoParity);    //设置校验位
    serialPort->setDataBits(QSerialPort::Data8);     //设置数据位
    serialPort->setStopBits(QSerialPort::OneStop);   //设置停止位
    serialPort->open(QIODevice::ReadWrite);
    #endif
    qDebug()<<list_avail_serialport;
    str=list_avail_serialport;
    return false;
}

/*
 * @breif 将字节序列转换为对应的16进制字符串
 */
QString jwSerial::ByteArrayToHexString(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    qDebug()<<len;
    for(int i=1;i<len;i++)
    {
        //qDebug()<<i;
        ret.insert(2*i+i-1," ");
    }

    return ret;
}

QByteArray jwSerial::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();
    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                qDebug()<<"非法的16进制字符："<<s;

            }
        }
    }
    //qDebug()<<ret;
    return ret;
}

void jwSerial::receiveInfo()
{
    QByteArray info = serialPort->readAll();
    QByteArray hexData = info.toHex();
    //这里面的协议 你们自己定义就行  单片机发什么 代表什么 我们这里简单模拟一下
    if(hexData == "0x10000")
    {
        //do something
    }
    else if(hexData  == "0x100001")
    {
        //do something
    }

}

void jwSerial::convertStringToHex(const QString &str, QByteArray &byteData)
{

}


void jwSerial::slot_startThread()
{
    if(!mThread->isRunning()){
        mThread->start();
    }

}
void jwSerial::slot_OpenSerial()
{
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);

    if(serialPort->open(QIODevice::ReadWrite)){
        connect(serialPort,SIGNAL(readRead()),this,SLOT(slot_ReadSerialData()));
    }
}




void jwSerial::slot_ReadSerialData()
{
    QByteArray buf=serialPort->readAll();
    if(!buf.isEmpty()){
        QString str=buf;
        emit serial_signals_sendData(str);
    }
    buf.clear();
}

void jwSerial::slot_CloseThread()
{
    serialPort->clear();
    serialPort->close();
    if(mThread->isRunning()){
       mThread->quit();
    }
}
