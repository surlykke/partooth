/* 
 * File:   DeviceListModel.cpp
 * Author: christian
 * 
 * Created on 14. august 2015, 19:56
 */

#include <qt/QtGui/qicon.h>

#include "devicelistmodel.h"

DeviceListModel::DeviceListModel()
{
}

DeviceListModel::~DeviceListModel()
{
}

void DeviceListModel::addDevice(Device1* device)
{
	qDebug() << "Adding device " << device->name() << "at:" << size();
	beginInsertRows(QModelIndex(), size(), size());
	append(device);
	endInsertRows();
}


void DeviceListModel::removeAll(const QDBusObjectPath& path)
{
	QList<int> indexesToDelete;
	for (int i = 0; i < size(); i++) {
		if (at(i)->path() == path.path()) {
			indexesToDelete.append(i);
		}
	}

	// We delete in descending order, so that the index of not-yet-deleted doesn't change
	while (! indexesToDelete.empty()) {
		int index = indexesToDelete.takeLast();
		beginRemoveRows(QModelIndex(), index, index);
		takeAt(index);
		endRemoveRows();
	}
}

bool DeviceListModel::contains(const QDBusObjectPath& path)
{
	for (Device1* device : *this) {
		if (device->path() == path.path()) {
			return true;
		}
	}

	return false;
}

int DeviceListModel::rowCount(const QModelIndex& parent) const
{
	return size();
}

QVariant DeviceListModel::data(const QModelIndex& index, int role) const
{
	if (! index.isValid()) {
		return QVariant();
	}
	else if (index.row() >= size()) {
		return QVariant();
	}
	else if (role == Qt::DisplayRole) {
		return at(index.row())->name();
	}
	else if (role == Qt::DecorationRole) {
		return QIcon::fromTheme(at(index.row())->icon()); // FIXME fallback
	}
	else {
		return QVariant();
	}
}

