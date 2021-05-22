#include "plothjw.h"
#include<QObject>
#include <QApplication>
#include<QWidget>


QT_CHARTS_USE_NAMESPACE

Plothjw::Plothjw()
{

}

Plothjw::Plothjw(const QAudioDeviceInfo &deviceInfo, QWidget *parent) :
    m_chart(new QChart),
    m_series(new QLineSeries)
{
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(440, 280);
    m_chart->addSeries(m_series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, XYSeriesIODevice::sampleCount);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Noise Envirment");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-0.5, 1.3);
    axisY->setTitleText("Noise Level");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_series->attachAxis(axisX);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);
    m_chart->legend()->hide();
    m_chart->setTitle("Data from the microphone (" + deviceInfo.deviceName() + ')');

    QVBoxLayout *mainLayout = new QVBoxLayout(parent);
    mainLayout->addWidget(chartView);

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(8);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    m_audioInput = new QAudioInput(deviceInfo, formatAudio, NULL);

    m_device = new XYSeriesIODevice(m_series, parent);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput->start(m_device);
}

Plothjw::~Plothjw()
{
    m_audioInput->stop();
    m_device->close();
}
