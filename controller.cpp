/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 *
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */


#include "controller.h"
#include "device.h"
#include "adapter.h"
#include "constants.h"
#include "devicelistmodel.h"

Controller::Controller(DeviceListModel* pairedDevices, DeviceListModel* otherDevices):
	QObject(),
	pairedDevices(pairedDevices),
	otherDevices(otherDevices),
	objectManager(0),
	adapter(0)
{
}

Controller::~Controller()
{
}

void Controller::initialize()
{
	connect(pairedDevices, SIGNAL(pairingChanged(QString, bool)), SLOT(pairingChanged(QString, bool)));
	connect(otherDevices, SIGNAL(pairingChanged(QString, bool)), SLOT(pairingChanged(QString, bool)));

	qDBusRegisterMetaType<PropertyMap>();
	qDBusRegisterMetaType<InterfaceMap>();
	qDBusRegisterMetaType<ObjectMap>();
	qDBusRegisterMetaType<QStringList>();
		
	objectManager = new OrgFreedesktopDBusObjectManagerInterface(BLUEZ_SERVICE, "/", SYS_BUS);
	ObjectMap objectMap = objectManager->GetManagedObjects();

	qDebug() << "connecting to objectmanager";

	connect(objectManager, 
		    SIGNAL(InterfacesAdded(const QDBusObjectPath&, InterfaceMap)),
			this, 
		    SLOT(onInterfacesAdded(const QDBusObjectPath&, InterfaceMap)));
	
	qDebug() << "InterfacesAdded connected";

	connect(objectManager, 
		    SIGNAL(InterfacesRemoved(const QDBusObjectPath&, const QStringList&)),
			this, 
		    SLOT(onInterfacesRemoved(const QDBusObjectPath&, const QStringList&)));

	qDebug() << "InterfacesRemoved connected";

	/*
	 * We look for an object implementing the org.bluez.Adapter1 interface. For now
	 * we don't handle having more than one adapter.
     */
	for (QDBusObjectPath path: objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(ADAPTER1_IF) && interfaceMap.contains(PROPS_IF)) {
			adapter = new Adapter(path.path());
			break;
		}
	}

	if (adapter) {
		
		if (! adapter->adapterInterface.powered()) {
			adapter->adapterInterface.setPowered(true);
			if (! adapter->adapterInterface.powered()) {
				qWarning() << "Unable to turn on adapter" << adapter->adapterInterface.name();
				return;
			}
		}

		qDebug() << "Have connected adapter";
		//	Find devices for our adapter
		for (QDBusObjectPath path: objectMap.keys()) {
			onInterfacesAdded(path, objectMap[path]);
		}

		adapter->adapterInterface.StartDiscovery();	
		qDebug() << "initialization done...";
	}
	else {
		qWarning() << "No bluetooth adapter found!";
	}
}

void Controller::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		qDebug() << "Device added";
		qDebug() << "path:" << path.path() 
		         << ", name=" << interfaces[DEVICE1_IF]["Name"]
		         << ", Paired=" << interfaces[DEVICE1_IF]["Paired"];
		if (interfaces[DEVICE1_IF]["Paired"].toBool()) {
			pairedDevices->add(path.path());
		}
		else {
			otherDevices->add(path.path());
		}
	}
}

void Controller::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	qDebug() << "Interfaces removed:" << path.path();
	qDebug() << interfaces;
	if (interfaces.contains(DEVICE1_IF)) {
		pairedDevices->remove(path.path());
		otherDevices->remove(path.path());
	}
}

void Controller::togglePairing(QString path)
{
	qDebug() << "Controller::togglePairing(" << path << ")";
	Device* device;
	if (device = pairedDevices->device(path)) {
		qDebug() << "Remove..";
		Adapter(device->deviceInterface.adapter().path()).adapterInterface.RemoveDevice(QDBusObjectPath(path));
	}
	else if (device = otherDevices->device(path)) {
		qDebug() << "Pair..";
		device->deviceInterface.Pair();
	}
}

void Controller::pairingChanged(QString path, bool paired)
{
	(paired ? otherDevices : pairedDevices)->remove(path);
	(paired ? pairedDevices : otherDevices)->add(path);
}

