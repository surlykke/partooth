/* 
 * File:   DeviceListModel.h
 * Author: christian
 *
 * Created on 14. august 2015, 19:56
 */

#ifndef DEVICELISTMODEL_H
#define	DEVICELISTMODEL_H

#include "device.h"

#define PathRole 0x101

class DeviceListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	DeviceListModel();
	virtual ~DeviceListModel();

	void initialize();

	void add(const QString& path);
	bool contains(const QString& path) const;
	void remove(const QString& path); 
	int row(const QString& path) const;

	Device* device(const QString& path);	
	virtual int rowCount(const QModelIndex& parent) const;
	virtual QVariant data(const QModelIndex& index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex& index) const;


private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties);
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);


private:
	void removePaired(int index);
	void removeOther(int index);
	QVariant dataForHeading(QString heading, int role) const;
	QVariant dataForDevice(Device* device, int role) const;
	inline int rows() const { return 2 + pairedDevices.size() + otherDevices.size(); }
	QList<Device*> pairedDevices;
	QList<Device*> otherDevices;

};

#endif	/* DEVICELISTMODEL_H */

