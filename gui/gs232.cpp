#include "gs232.h"
#include <QDebug>

gs232::gs232()
{
    // Инициализация последовательного порта.
    m_pSerialPort = new QSerialPort();
    m_pSerialPort->setPortName("/dev/ttyUSB0");
    // Скорость передачи данных. 19200 бит/с.
    m_pSerialPort->setBaudRate(QSerialPort::Baud9600);
    m_pSerialPort->setDataBits(QSerialPort::Data8);
    m_pSerialPort->setParity(QSerialPort::NoParity);
    m_pSerialPort->setStopBits(QSerialPort::OneStop);
    m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (!m_pSerialPort->open(QSerialPort::ReadWrite))
    {
        delete m_pSerialPort;
        m_pSerialPort = nullptr;
    }
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &gs232::update);
    //timer->start(1000);
}

void gs232::update()
{
    qDebug() << "update";

    QByteArray sendData;
    sendData.append('C');
    sendData.append(13);
    m_pSerialPort->write(sendData);

    QByteArray datas;
    while(datas.length() < 5){
        if(!m_pSerialPort->waitForReadyRead(1000)){ //block until new data arrives
            qDebug() << "error: " << m_pSerialPort->errorString();
            break;
        } else {
            qDebug() << "New data available: " << m_pSerialPort->bytesAvailable();
            datas.append(m_pSerialPort->readAll());
            qDebug() << datas;
            int value = QString(datas).split("+")[1].toInt();
            qDebug() << "Value: " << value;
            emit positionUpdate(value);
        }
    }
}

void gs232::go(int value)
{
    qDebug() << "go: " << value;
    if (m_pSerialPort == nullptr)
        return;

    QByteArray sendData;
    sendData.append('M');
    sendData.append(QString::number(value).rightJustified(3, '0').toLatin1());
    sendData.append(13);
    m_pSerialPort->write(sendData);
    qDebug() << "sendData: " << sendData;

}



void gs232::stop()
{
    QByteArray sendData;
    sendData.append('S');
    sendData.append(13);
    m_pSerialPort->write(sendData);

}
