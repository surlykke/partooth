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

	// Find adapters, turn them on
	for (QDBusObjectPath path: objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(ADAPTER1_IF) && interfaceMap.contains(PROPS_IF)) {
			Adapter adapter(path.path());
			adapter.adapterInterface.setPowered(true);
			if (adapter.adapterInterface.powered()) {
				adapter.adapterInterface.StartDiscovery();
			}
			else {
				qWarning() << "Unable to turn on adapter" << adapter.adapterInterface.name();
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



Device* DeviceListModel::device(const QString& path)
{
	for (Device* dev : pairedDevices) {
		if (dev->deviceInterface.path() == path) {
			return dev;
		}
	}
	
	for (Device* dev : otherDevices) {
		if (dev->deviceInterface.path() == path) {
			return dev;
		}
	}

	return NULL;
}

void DeviceListModel::add(const QString& path)
{
	qDebug() << "Adding device " << path; 

	if (! device(path)) {
		Device* device = new Device(path, this);
		connect(&(device->propertiesInterface), 
			    SIGNAL(PropertiesChanged(const QString&, const QVariantMap&, const QStringList&)),
				SLOT(onPropertiesChanged(const QString&, const QVariantMap&, const QStringList&)));

		if (device->deviceInterface.paired()) {
			int row = 1 + pairedDevices.size();
			beginInsertRows(QModelIndex(), row, row);
			pairedDevices.append(device);
			endInsertRows();		
		}
		else {
			int row = 2 + pairedDevices.size() + otherDevices.size();
			beginInsertRows(QModelIndex(), row, row);
			otherDevices.append(device);
			endInsertRows();		
		}
	}
}

void DeviceListModel::remove(const QString& path)
{
	QList<int> indexesToDelete;
	for (int i = 0; i < pairedDevices.size(); i++) {
		if (pairedDevices[i]->deviceInterface.path() == path) {
			removePaired(i);
			return;
		}
	}

	for (int i = 0; i < otherDevices.size(); i++) {
		if (otherDevices[i]->deviceInterface.path() == path) {
			removeOther(i);
			return;
		}
	}
}

int DeviceListModel::row(const QString& path) const
{
	for (int i = 0; i < pairedDevices.size(); i++) {
		if (pairedDevices[i]->deviceInterface.path() == path) {
			return 1 + i;
		}
	}

	for (int i = 0; i < otherDevices.size(); i++) {
		if (otherDevices[i]->deviceInterface.path() == path) {
			return 2 + pairedDevices.size() + i;
		}
	}
}


int DeviceListModel::rowCount(const QModelIndex& parent) const
{
	return rows();
}

QVariant DeviceListModel::data(const QModelIndex& index, int role) const
{
	if (! index.isValid()) {
		return QVariant();
	}
	else if (0> index.row() || index.row() >= rows()) {
		return QVariant();
	}
	else if (index.row() == 0) { // Heading for paired devices
		return dataForHeading(tr("Paired devices"), role);
	}
	else if (index.row() == 1 + pairedDevices.size()) { // Heading for other devices
		return dataForHeading(tr("Other devices"), role);
	}
	else if (index.row() <= pairedDevices.size()) { 
		return dataForDevice(pairedDevices[index.row() - 1], role);
	}
	else {
		return dataForDevice(otherDevices[index.row() - pairedDevices.size() - 2], role);
	}
}

Qt::ItemFlags DeviceListModel::flags(const QModelIndex& index) const
{
	if (index.row() == 0 || index.row() == 1 + pairedDevices.size()) { // one of the 'heading' lines
		return Qt::NoItemFlags;
	}
	else {
		return QAbstractListModel::flags(index);
	}
}


void DeviceListModel::onPropertiesChanged(const QString& interface, const QVariantMap& changed_properties, const QStringList& invalidated_properties)
{
	if (DEVICE1_IF == interface) {
		OrgFreedesktopDBusPropertiesInterface* props = 
			dynamic_cast<OrgFreedesktopDBusPropertiesInterface*>(sender());
		
		if (props) {
			QString path = props->path();
			if (changed_properties.contains("Paired")) {
				remove(path);
				add(path);
			}
			else if (changed_properties.contains("name") || changed_properties.contains("icon")) {
				emit dataChanged(index(row(path)), index(row(path)));
			}
		}
	}

}

void DeviceListModel::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	qDebug() << "Interfaces added for"  << path.path();
	qDebug() << interfaces;
	qDebug() << "----------------------------------------------------------------------------------";
	if (interfaces.contains(DEVICE1_IF)) {
		qDebug() << "Adding" << path.path();
		add(path.path());
	}
}

void DeviceListModel::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	qDebug() << "Interfaces removed for" << path.path();
	qDebug() << interfaces;
	qDebug() << "----------------------------------------------------------------------------------";
	if (interfaces.contains(DEVICE1_IF)) {
		qDebug() << "Removing" << path.path();
		remove(path.path());
	}
}




QVariant DeviceListModel::dataForHeading(QString heading, int role) const
{
	if (role == Qt::DisplayRole) {
		return "    " + heading;
	}
	else if (role == Qt::ForegroundRole) {
		return QColor(Qt::black);
	}
	else if (role == Qt::SizeHintRole) {
		return QSize(0,60);
	}
/*	else if (role == Qt::TextAlignmentRole) {
		return Qt::AlignCenter;
	}*/
	else {
		return QVariant();
	}
}

QVariant DeviceListModel::dataForDevice(Device* device, int role) const
{
	if (role == Qt::DisplayRole) {
		return device->deviceInterface.name();
	}
	else if (role == Qt::DecorationRole) {
		return QIcon::fromTheme(device->deviceInterface.icon());
	}
	else if (role == PathRole) {
		return device->deviceInterface.path();
	}
	else {
		return QVariant();
	}
}




void DeviceListModel::removePaired(int index)
{

	if (index < 0 || index >= pairedDevices.size()) {
		return;
	}
	int row = 1 + index;
	beginRemoveRows(QModelIndex(), row, row);
	pairedDevices.takeAt(index)->deleteLater();
	endRemoveRows();
}

void DeviceListModel::removeOther(int index)
{
	if (index < 0 || index >= otherDevices.size()) {
		return;
	}
	int row = 2 + pairedDevices.size() + index;
	beginRemoveRows(QModelIndex(), row, row);
	otherDevices.takeAt(index)->deleteLater();
	endRemoveRows();

}
