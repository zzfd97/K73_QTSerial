#include "mainwindow.h"
#include "usbhidhjw.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include "qdebug.h"
#include <QMessageBox>
#include <QSerialPort>




void MainWindow::stringTohexString(QString& str, QString& hexStr)
{
    str=str;
    hexStr=hexStr;
}

int MainWindow::hexStringToString(QString& hexStr, QString& str)
{
    int ret = 0;
    bool ok;
    QByteArray retByte;
    hexStr = hexStr.trimmed();
    hexStr = hexStr.simplified();
    QStringList sl = hexStr.split(" ");

    foreach (QString s, sl)
    {
        if(!s.isEmpty())
        {
            char c = (s.toInt(&ok,16))&0xFF;
            if (ok)
            {
                retByte.append(c);
            }
            else
            {
                ret = -1;
            }
        }
    }

    str = retByte;

    return ret;
}

int MainWindow::hexStringToHexArray(QString& hexStr, QByteArray& arr)
{
    int ret = 0;
    bool ok;
    hexStr = hexStr.trimmed();
    hexStr = hexStr.simplified();
    QStringList sl = hexStr.split(" ");

    foreach (QString s, sl)
    {
        if(!s.isEmpty())
        {
            char c = (s.toInt(&ok,16))&0xFF;
            if (ok)
            {
                arr.append(c);
            }
            else
            {
                ret = -1;
            }
        }
    }

    return ret;
}

int MainWindow::hexArrayToString(QByteArray& hexArr, QString& str)
{
    int ret = 0;
    str = hexArr.toHex(' ').toLower();
    return ret;
}

void MainWindow::CreateSerialportWindow(void)
{
    QStringList list;
    list.clear();
    list << "2400" << "4800" << "9600" << "14400" << \
    "19200" << "38400" << "43000" << "57600" << "76800" << \
    "115200" << "230400" << "256000" << "460800" << "921600";
    ui->comboBox_2->addItems(list);
    ui->comboBox_2->setCurrentText(tr("115200"));
    //////////////////////////////////////////////////////////////
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (-1 == ui->comboBox_7->findText(info.portName()))
            ui->comboBox_7->addItem(info.portName());
    }
    mSerialPort.close();
    mSerialPort.setReadBufferSize(1024*100);
    mSerialPort.setSettingsRestoredOnClose(true);

    //连接信号和槽
    QObject::connect(&mSerialPort, &QSerialPort::readyRead, this, &MainWindow::readyRead);
    QObject::connect(this, &MainWindow::sig_deviceChanged, this, &MainWindow::on_deviceChanged);
    // 注册各类设备，可根据要求删减
#if 0
    static const GUID GUID_DEVINTERFACE_LIST[] =
    {
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
    //// GUID_DEVINTERFACE_COMPORT
    //{ 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
    //// GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
    //{ 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
    //// GUID_DEVINTERFACE_PARALLEL
    //{ 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
    //// GUID_DEVINTERFACE_PARCLASS
    //{ 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
    };

    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for(int i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++)
    {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();//m_usb->GetDriverGUID();
        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify)
        {
            DWORD Err = GetLastError();
            qDebug() << "注册失败" <<endl;
        }
        //else
    }
