/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 *
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */


#include "controller.h"
#include "org.bluez.Device1.h"
#define BLUEZ_SERVICE "org.bluez" 
#define ADAPTER1_IF "org.bluez.Adapter1" 
#define DEVICE1_IF "org.bluez.Device1" 

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
	qDBusRegisterMetaType<PropertyMap>();
	qDBusRegisterMetaType<InterfaceMap>();
	qDBusRegisterMetaType<ObjectMap>();
	qDBusRegisterMetaType<QStringList>();
		
	objectManager = new ObjectManager("org.bluez", "/", QDBusConnection::systemBus());
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
		if (interfaceMap.contains(ADAPTER1_IF)) {
			adapter = new Adapter1(BLUEZ_SERVICE, path.path(), QDBusConnection::systemBus(), this);
			break;
		}
	}

	if (adapter) {
		
		if (! adapter->powered()) {
			adapter->setPowered(true);
			if (! adapter->powered()) {
				qWarning() << "Unable to turn on adapter" << adapter->name();
				return;
			}
		}

		qDebug() << "Have connected adapter";
		//	Find devices for our adapter
		for (QDBusObjectPath path: objectMap.keys()) {
			onInterfacesAdded(path, objectMap[path]);
		}

		adapter->StartDiscovery();	
		qDebug() << "initialization done...";
	}
	else {
		qWarning() << "No bluetooth adapter found!";
	}
}

void Controller::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	qDebug() << "Consider device at:" << path.path();
	pairedDevices->removeAll(path);
	otherDevices->removeAll(path);
	if (interfaces.contains(DEVICE1_IF)) {
		Device1* device = new Device1(BLUEZ_SERVICE, path.path(), QDBusConnection::systemBus());
	
		if (device->paired()) {
			qDebug() << "Adding paired device:" << device->name();
			pairedDevices->addDevice(device);
		}
		else {
			qDebug() << "Adding unpaired device:" << device->name();
			otherDevices->addDevice(device);
		}
	}
}

void Controller::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	qDebug() << "Interfaces removed:" << interfaces;
	if (interfaces.contains(DEVICE1_IF)) {
		pairedDevices->removeAll(path);
		otherDevices->removeAll(path);
	}
}

void Controller::forgetDevice(int row)
{
	Device1* device = pairedDevices->at(row);
	Adapter1 adapter(BLUEZ_SERVICE, device->adapter().path(), QDBusConnection::systemBus());
	adapter.RemoveDevice(QDBusObjectPath(device->path()));
}


void Controller::pairDevice(int row)
{
	otherDevices->at(row)->Pair();
}

