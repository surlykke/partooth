/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "mainwindow.h"
#include "deviceframe.h"

mainwindow::mainwindow() : devices(this)
{
	widget.setupUi(this);

	connect(&devices, SIGNAL(deviceAdded(Device1*)), SLOT(onDeviceAdded(Device1*)));
	connect(&devices, SIGNAL(deviceAboutToBeRemoved(Device1*)), SLOT(onDeviceAboutToBeRemoved(Device1*)));
	devices.initialize();
}

mainwindow::~mainwindow()
{
}

void mainwindow::onDeviceAdded(Device1* device)
{
	deviceFrames[device] = new DeviceFrame(device, this);
	if (device->paired()) {
		widget.pairedDevicesGroupBox->layout()->addWidget(deviceFrames[device]);
	}
	else {
		widget.otherDevicesGroupBox->layout()->addWidget(deviceFrames[device]);
	}
}

void mainwindow::onDeviceAboutToBeRemoved(Device1* device)
{
	qDebug() << "onDeviceAboutToBeRemoved:" << device->name();
	if (deviceFrames.contains(device)) {	
		qDebug() << "Deleting";
		delete deviceFrames.take(device);
	}
	else {
		qDebug() << "Don't know this one";
	}
}