#endif

    on_btnOpen_clicked();

}
void MainWindow::readyRead()
{
    QByteArray buffer = mSerialPort.readAll();
    mByteSendData=buffer;
    QString info;
    QString tmpStr;
    QString timeStr = "";
    //m_rxCnt += buffer.size();
    //m_stsRx->setText("RX: " + QString::number(m_rxCnt));

    QDateTime dateTime(QDateTime::currentDateTime());
    timeStr = "[" + dateTime.toString(" HH:mm:ss.zzz") + "] ";

    m_recvHex=0;
    if (m_recvHex == 1)
    {
        info = buffer.toHex(' ').data();
        for (int i = 0; i < buffer.size(); i++)
        {
            tmpStr.sprintf("%02x ", buffer.at(i));
            qDebug() << buffer.at(i);
            info.append(tmpStr);
        }
    }
    else
    {
        info = QString(buffer);
    }
    //ui->textBrowser_3->clear();
    ui->textBrowser_3->append(timeStr + info);
    // TODO
    // 似乎直接用QByteArray无法直接取真正的值
    // 这里先转为数组，再判断，需要优化
    char *data = (char*)buffer.data();
    uint8_t buf[255] = {0};
    m_rxCnt+=buffer.size();
    if(buffer.contains("jw"))
    {
        //for (int i = 0; i < buffer.size(); i++)
        if(buffer.size()>5)
        {
           buf[0]=buffer.at(2)-48;
           buf[1]=buffer.at(3)-48;
           buf[2]=buffer.at(4)-48;
           //qDebug()<<"接收正确帧"<<int(buf[0]*100+buf[1]*10+buf[2]);
        }
        //解析接收数据

    }else{
         //qDebug()<<"接收错误帧";
    }


    // 根据值判断做逻辑处理，可做成函数
    if (buf[0] == 0xaa && buf[1] == 0x55)
    {

    }

    mSerialPort.clear();
}

void MainWindow::on_deviceChanged(int flag)
{
    if (flag == 1)
    {
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if (-1 == ui->comboBox_7->findText(info.portName()))
                ui->comboBox_7->addItem(info.portName());
        }
    }
    else
    {
        mSerialPort.close();
        ui->pushButton_7->setText(tr("打开串口"));
        //ui->pushButton_7->setIcon(QIcon(":images/notopened.ico"));
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QDateTime time=QDateTime::currentDateTime();
    timer=new QTimer(this);

    setMinimumSize(1103,718);
    setMaximumSize(1103,718);
    setWindowOpacity(0.90);
    this->setWindowTitle(tr("K73_小米"));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);
    //create serial port

    ui->setupUi(this);

    /************************************************************/
