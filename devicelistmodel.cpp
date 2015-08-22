/* 
 * File:   DeviceListModel.cpp
 * Author: christian
 * 
 * Created on 14. august 2015, 19:56
 */

#include <QIcon>
#include <QFont>

#include "device.h"
#include "adapter.h"
#include "devicelistmodel.h"
#include "constants.h"
#include "org.freedesktop.DBus.ObjectManager.h"

DeviceListModel::DeviceListModel()
{
}

DeviceListModel::~DeviceListModel()
{
}

void DeviceListModel::initialize()
{
	qDBusRegisterMetaType<PropertyMap>();
	qDBusRegisterMetaType<InterfaceMap>();
	qDBusRegisterMetaType<ObjectMap>();
	qDBusRegisterMetaType<QStringList>();
		
	OrgFreedesktopDBusObjectManagerInterface* objectManager = 
		new OrgFreedesktopDBusObjectManagerInterface(BLUEZ_SERVICE, "/", SYS_BUS);

	qDebug() << "Connecting interfaces added";
	connect(objectManager, 
		    SIGNAL(InterfacesAdded(const QDBusObjectPath&, InterfaceMap)),
		    SLOT(onInterfacesAdded(const QDBusObjectPath&, InterfaceMap)));

	qDebug() << "Connecting interfaces removed";
	connect(objectManager, 
		    SIGNAL(InterfacesRemoved(const QDBusObjectPath&, const QStringList&)),
		    SLOT(onInterfacesRemoved(const QDBusObjectPath&, const QStringList&)));

		
	ObjectMap objectMap = objectManager->GetManagedObjects();

	// Find adapters, turn them on, make them scan
	for (QDBusObjectPath path: objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(ADAPTER1_IF) && interfaceMap.contains(PROPS_IF)) {
			Adapter adapter(path.path());
			adapter.setPowered(true);
			if (adapter.powered()) {
				adapter.StartDiscovery();
			}
			else {
				qWarning() << "Unable to turn on adapter" << adapter.name();
			}
		}
	}

	// Find all devices and add them
	for (QDBusObjectPath path : objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(DEVICE1_IF)) {
			add(path.path());
		}
	}

}



void DeviceListModel::add(const QString& path)
{

	if (! devices.contains(path)) {
		devices[path] = new Device(path, this);
		emit deviceAdded(devices[path]);
	}
}

void DeviceListModel::remove(const QString& path)
{
	if (devices.contains(path)) {
		devices.take(path)->deleteLater();
	}
}


void DeviceListModel::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		add(path.path());
	}
}

void DeviceListModel::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		remove(path.path());
	}
}




