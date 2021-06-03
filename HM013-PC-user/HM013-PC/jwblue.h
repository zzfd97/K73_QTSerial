#ifndef JWBLUE_H
#define JWBLUE_H

#include <QObject>

#include<QBluetoothDeviceDiscoveryAgent>
#include<QBluetoothDeviceInfo>
#include<QBluetoothUuid>
#include<QBluetoothServiceInfo>
#include<QLowEnergyController>
#include<QLowEnergyService>
#include<QLowEnergyDescriptor>
class jwBlue: public QObject{
    Q_OBJECT
public:

    jwBlue();
    QString getAddress(QBluetoothDeviceInfo device) const;
private:
        QList<QBluetoothDeviceInfo> device_list;  //存放搜索到到蓝牙设备列表
        QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;  //设备搜索对象
        QLowEnergyController *m_controler;   //单个蓝牙设备控制器
        QLowEnergyService *m_service; //服务对象实例
};


#endif // JWBLUE_H
