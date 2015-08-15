/* 
 * File:   adapter.h
 * Author: christian
 *
 * Created on 15. august 2015, 10:01
 */

#ifndef ADAPTER_H
#define	ADAPTER_H

#include "org.bluez.Adapter1.h"
#include "org.freedesktop.DBus.Properties.h"


class Adapter : public QObject
{
	Q_OBJECT

public:
	explicit Adapter(QString path, QObject* parent = 0);
	virtual ~Adapter();
    OrgBluezAdapter1Interface adapterInterface;
	OrgFreedesktopDBusPropertiesInterface propertiesInterface;
};

#endif	/* ADAPTER_H */

