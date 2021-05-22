#include "appthreadhjw.h"
#include <QtCore/qmath.h>



APPThreadhjw::APPThreadhjw()
{
    QTimer::singleShot(500, Qt::CoarseTimer, this, SLOT(threadtimeout()));
}
APPThreadhjw::~APPThreadhjw()
{
    this->quit();
    this->wait();
}


void APPThreadhjw:: threadtimeout(void)
{
     qDebug()<<"threadtimeout";
     //QTimer::singleShot(500, Qt::CoarseTimer, this, SLOT(threadtimeout()));
}

void APPThreadhjw::run()
{
    static int i=0;
    static int y=0;
    while(1)
    {
        //qDebug()<<tr(" QThread::currentThreadId()==")<<QThread::currentThreadId();
        msleep(10);

        //y=1-qSqrt(i);

        if(++i>1000){
            i=0;
        }
        if(y>1000){
            y=0;
            i=0;
        }
        emit threadrun(i);
    }
}