#if  0
     QBluetoothDeviceDiscoveryAgent *discoveryAgent;
     discoveryAgent = new QBluetoothDeviceDiscoveryAgent;
     connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(discoverBlueTooth(QBluetoothDeviceInfo)));
     connect(discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
     discoveryAgent->start();
#endif
     /************************************************************/


    ui->dial->setMinimum(0);
    ui->dial->setMaximum(255);
    ui->dial->setNotchTarget(1);
    ui->dial->setNotchesVisible(true);
    ui->dial->setValue(0x26);
    ui->lcdNumber->display(0x26);

    ui->dial_2->setMinimum(0);
    ui->dial_2->setMaximum(255);
    ui->dial_2->setNotchTarget(1);
    ui->dial_2->setNotchesVisible(true);
    ui->dial_2->setValue(0x26);
    ui->lcdNumber_2->display(0x26);

    ui->dial_3->setMinimum(0);
    ui->dial_3->setMaximum(255);
    ui->dial_3->setNotchTarget(1);
    ui->dial_3->setNotchesVisible(true);
    ui->dial_3->setValue(0x26);
    ui->lcdNumber_3->display(0x26);

    ui->dial_4->setMinimum(0);
    ui->dial_4->setMaximum(255);
    ui->dial_4->setNotchTarget(1);
    ui->dial_4->setNotchesVisible(true);
    ui->dial_4->setValue(0x26);
    ui->lcdNumber_4->display(0x26);

    ui->verticalSlider_2->setMinimum(0);
    ui->verticalSlider_2->setMaximum(100);
    ui->verticalSlider_2->setSingleStep(10);
    ui->verticalSlider_2->setTickPosition(QSlider::TicksLeft);

    ui->verticalSlider_3->setMinimum(0);
    ui->verticalSlider_3->setMaximum(100);
    ui->verticalSlider_3->setSingleStep(10);
    ui->verticalSlider_3->setTickPosition(QSlider::TicksLeft);

    ui->verticalSlider_4->setMinimum(0);
    ui->verticalSlider_4->setMaximum(100);
    ui->verticalSlider_4->setSingleStep(10);
    ui->verticalSlider_4->setTickPosition(QSlider::TicksLeft);


    ui->verticalSlider_5->setMinimum(0);
    ui->verticalSlider_5->setMaximum(100);
    ui->verticalSlider_5->setSingleStep(10);
    ui->verticalSlider_5->setTickPosition(QSlider::TicksLeft);

    ui->progressBar->setMaximum(1000);
    ui->progressBar->setRange(0,1000);


    ///////////////////////////////////////////////////
    /// \brief connect
    ///
    CreateBlackImage();
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(LableTextShow()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(LableTextShow()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(LableTextShow()));

    //plot an image draw

    const QAudioDeviceInfo inputDevice = QAudioDeviceInfo::defaultInputDevice();
    if (inputDevice.isNull()) {
        QMessageBox::warning(nullptr, "audio",
                             "There is no audio input device available.");
    }

    QPalette pal(ui->widget->palette());

    //设置背景黑色
    pal.setColor(QPalette::Background, Qt::black);
    ui->widget->setAutoFillBackground(true);
    ui->widget->setPalette(pal);
    ui->widget->show();
    plot=new Plothjw(inputDevice,ui->widget);

    //USB handle
    //USBHIDHJW hjw;
    m_file =new FileProtocolParsehjw;
    //pMainThread =new QThreadPool;
    //pMainThread->setMaxThreadCount(10);
    //for(int i=0;i<10;i++){
        //m_serial->setAutoDelete(true);
        //pMainThread->start(m_serial);
    //}
    CreatePlot();
    CreatWaveplot();

#if 0
    m_serial->moveToThread(&mainThread);
    connect(&mainThread,&QThread::finished,m_serial,&QObject::deleteLater);
    connect(this,&MainWindow::serialDataSend,m_serial,&jwSerialThread::doDataSend);
    //connect(&mSerialPort,&QSerialPort::readyRead,m_serial,&jwSerialThread::doDataReceive);
    connect(m_serial,&jwSerialThread::sendResulttoGUI,this,&MainWindow::on_serial_show);
    mainThread.start();
#endif
    CreateSerialportWindow();
}



void MainWindow::discoveryFinished()
{
    qDebug()<<"discoveryFinished";
    static QString serviceUuid("00001101-0000-1000-8000-00805F9B34FB");
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->connectToService(QBluetoothAddress(BTaddress), QBluetoothUuid(serviceUuid),QIODevice::ReadWrite);
    connect(socket,SIGNAL(readyRead()), this, SLOT(readBluetoothDataEvent()));
    connect(socket,SIGNAL(connected()), this, SLOT(bluetoothConnectedEvent()));
\
}

void MainWindow::discoverBlueTooth(QBluetoothDeviceInfo info)
{
    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());
     if(info.name()=="HC-06")
     {
         BTaddress = info.address().toString();
     }
     qDebug()<<label;
}

void MainWindow::readBluetoothDataEvent()
{
   char data[100];
   qint64 len = socket->read((char *)data, 100);

   QByteArray qa2((char*)data,len);
   QString qstr(qa2.toHex());//
   qDebug()<<"----"<<qstr.toUpper();
}



void MainWindow::bluetoothDataSend(QString str)
{
    QByteArray arrayData;
    arrayData = str.toUtf8();
    socket->write(arrayData);
}

// 连接成功的事件中添加一行发送数据：
void MainWindow::bluetoothConnectedEvent()
{
    qDebug()<<"bluetoothConnectedEvent";
    bluetoothDataSend("hellow bluetooth");
}



