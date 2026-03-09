#ifndef GS232_H
#define GS232_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class gs232 : public QObject
{
    Q_OBJECT

    struct dir {
        unsigned right: 1;
        unsigned right_overlap: 1;
        unsigned left: 1;
        unsigned left_overlap: 1;
        unsigned up: 1;
        unsigned down: 1;
        unsigned wire_right;
        unsigned wire_left;
    };
    struct configAz {
        int16_t count;
        int16_t overlap_position;
        int16_t overlap_size;
        int16_t parking;
    };

    struct configEl {
        int16_t count;
        uint8_t min;
        uint8_t max;
        uint8_t parking;
    };

    struct configFlags {
        unsigned com_echo: 1;
        unsigned ui_use_old: 1;
        unsigned el_enable: 1;
    };

    struct config {
        struct configAz Az;
        struct configEl El;
        struct configFlags Flags;
    };


public slots:
    Q_INVOKABLE void go(int value);
    void stop();
public:
    gs232();

signals:
    void positionUpdate(int value);

private slots:
    void update(void);

private:
    void azConvert(void);
    void calcDir(void);
    int16_t antAzimuth = 0;
    int16_t targetAzimuth = 0;
    int16_t tmpAntAzimuth = 0;
    int16_t tmpTargetAzimuth = 0;
    int16_t virtualZero = 180;
    int8_t dir = 0;

    struct dir dirAllowed;
    struct config cfg;

    QSerialPort* m_pSerialPort;
    QTimer *updateTimer;
    QTimer *rotateTimer;
};

#endif // GS232_H
