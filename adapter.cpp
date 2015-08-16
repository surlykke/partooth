/* 
 * File:   adapter.cpp
 * Author: christian
 * 
 * Created on 15. august 2015, 10:01
 */

#include "adapter.h"
#include "org.bluez.Adapter1.h"
#include "constants.h"

Adapter::Adapter(QString path, QObject* parent) :	
	OrgBluezAdapter1Interface(BLUEZ_SERVICE, path, SYS_BUS, parent)
{
}

Adapter::~Adapter()
{
}