void MainWindow::CreatePlot(void)
{
    ui->serial_plot->addGraph();
    ui->serial_plot->addGraph(); // blue line
    ui->serial_plot->graph(0)->setPen(QPen(QColor(255, 110, 34)));
    ui->serial_plot->addGraph(); // red line
    ui->serial_plot->graph(1)->setPen(QPen(QColor(12, 110, 200)));
    ui->serial_plot->addGraph(); // red line
    ui->serial_plot->graph(2)->setPen(QPen(QColor(12, 0, 200)));
    ui->serial_plot->addGraph(); // red line
    ui->serial_plot->graph(3)->setPen(QPen(QColor(110, 110, 200)));
    ui->serial_plot->addGraph(); // red line
    ui->serial_plot->graph(4)->setPen(QPen(QColor(200, 110, 200)));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->serial_plot->xAxis->setTicker(timeTicker);
    ui->serial_plot->xAxis->setLabel("- - - - - - - - - - - - - - - - - - - -调试时间- - - - - - - - - - - - - - - - - - -");
    ui->serial_plot->axisRect()->setupFullAxesBox();
    ui->serial_plot->yAxis->setRange(-100, 1000);
    ui->serial_plot->yAxis->setLabel("- - - - - -任务进程- - - - - - -");
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->serial_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->serial_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->serial_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->serial_plot->yAxis2, SLOT(setRange(QCPRange)));
    dataTimer= new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()),this, SLOT(realtimeDataSlot()));
    dataTimer->start(100);
}

void MainWindow::CreatWaveplot(void)
{

    // prepare data:
    QVector<double> x1(20), y1(20);
    QVector<double> x2(100), y2(100);
    QVector<double> x3(20), y3(20);
    QVector<double> x4(20), y4(20);
    for (int i=0; i<x1.size(); ++i)
    {
      x1[i] = i/(double)(x1.size()-1)*10;
      y1[i] = qCos(x1[i]*0.8+qSin(x1[i]*0.16+1.0))*qSin(x1[i]*0.54)+1.4;
    }
    for (int i=0; i<x2.size(); ++i)
    {
      x2[i] = i/(double)(x2.size()-1)*10;
      y2[i] = qCos(x2[i]*0.85+qSin(x2[i]*0.165+1.1))*qSin(x2[i]*0.50)+1.7;
    }
    for (int i=0; i<x3.size(); ++i)
    {
      x3[i] = i/(double)(x3.size()-1)*10;
      y3[i] = 0.05+3*(0.5+qCos(x3[i]*x3[i]*0.2+2)*0.5)/(double)(x3[i]+0.7)+qrand()/(double)RAND_MAX*0.01;
    }
    for (int i=0; i<x4.size(); ++i)
    {
      x4[i] = x3[i];
      y4[i] = (0.5-y3[i])+((x4[i]-2)*(x4[i]-2)*0.02);
    }

    // create and configure plottables:
    QCPGraph *graph1 =  ui->serial_plot->addGraph();
    graph1->setData(x1, y1);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));

    QCPGraph *graph2 =  ui->serial_plot->addGraph();
    graph2->setData(x2, y2);
    graph2->setPen(Qt::NoPen);
    graph2->setBrush(QColor(200, 200, 200, 20));
    graph2->setChannelFillGraph(graph1);

    QCPBars *bars1 = new QCPBars( ui->serial_plot->xAxis,  ui->serial_plot->yAxis);
    bars1->setWidth(9/(double)x3.size());
    bars1->setData(x3, y3);
    bars1->setPen(Qt::NoPen);
    bars1->setBrush(QColor(10, 140, 70, 160));

    QCPBars *bars2 = new QCPBars( ui->serial_plot->xAxis,  ui->serial_plot->yAxis);
    bars2->setWidth(9/(double)x4.size());
    bars2->setData(x4, y4);
    bars2->setPen(Qt::NoPen);
    bars2->setBrush(QColor(10, 100, 50, 70));
    bars2->moveAbove(bars1);

    // move bars above graphs and grid below bars:
    ui->graph_plot->addLayer("abovemain",  ui->serial_plot->layer("main"), QCustomPlot::limAbove);
    ui->graph_plot->addLayer("belowmain",  ui->serial_plot->layer("main"), QCustomPlot::limBelow);
    graph1->setLayer("abovemain");
    ui->graph_plot->xAxis->grid()->setLayer("belowmain");
    ui->graph_plot->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    ui->graph_plot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->graph_plot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->graph_plot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->graph_plot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->graph_plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->graph_plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->graph_plot->xAxis->setTickLabelColor(Qt::white);
    ui->graph_plot->yAxis->setTickLabelColor(Qt::white);
    ui->graph_plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->graph_plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->graph_plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->graph_plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->graph_plot->xAxis->grid()->setSubGridVisible(true);
    ui->graph_plot->yAxis->grid()->setSubGridVisible(true);
    ui->graph_plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->graph_plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->graph_plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->graph_plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->graph_plot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->graph_plot->axisRect()->setBackground(axisRectGradient);

    ui->graph_plot->rescaleAxes();
    ui->graph_plot->yAxis->setRange(0, 2);
    ui->graph_plot->replot();
    ui->graph_plot->show();
}

