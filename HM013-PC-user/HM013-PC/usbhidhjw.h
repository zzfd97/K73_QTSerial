#ifndef USBHIDHJW_H
#define USBHIDHJW_H

#include <QObject>
#include <QWidget>
#include "hidapi.h"

class USBHIDHJW
{
public:
    USBHIDHJW();
    bool USBWrite(unsigned char *p,int len);
    bool USBRead(unsigned char *p,int len);
private:
    hid_device *handle;
};

#endif // USBHIDHJW_H
