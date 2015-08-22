/* 
 * File:   DeviceListModel.h
 * Author: christian
 *
 * Created on 14. august 2015, 19:56
 */

#ifndef DEVICELISTMODEL_H
#define	DEVICELISTMODEL_H

#include <QObject>
#include <QDBusObjectPath>

#include "dbus_types.h"

class Device;

class DeviceListModel : public QObject
{
	Q_OBJECT

public:
	DeviceListModel();
	virtual ~DeviceListModel();

	void initialize();

	void add(const QString& path);
	bool contains(const QString& path) const;
	void remove(const QString& path); 

signals:
	void deviceAdded(Device* device);

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	QMap<QString, Device*> devices;
};

#endif	/* DEVICELISTMODEL_H */