void MainWindow::realtimeDataSlot()
{
  double plotData;
  bool ok;
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/300.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  ////////////////////////////////////////////////////////////////////
  char *data = (char*)mByteSendData.data();
  uint8_t buf[255] = {0};
  m_rxCnt+=mByteSendData.size();
  if(mByteSendData.contains("jw"))
  {
      //for (int i = 0; i < buffer.size(); i++)
      if(mByteSendData.size()>5)
      {
         buf[0]=mByteSendData.at(2)-48;
         buf[1]=mByteSendData.at(3)-48;
         buf[2]=mByteSendData.at(4)-48;
         plotData=double(buf[0]*100+buf[1]*10+buf[2]);
         qDebug()<<"接收正确帧"<<int(buf[0]*100+buf[1]*10+buf[2]);
         ui->serial_plot->graph(0)->addData(key,plotData);
      }
      //解析接收数据
  }
  ////////////////////////////////////////////////////////////////////

  //if (key-lastPointKey > 0.100) // at most add point every 2 ms
  {
    ui->serial_plot->replot();
    //plotData=mByteSendData.toDouble(&ok);
    //ui->serial_plot->yAxis->setRange(double(-(plotData+300)), double(plotData+300));
    //ui->serial_plot->graph(0)->addData(key,plotData);
    lastPointKey = key;

  }

  mByteSendData.clear();
  mSerialPort.clear();

  // make key axis range scroll with the data (at a constant range size of 8):
  ui->serial_plot->xAxis->setRange(key, 8, Qt::AlignTrailing);
  ui->serial_plot->replot();
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    #if 0
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->widget_2->graph(0)->data()->size()+ui->widget_2->graph(1)->data()->size())
          , 0);
    #endif
    lastFpsKey = key;
    frameCount = 0;
  }
}

void MainWindow::on_serial_show(QByteArray data)
{
    ui->textBrowser_3->append(data);
    qDebug()<<"on_serial_show"<<data;
}

bool MainWindow::find_SerialDevice(void)
{
    //查找可用的串口
    ui->comboBox_7->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //mserialportList.append(info.portName()+info.description());
        qDebug()<<"串口名:"<<info.portName();
        qDebug()<<"描述信息:"<<info.description();
        qDebug()<<"制造商:"<<info.manufacturer();

        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
        ui->comboBox_7->addItem(serial.portName());
        serial.close();
        }

    }

    //serialThread->run();

}

MainWindow::~MainWindow()
{
    //jwSerialThread->quit();
    //jwSerialThread->wait();

    delete timer;
    //delete jwSerialThread;
    delete m_file;
    delete ui;
    mainThread.quit();
    mainThread.wait();
}
void MainWindow::ThreadTxt(int value)
{
    //ui->textEdit->append("run ...");
    ui->progressBar->setValue(value);
}

void MainWindow::timerUpdate()
{
    //static int cout=0;
    QDateTime time=QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    ui->label_2->setText(str);
}

void MainWindow::LableTextShow(void)
{
    pAnimation->start();
}



void MainWindow::on_pushButton_2_clicked()//导入
{
    m_file->FileOpen();
    QByteArray byte=m_file->FileRead();
    ui->textBrowser->append(byte);
}

