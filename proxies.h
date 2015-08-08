/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 *
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef PROXIES_H
#define	PROXIES_H

#include <QObject>
#include <QMap>

#include "org.bluez.Adapter1.h"
#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.ObjectManager.h"

class Proxies : public QObject
{
	Q_OBJECT

public:
	Proxies();
	virtual ~Proxies();
	void initialize();

signals:
	void deviceAdded(QString path, Device1* newDevice);
	void deviceRemoved(QString path);
	void deviceChanged(QString path);

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	void findAdapter();
	void findDevices();

	ObjectManager* objectManager;
	Adapter1* adapter;
	QMap<QString , Device1*> devices;
};

#endif	/* PROXIES_H */

