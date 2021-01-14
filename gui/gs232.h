#ifndef GS232_H
#define GS232_H

#include <QObject>
#include <QSerialPort>

class gs232 : public QObject
{
    Q_OBJECT

public slots:
    Q_INVOKABLE void go(int value);
    void stop();
public:
    gs232();
private:
    QSerialPort* m_pSerialPort;
};

#endif // GS232_H
