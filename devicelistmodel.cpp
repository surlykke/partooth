/* 
 * File:   DeviceListModel.cpp
 * Author: christian
 * 
 * Created on 14. august 2015, 19:56
 */

#include <QIcon>

#include "device.h"
#include "devicelistmodel.h"
#include "constants.h"

DeviceListModel::DeviceListModel()
{
}

DeviceListModel::~DeviceListModel()
{
}

Device* DeviceListModel::device(const QString& path)
{
	for (Device* dev : devices) {
		if (dev->deviceInterface.path() == path) {
			return dev;
		}
	}

	return NULL;
}

Device* DeviceListModel::deviceAt(int row)
{
	if (row < 0 || row >= devices.size()) {
		return NULL;
	}

	return devices[row];
}


void DeviceListModel::add(const QString& path)
{
	qDebug() << "Adding device " << path; 

	if (! device(path)) {
		Device* device = new Device(path, this);
		connect(&(device->propertiesInterface), 
			    SIGNAL(PropertiesChanged(const QString&, const QVariantMap&, const QStringList&)),
				SLOT(onPropertiesChanged(const QString&, const QVariantMap&, const QStringList&)));

		beginInsertRows(QModelIndex(), devices.size(), devices.size());
		devices.append(new Device(path, this));
		endInsertRows();
	}
}

void DeviceListModel::remove(const QString& path)
{
	QList<int> indexesToDelete;
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i]->deviceInterface.path() == path) {
			removeAt(i);
			return;
		}
	}
}

int DeviceListModel::rowCount(const QModelIndex& parent) const
{
	return devices.size();
}

QVariant DeviceListModel::data(const QModelIndex& index, int role) const
{
	if (! index.isValid()) {
		return QVariant();
	}
	else if (0> index.row() || index.row() >= devices.size()) {
		return QVariant();
	}
	else if (role == Qt::DisplayRole) {
		return devices[index.row()]->deviceInterface.name();
	}
	else if (role == Qt::DecorationRole) {
		return QIcon::fromTheme(devices[index.row()]->deviceInterface.icon()); // FIXME fallback
	}
	else if (role == PathRole) {
		return devices[index.row()]->deviceInterface.path();
	}
	else {
		return QVariant();
	}
}

void DeviceListModel::onPropertiesChanged(const QString& interface, const QVariantMap& changed_properties, const QStringList& invalidated_properties)
{
	if (DEVICE1_IF == interface) {
		OrgFreedesktopDBusPropertiesInterface* props = dynamic_cast<OrgFreedesktopDBusPropertiesInterface*>(sender());
		qDebug() << "props:" << props;
		if (props) {
			if (changed_properties.contains("Paired")) {
				emit pairingChanged(props->path(), changed_properties["Paired"].toBool());
			}
			else if (changed_properties.contains("name") || changed_properties.contains("icon")) {
				for (int i = 0; i < devices.size(); i++) {
					if (devices[i]->deviceInterface.path() == props->path()) {
						emit dataChanged(index(i), index(i));
						return;
					}
				}	
			}
		}
	}

}


void DeviceListModel::removeAt(int row)
{
	if (row < 0 || row >= devices.size()) {
		return;
	}

	beginRemoveRows(QModelIndex(), row, row);
	devices.takeAt(row)->deleteLater();
	endRemoveRows();
}

