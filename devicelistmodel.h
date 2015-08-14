/* 
 * File:   DeviceListModel.h
 * Author: christian
 *
 * Created on 14. august 2015, 19:56
 */

#ifndef DEVICELISTMODEL_H
#define	DEVICELISTMODEL_H

#include "org.bluez.Device1.h"


class DeviceListModel : public QAbstractListModel, public QList<Device1*>
{
public:
	DeviceListModel();
	virtual ~DeviceListModel();

	void addDevice(Device1* device);
	bool contains(const QDBusObjectPath& path);
	void removeAll(const QDBusObjectPath& path);

	virtual int rowCount(const QModelIndex& parent) const;
	virtual QVariant data(const QModelIndex& index, int role) const;

};

#endif	/* DEVICELISTMODEL_H */

