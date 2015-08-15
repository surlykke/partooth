/* 
 * File:   device.h
 * Author: christian
 *
 * Created on 15. august 2015, 09:56
 */

#ifndef DEVICE_H
#define	DEVICE_H

#include <QObject>

#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.Properties.h"


class Device : public QObject
{
public:
	Device(QString path, QObject* parent = 0);
	virtual ~Device();
	OrgBluezDevice1Interface deviceInterface;
	OrgFreedesktopDBusPropertiesInterface propertiesInterface;
	const QDBusObjectPath objectPath;
};

#endif	/* DEVICE_H */

