#include <linux/rfkill.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>

#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "rfkill.h"

Rfkill::Rfkill(QObject *parent) : QObject(parent)
{
    qDebug() << "Connect deviceReadyRead";
}

void Rfkill::startMonitoring()
{
    QtConcurrent::run(this, &Rfkill::monitorDevRfkill);
}


// To be run in a thread
void Rfkill::monitorDevRfkill()
{
    qDebug() << "starting monitorDevRfkill thread";
    int fd = open(DEV_RFKILL, O_RDONLY);
    if (fd < 1) {
        qDebug() << "Unable to open" << DEV_RFKILL << ":" << strerror(errno);
        return;
    }

    while (1) {
        struct pollfd pollfd;
        pollfd.events = POLLIN;
        pollfd.fd = fd;
        pollfd.revents = 0;
        qDebug() << "Polling";
        poll(&pollfd, 1, -1);

        struct rfkill_event event;
        int bufsize = sizeof(struct rfkill_event);

        qDebug() << "Reading";
        if (read(fd, &event, bufsize) == bufsize) {
            qDebug() << "Read";
            if (event.type == RFKILL_TYPE_BLUETOOTH) {
                hardBlocked = event.hard != 0;
                softBlocked = event.soft != 0;
                emit changed();
            }
        }
        else {
            qWarning() << "Partial read of" << DEV_RFKILL;
        }
    }
}


