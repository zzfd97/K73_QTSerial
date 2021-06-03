#include "mainwindow.h"
#include "usbhidhjw.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include "qdebug.h"
#include <QMessageBox>
#include <QSerialPort>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    QDateTime time=QDateTime::currentDateTime();
    timer=new QTimer(this);

    setMinimumSize(1103,718);
    setMaximumSize(1103,718);
    setWindowOpacity(0.90);
    this->setWindowTitle(tr("K73_xiaomi"));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);
    //create serial port


    jwSerialThread* m_serial=new jwSerialThread;
    mSerialPort=new QSerialPort;

    if(m_serial!=nullptr){
        qDebug()<<"创建串口成功！";
    }else{
         qDebug()<<"创建串口失败！";
    }


    ui->setupUi(this);

    /************************************************************/

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
     discoveryAgent = new QBluetoothDeviceDiscoveryAgent;
     connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(discoverBlueTooth(QBluetoothDeviceInfo)));
     connect(discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
     discoveryAgent->start();

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

    m_serial->moveToThread(&mainThread);
    connect(&mainThread,&QThread::finished,m_serial,&QObject::deleteLater);
    connect(this,&MainWindow::serialDataSend,m_serial,&jwSerialThread::doDataSend);
    //connect(&mSerialPort,&QSerialPort::readyRead,m_serial,&jwSerialThread::doDataReceive);
    connect(m_serial,&jwSerialThread::sendResulttoGUI,this,&MainWindow::on_serial_show);
    mainThread.start();
    //connect(&mSerialPort,&QSerialPort::readyRead,this,&MainWindow::serialDataRead);
}

#if 0
void MainWindow::serialDataRead(void)
{
    // 获取当前时间字符串
      QDateTime current_date_time =QDateTime::currentDateTime();
      QString dateStr =current_date_time.toString("[yyyy-MM-dd hh:mm:ss.zzz]");

      //从接收缓冲区中读取数据
      QByteArray buffer = mSerialPort.readAll();
      QString bufferStr = ByteArrayToHexString(buffer);

      QString displayStr = dateStr+"\n"+bufferStr+"\n";

      //从界面中读取以前收到的数据
      QString oldString = ui->textEdit_3->toPlainText();
      oldString = oldString + QString(displayStr);

      ui->textEdit_3->clear();
      ui->textEdit_3->append(oldString);
}
#endif


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

/*
 * @breif 将字节序列转换为对应的16进制字符串
 */
QString MainWindow::ByteArrayToHexString(QByteArray data)
{
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    qDebug()<<"ByteArrayToHexString"<<len;
    for(int i=1;i<len;i++)
    {
        //qDebug()<<i;
        ret.insert(2*i+i-1," ");
    }

    return ret;
}
/*
 * @breif 将16进制字符串转换为对应的字节序列
 */
