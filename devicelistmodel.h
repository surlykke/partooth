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

	void add(const QString& path);
	bool contains(const QString& path);
	void remove(const QString& path);

	Device* device(const QString& path);	
	Device* deviceAt(int row);
	virtual int rowCount(const QModelIndex& parent) const;
	virtual QVariant data(const QModelIndex& index, int role) const;

signals:
	void pairingChanged(QString path, bool paired);

private slots:
    void onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties);

private:
	void removeAt(int row);
	QList<Device*> devices;

};

#endif	/* DEVICELISTMODEL_H */

