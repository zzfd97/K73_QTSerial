#ifndef APPTHREADHJW_H
#define APPTHREADHJW_H
#include <QObject>
#include "qthread.h"
#include "qstring.h"
#include <QMutex>
#include <QObject>
#include <QTimer>
#include <QMutexLocker>
#include <QWaitCondition>
#include "qbytearray.h"
#include "qtimer.h"
#include <QDebug>


class APPThreadhjw: public QThread
{
    Q_OBJECT
public:
    APPThreadhjw();
   ~APPThreadhjw();


private slots:
    void threadtimeout(void);
signals:
    void threadrun(int value);
protected:
    virtual void run();
};

#endif // APPTHREADHJW_H
