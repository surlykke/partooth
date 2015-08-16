/* 
 * File:   device.cpp
 * Author: christian
 * 
 * Created on 15. august 2015, 09:56
 */

#include "device.h"
#include "constants.h"

Device::Device(QString path, QObject* parent) :	
	OrgBluezDevice1Interface(BLUEZ_SERVICE, path, SYS_BUS),
	propertiesInterface(new OrgFreedesktopDBusPropertiesInterface(BLUEZ_SERVICE, path, SYS_BUS, this))
{
	connect(propertiesInterface,
		    SIGNAL(PropertiesChanged(const QString&, const QVariantMap&, const QStringList&)),
			SLOT(onPropertiesChanged()));
}

Device::~Device()
{
}

void Device::onPropertiesChanged()
{
	emit propertiesChanged(path());
}

