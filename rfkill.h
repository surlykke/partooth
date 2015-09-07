#ifndef RFKILL_H
#define RFKILL_H

#include <QObject>
#include <QFile>

#define DEV_RFKILL "/dev/rfkill"

class Rfkill : public QObject
{
    Q_OBJECT

public:
    explicit Rfkill(QObject *parent = 0);
    void startMonitoring();
    bool hardBlocked;
    bool softBlocked;

signals:
    void changed();

private:
    void monitorDevRfkill();
};

#endif // RFKILL_H

