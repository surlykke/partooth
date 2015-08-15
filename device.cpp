/* 
 * File:   device.cpp
 * Author: christian
 * 
 * Created on 15. august 2015, 09:56
 */

#include "device.h"
#include "constants.h"

Device::Device(QString path, QObject* parent) :	
    QObject(parent),
	deviceInterface(BLUEZ_SERVICE, path, SYS_BUS),
	propertiesInterface(BLUEZ_SERVICE, path, SYS_BUS),
	objectPath(path)
{
}

Device::~Device()
{
}

