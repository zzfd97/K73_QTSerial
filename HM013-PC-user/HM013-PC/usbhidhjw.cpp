#include "usbhidhjw.h"
#include <QThread>
#include <QLatin1String>
#include <QString>
#include "qdebug.h"

USBHIDHJW::USBHIDHJW()
{
    struct hid_device_info *devs, *cur_dev;
    unsigned char buf[256];
    int res;
    res = hid_init();
    devs = hid_enumerate(0x0a12, 0x4007);
    cur_dev = devs;
    while (cur_dev) {
    //qDebug()<<"Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls"<< cur_dev->vendor_id<<cur_dev->product_id<< cur_dev->path<<cur_dev->serial_number;
    //qDebug()<<"  Manufacturer: %ls\n"<<cur_dev->manufacturer_string;
    //qDebug()<<"Interface"<<cur_dev->interface_number;
    //qDebug()<<"VID"<<cur_dev->vendor_id;
    //qDebug()<<"PID"<<cur_dev->product_id;
    //printf("VID is %x",cur_dev->vendor_id);
   // printf("PID is %x",cur_dev->product_id);
    cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);
    handle = hid_open(0x0a12, 0x4007, NULL);

    if(handle ==NULL) return ;

    qDebug()<<"handle "<<handle;
    if(handle!=NULL)
    hid_set_nonblocking(handle, 1);

    buf[0] = 0x01;
    buf[1] = 0x02;
    buf[2] = 0x01;
    buf[3] = 0x01;
#if 1
    res = hid_write(handle,(unsigned char*) buf, 4);
    if (res < 0) {
        qDebug()<<"hid_write false!";
    }else{
        qDebug()<<"hid_write successfully!"<<res;
    }
    while (res == 0) {
    qDebug()<<"hid_read "<<res;
    res = hid_read(handle, (unsigned char*)buf, sizeof(buf));
    qDebug()<<"hid_read res"<<res;
    QThread::sleep(500);
    if (res == 0)
    qDebug()<<"waiting...\n";
    if (res < 0)
    printf("Unable to read()\n");
    }

    for(int i=0;i<res;i++){
    printf("%02hhx",buf[i]);
    }
    printf("begin to read()\n");
#endif
}
bool USBHIDHJW::USBWrite(unsigned char *p,int len)
{
    if((p==NULL)||(len<=0)) return true;
    if(handle!=NULL)
    hid_write(handle,p,len);
    return false;
}
bool USBHIDHJW::USBRead(unsigned char *p,int len)
{
    if((p==NULL)||(len<=0)) return true;
    return false;
}