void MainWindow::on_dial_valueChanged(int value)
{
     ui->lcdNumber->display(value);
}
void MainWindow::CreateBlackImage(void)
{
    pAnimation = new QPropertyAnimation(ui->label_3, "geometry");
    pAnimation->setDuration(1000);
    pAnimation->setStartValue(QRect(10, 540, 1081, 171));
    pAnimation->setEndValue(QRect(500, 200, 75, 25));
    pAnimation->setEasingCurve(QEasingCurve::SineCurve);
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_widget_windowIconTextChanged(const QString &iconText)
{
    QString str=iconText;
}

void MainWindow::on_dial_2_valueChanged(int value)
{
    ui->lcdNumber_2->display(value);
}

void MainWindow::on_dial_3_valueChanged(int value)
{
    ui->lcdNumber_3->display(value);
}

void MainWindow::on_dial_4_valueChanged(int value)
{
    ui->lcdNumber_4->display(value);
}

void MainWindow::on_pushButton_8_clicked()
{
     QStringList str;

     if(str.size()==0){
          ui->textBrowser_3->setText(tr("未发现"));
     }
     for(int i=0;i<str.size();i++){
        ui->textBrowser_3->setText(str[i]);
     }
    //find_SerialDevice();
}
 void MainWindow::mainGUISendData(void)
 {
    QByteArray data;
    emit serialDataSend(data);
 }
void MainWindow::hanldeResults(QString &result)
{
    qDebug()<<result<<"线程ID:"<<QThread::currentThreadId();
}


void MainWindow::on_pushButton_7_clicked()
{
     qDebug()<<ui->comboBox_7->currentText();
     qDebug()<<ui->pushButton_7->text();
    if(ui->pushButton_7->text()==QString(tr("打开串口")))
    {
#if 0
        // 串口设备
        on_cbPortName_currentTextChanged(ui->comboBox_7->currentText());
        // 波特率（注：QtSerial支持的枚举不多，但设置了其它值也行）
        on_cbBaudrate_currentTextChanged(ui->comboBox_2->currentText());
        //设置数据位
        on_cbDatabit_currentTextChanged(ui->comboBox_4->currentText());
        //设置停止位
        on_cbStopbit_currentIndexChanged(ui->comboBox_3->currentIndex());
        //设置奇偶校验
        on_cbParity_currentIndexChanged(ui->comboBox_5->currentIndex());
        //设置流控制
        on_cbFlow_currentIndexChanged(ui->comboBox_6->currentIndex());
#endif
        // 打开串口

        mSerialPort.setPortName(ui->comboBox_7->currentText());
        mSerialPort.setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);//设置波特率和读写方向
        mSerialPort.setDataBits(QSerialPort::Data8);		//数据位为8位
        mSerialPort.setFlowControl(QSerialPort::NoFlowControl);//无流控制
        mSerialPort.setParity(QSerialPort::NoParity);	//无校验位
        mSerialPort.setStopBits(QSerialPort::OneStop); //一位停止位

        showMessage("port opened.");
        ui->textBrowser_3->setText("串口已找到");
        ui->pushButton_7->setText(tr("关闭串口"));
        mSerialPort.open(QIODevice::ReadWrite);
        //ui->pushButton_7->setIcon(QIcon(":images/opened.ico"));
        if(!mSerialPort.open(QIODevice::ReadWrite) && !mSerialPort.isOpen())
        {
            //QMessageBox::about(NULL, tr("info"), tr("open port failed."));
            showMessage("open port failed.\n");
            return;
        }
    }
    else
    {
        mSerialPort.close();

        showMessage("port closed.");
        ui->pushButton_7->setText(tr("打开串口"));
        //ui->pushButton_7->setIcon(QIcon(":images/notopened.ico"));
        qDebug()<<"serial open fiale";
    }
}

void MainWindow::serialport_receivedata()
{
    QByteArray info = mSerialPort.readAll();
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
    qDebug()<<"\r\n hjw ------>";
}

void MainWindow::showMessage(const char* str)
{
    QString tmp = str;
    ui->statusbar->showMessage(tmp, 500);
}






void MainWindow::on_pushButton_9_clicked()
{
    Serialport_SendData();
}

void MainWindow::on_pushButton_10_clicked()
{
     ui->serialSendWindow->clear();
}



