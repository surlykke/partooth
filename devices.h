/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 *
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef DEVICES_H
#define	DEVICES_H

#include <QObject>
#include <QMap>

#include "org.bluez.Adapter1.h"
#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.ObjectManager.h"


class Devices : public QObject
{
	Q_OBJECT

public:
	Devices(QObject *parent = 0);
	virtual ~Devices();
	void initialize();

signals:
	void deviceAdded(Device1* device);	
	void deviceAboutToBeRemoved(Device1* device);

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	void findAdapter();
	void findDevices();

	ObjectManager* objectManager;
	Adapter1* adapter;
	QMap<QString, Device1*> devices;
};

#endif	/* PROXIES_H */

