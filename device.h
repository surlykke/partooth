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


class Device : public OrgBluezDevice1Interface
{
	Q_OBJECT

public:
	Device(QString path, QObject* parent = 0);
	virtual ~Device();
	OrgFreedesktopDBusPropertiesInterface* propertiesInterface;

signals:
	void propertiesChanged(QString path);

private slots:
	void onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties);

	void showServices();
};

#endif	/* DEVICE_H */