void MainWindow::on_btnOpen_clicked()
{


    if(ui->pushButton_7->text()==QString("打开串口"))
    {
        // 打开串口
        if(!mSerialPort.open(QIODevice::ReadWrite) && !mSerialPort.isOpen())
        {
            //QMessageBox::about(NULL, tr("info"), tr("open port failed."));
            showMessage("open port failed.\n");
            return;
        }

        showMessage("port opened.");
        ui->pushButton_7->setText(tr("关闭串口"));
        ui->pushButton_7->setIcon(QIcon(":images/opened.ico"));
    }
    else
    {
        mSerialPort.close();

        showMessage("port closed.");
        ui->pushButton_7->setText(tr("打开串口"));
        ui->pushButton_7->setIcon(QIcon(":images/notopened.ico"));
    }
}

void MainWindow::on_btnSaveRecv_clicked()
{

}

void MainWindow::on_btnClearRecv_clicked()
{
    ui->textBrowser_3->clear();
}

void MainWindow::on_ckRecvHex_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        m_recvHex = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        m_recvHex = 0;
    }
}

void MainWindow::on_ckTimestamp_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        //m_showTimestamp = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        //m_showTimestamp = 0;
    }
}

void MainWindow::on_btnSend_clicked()
{
#if 0
    if (m_sendTimer)
    {

        if(ui->serialSendWindow->text()==QString("发送"))
        {
            if (m_sendTimerId == 0)
                m_sendTimerId = startTimer(ui->txtInterval->text().toInt());
            //ui->serialSendWindow->setText(tr("停止发送"));
        }
        else
        {
            if (m_sendTimerId)
            {
                killTimer(m_sendTimerId);
                m_sendTimerId = 0;
            }
            ui->btnSend->setText(tr("发送"));
        }
    }
    else
    {
        sendData();
    }
#endif
}

void MainWindow::Serialport_SendData(void)
{


    if (!mSerialPort.isOpen())
    {
        showMessage("serial port not opened.");
        return;
    }



    QString sendStr = ui->serialSendWindow->toPlainText().toLatin1().toLower();

    QByteArray sendData;
    QString showStr;
    m_sendHex=0;
 #if 1
    if (m_sendHex == 1)
    {
        hexStringToHexArray(sendStr, sendData);
    }
    else if (m_sendHex == 0)
    {
        sendData = sendStr.toLatin1();
    }
    if (m_sendNewline == 1)//回车
    {
        sendData.append(0x0d);
        sendData.append(0x0a);
    }
    qDebug() << sendData;
   // m_txCnt += sendData.size();
    //m_stsTx->setText("TX: " + QString::number(m_txCnt));
#endif
    mSerialPort.write(sendData);
    qDebug()<<"发送中...";
}

void MainWindow::on_btnClearSend_clicked()
{

}

void MainWindow::on_chSendHex_stateChanged(int arg1)
{
    QString sendStr ;
    QByteArray sendData = sendStr.toLatin1();//ui->txtSend->toPlainText().toLatin1();
    QString tmpStr;
    QString showStr;
    //qDebug() << sendStr;
    //qDebug() << sendData;
    if (arg1 == Qt::Checked)
    {
        m_sendHex = 1;
        showStr = sendData.toHex(' ').data();
    }
    else if (arg1 == Qt::Unchecked)
    {
        m_sendHex = 0;
        hexStringToString(sendStr, showStr); // 用string来转
    }

}

void MainWindow::on_ckSendTimer_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        m_sendTimer = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        m_sendTimer = 0;
        if (m_sendTimerId)
        {
            killTimer(m_sendTimerId);
            m_sendTimerId = 0;
           // ui->serialSendWindow->setText("发送");
        }
    }
}


void MainWindow::on_chSendNewline_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        m_sendNewline = 1;
    }
    else if (arg1 == Qt::Unchecked)
    {
        m_sendNewline = 0;
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->textBrowser_3->clear();
}


void MainWindow::on_m_recvHex_clicked()
{
    m_sendHex=m_sendHex?1:0;
    qDebug()<<"m_sendHex"<<m_sendHex;
}

void MainWindow::on_m_sendHex_clicked()
{
    m_recvHex=m_recvHex?1:0;
    qDebug()<<"m_recvHex"<<m_recvHex;
}
