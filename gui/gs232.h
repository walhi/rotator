#ifndef GS232_H
#define GS232_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class gs232 : public QObject
{
    Q_OBJECT

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


    QSerialPort* m_pSerialPort;
    QTimer *timer;
};

#endif // GS232_H