QByteArray MainWindow::HexStringToByteArray(QString HexString)
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
                QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    //qDebug()<<ret;
    return ret;
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
    ui->serial_plot->yAxis->setRange(-50, 120);
    ui->serial_plot->yAxis->setLabel("- - - - - -任务进程- - - - - - -");
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->serial_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->serial_plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->serial_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->serial_plot->yAxis2, SLOT(setRange(QCPRange)));
    dataTimer= new QTimer(this);
    //connect(dataTimer, SIGNAL(timeout()),this, SLOT(realtimeDataSlot()));
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
    bool ok;
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/300.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  //if (key-lastPointKey > 0.100) // at most add point every 2 ms
  {
      //add data to lines:
    char *cdata=mByteSendData.data();
    int length=mByteSendData.size();
    qDebug()<<"realtimeDataSlot"<<*cdata<<length;
    QVector<double> fdata;
    //QDataStream stream(&mByteSendData,QIODevice::WriteOnly);

    qDebug()<<"QDataStream"<<fdata;
    //qDebug()<<"hex"<<mByteSendData.fromRawData(cdata,length);
    for(int i=0;i<mByteSendData.size();i++){
    ui->serial_plot->graph(0)->addData(key,mByteSendData.at(i));
    qDebug()<<"hex1"<<mByteSendData.toHex().at(i);
    }

    lastPointKey = key;
    mByteSendData.clear();
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->serial_plot->xAxis->setRange(key, 8, Qt::AlignTrailing);
  ui->serial_plot->replot();
  //ui->widget->show();
  // calculate frames per second:
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
    find_SerialDevice();
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

void MainWindow::on_comboBox_2_activated(const QString &arg1)
{

    ui->comboBox_2->currentIndex();
    qDebug()<<ui->comboBox_2->currentIndex();
}

void MainWindow::on_pushButton_7_clicked()
{
    QString str=ui->pushButton_7->text();

    if(mSerialPort!=nullptr){
        ui->textBrowser_3->append("创建串口成功!");
    }else{
        ui->textBrowser_3->append("创建串口失败!");
    }

    if(str==tr("开始")){
        //dataTimer->start();
        ui->pushButton_7->setText(tr("停止"));

        if(mSerialPort!=nullptr){
            mSerialPort->setPortName(ui->comboBox_7->currentText());
            mSerialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
            //数据位
            switch(ui->comboBox_4->currentIndex()){
            case 8:
            mSerialPort->setDataBits(QSerialPort::Data8);
            break;
            }
            //停止位
            switch(ui->comboBox_3->currentIndex()){
            case 1:
            mSerialPort->setStopBits(QSerialPort::OneStop);
            break;
            }
            //设置流控制
            mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
            ui->comboBox_3->setEnabled(false);
            ui->comboBox_4->setEnabled(false);
            if(mSerialPort->open(QIODevice::ReadWrite)){
                mSerialPort->write("sdghghahs");
                qDebug()<<"\r\n 串口发送成";
            }
                qDebug()<<"\r\n 串口发送成错误";
          }

    }else{
        //dataTimer->stop();
        ui->pushButton_7->setText(tr("开始"));
        if(mSerialPort!=nullptr){
            mSerialPort->clear();
            mSerialPort->close();
            mSerialPort->deleteLater();
            ui->comboBox_3->setEnabled(true);
            ui->comboBox_4->setEnabled(true);
        }
    }
#if 0
    QString serialportname=ui->comboBox_7->currentText();

    qDebug()<<"m_serial"<<m_serial;
    if(m_serial!=nullptr){
        if(m_serial->jwSerialisOpen()){
           //m_serial->jwSerialPort(ui->comboBox_7->currentText(),ui->comboBox_2->currentText());

        }else{

        }
        mainThread.start();
        //connect(m_serial,SIGNAL(sendResulttoGUI(QByteArray)),this,SLOT(on_serial_show(QByteArray)),Qt::QueuedConnection);
    }
#endif
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    int ofdex=ui->tabWidget->currentIndex();
    index=ofdex;
    switch(ofdex){
        case 0:

        break;

        case 2:
                //CreatePlot();
        break;
        case 3:
                //CreatWaveplot();
        break;
    }

    qDebug()<<"tabweight"<<ofdex;
}

void MainWindow::on_comboBox_7_activated(const QString &arg1)
{
    int ofindex=ui->comboBox_7->currentIndex();

}

void MainWindow::on_pushButton_9_clicked()
{
    char *str_data;
    QString strdata;
    QString strend;
    QByteArray data;
    strdata=ui->textEdit_3->toPlainText();;
    //mSerialPort.write(data);
    data=strdata.toLatin1();
    str_data=data.data();
    strend=QString(QLatin1String(str_data));
    mByteSendData=data;//.data();


    realtimeDataSlot();

    qDebug()<<"mByteSendData"<<str_data<<strend;

}

void MainWindow::on_pushButton_10_clicked()
{
   ui->textEdit_3->clear();

}
