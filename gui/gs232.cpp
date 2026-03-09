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
        //return;
    }
    updateTimer = new QTimer(this);
    rotateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &gs232::update);
    connect(rotateTimer, &QTimer::timeout, this, &gs232::update);
    updateTimer->start(1000);

    cfg.Az.overlap_position = 225;
    cfg.Az.overlap_size = 10;

    /* Расчет виртуального нуля             */
    /* 180 - overlap pos                    */
    if (cfg.Az.overlap_position >= 180){
        virtualZero = cfg.Az.overlap_position - 180;
    } else {
        virtualZero = cfg.Az.overlap_position + 180;
    }
    virtualZero = (virtualZero > 180)?(virtualZero - 360):virtualZero;
    qDebug() << "Virtual zero: " << virtualZero;
}

/*=========================================*/
/* Расчет допустимых направлений           */
void gs232::calcDir(void)
{
    if (dirAllowed.wire_right){
        /* Провод расположен справа от антенны */
        /* В overlap зону можно заходить против часовой стрелки */
        dirAllowed.right_overlap = 0;
        dirAllowed.right = 1;
        if (tmpTargetAzimuth == (-180 - cfg.Az.overlap_size)){
            /* Дошли до края overlap зоны */
            dirAllowed.left = 0;
            dirAllowed.left_overlap = 1;
        } else if ((tmpTargetAzimuth <= 0) && tmpTargetAzimuth > -180){
            /* Ещё не вошли в overlap зону */
            dirAllowed.left = 1;
            dirAllowed.left_overlap = 0;
        } else if (tmpTargetAzimuth <= 0){
            /* Внутри overlap зоны */
            dirAllowed.left = 1;
            dirAllowed.left_overlap = 1;
        } else {
            /* Переход через 0 */
        }
    } else if (dirAllowed.wire_left){
        /* Провод расположен слева от антенны */
        /* В overlap зону можно заходить по часовой стрелке */
        dirAllowed.left_overlap = 0;
        dirAllowed.left = 1;
        if (tmpTargetAzimuth == (180 + cfg.Az.overlap_size)){
            /* Дошли до края overlap зоны */
            dirAllowed.right = 0;
            dirAllowed.right_overlap = 0;
        } else if ((tmpTargetAzimuth >= 0) && tmpTargetAzimuth < 180){
            /* Ещё не вошли в overlap зону */
            dirAllowed.right = 1;
            dirAllowed.right_overlap = 0;
        } else if (tmpTargetAzimuth >= 0){
            /* Внутри overlap зоны */
            dirAllowed.right = 1;
            dirAllowed.right_overlap = 1;
        } else {
            /* Переход через 0 */
        }
    } else {
        /* Антенна находится на виртуальном нуле */
        /* Разрешено использовать любое направление */
        dirAllowed.right = 1;
        dirAllowed.left = 1;
        dirAllowed.right_overlap = 0;
        dirAllowed.left_overlap = 0;
    }
    qDebug() << "calcDir: " << dirAllowed.left << dirAllowed.left_overlap << dirAllowed.right_overlap << dirAllowed.right;
}

void gs232::update()
{
    if (targetAzimuth == antAzimuth){
        dir = 0;
        rotateTimer->stop();
    } else {
        antAzimuth += dir;
        if (antAzimuth >= 360) antAzimuth -= 360;
        if (antAzimuth < 0) antAzimuth += 360;
        emit positionUpdate(antAzimuth);
    }
    if (m_pSerialPort == nullptr) return;

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

/*=========================================*/
/* Расчет зоны overlap                     */
void gs232::azConvert(void)
{
    qDebug() << "orig: " << "ant: " << antAzimuth << " target: " << targetAzimuth;
    /* Поворот координат на виртуальный 0    */
    tmpAntAzimuth = antAzimuth - virtualZero;
    if (tmpAntAzimuth < 0) tmpAntAzimuth += 360;
    if (tmpAntAzimuth >= 360) tmpAntAzimuth -= 360;
    tmpTargetAzimuth = targetAzimuth - virtualZero;
    if (tmpTargetAzimuth < 0) tmpTargetAzimuth += 360;
    if (tmpTargetAzimuth >= 360) tmpTargetAzimuth -= 360;
    qDebug() << "rotate: " << "tant: " << tmpAntAzimuth << " ttarget: " << tmpTargetAzimuth;

    /* Переход от 0 - 360 в -180 - 180       */
    tmpAntAzimuth = (tmpAntAzimuth > 180)?(tmpAntAzimuth - 360):tmpAntAzimuth;
    tmpTargetAzimuth = (tmpTargetAzimuth > 180)?(tmpTargetAzimuth - 360):tmpTargetAzimuth;
    qDebug() << "out: " << "tant: " << tmpAntAzimuth << " ttarget: " << tmpTargetAzimuth;

    /* С какой стороны антенны провод */
    if (tmpAntAzimuth == 0){
        /* В виртуальном нуле, состояние провода */
        /* не определено                         */
        dirAllowed.wire_right = 0;
        dirAllowed.wire_left = 0;
    } else {
        if (dirAllowed.wire_right == 0 && dirAllowed.wire_left == 0){
            /* Начали движение от нуля, все просто */
            /* Иные случаи игнорируем */
            if (tmpAntAzimuth > 0){
                dirAllowed.wire_left = 1;
            } else{
                dirAllowed.wire_right = 1;
            }
        }
        if (dirAllowed.wire_right && dirAllowed.wire_left){
            /* Такой ситуации не может быть. */
            dirAllowed.wire_left = 0;
            dirAllowed.wire_right = 0;
        }
    }

    qDebug() << "wire: left: " << dirAllowed.wire_left << " right: " << dirAllowed.wire_right;
    /* Расширение шкалы -180 - 180 на размер overlap зоны */
    if (dirAllowed.wire_left){
        if (tmpAntAzimuth < 0) tmpAntAzimuth += 360;
        if (tmpTargetAzimuth < 0 && tmpTargetAzimuth <= (-180 + cfg.Az.overlap_size)) tmpTargetAzimuth += 360;
    }
    if (dirAllowed.wire_right){
        if (tmpAntAzimuth > 0) tmpAntAzimuth -= 360;
        if (tmpTargetAzimuth > 0 && tmpTargetAzimuth >= (180 - cfg.Az.overlap_size)) tmpTargetAzimuth -= 360;
    }
}

void gs232::go(int value)
{
    if (value >= 360) value -= 360;
    targetAzimuth = value;
    azConvert();
    if (tmpTargetAzimuth < tmpAntAzimuth){
        dir = -1;
    } else {
        dir = 1;
    }
    qDebug() << "ant: " << antAzimuth << " target: " << targetAzimuth << "tant: " << tmpAntAzimuth << " ttarget: " << tmpTargetAzimuth;
    rotateTimer->start(75);
    calcDir();

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
