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

#include "org.freedesktop.DBus.ObjectManager.h"

class Adapter;
class Device;
class DeviceListModel;
class OrgFreedesktopDBusObjectManagerInterface;

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller(DeviceListModel* pairedDevices, DeviceListModel* otherDevices);
	virtual ~Controller();
	void initialize();

public slots:
	void pairingChanged(QString path, bool paired);
	void togglePairing(QString path);

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	void findAdapter();
	void findDevices();

	DeviceListModel* pairedDevices;
	DeviceListModel* otherDevices;
	OrgFreedesktopDBusObjectManagerInterface* objectManager;
	Adapter* adapter;
	
};

#endif	/* CONTROLLER_H */

