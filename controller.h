/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 *
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <QObject>
#include <QMap>

#include "org.bluez.Adapter1.h"
#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "devicelistmodel.h"

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(DeviceListModel* pairedDevices, DeviceListModel* otherDevices);
	virtual ~Controller();
	void initialize();

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	void findAdapter();
	void findDevices();

	DeviceListModel* pairedDevices;
	DeviceListModel* otherDevices;
	ObjectManager* objectManager;
	Adapter1* adapter;
	
};

#endif	/* CONTROLLER_H */

