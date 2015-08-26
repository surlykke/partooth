/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "mainwindow.h"
#include "deviceframe.h"
#include "device.h"
#include "constants.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "adapter.h"

MainWindow::MainWindow()
{
	widget.setupUi(this);

	qDBusRegisterMetaType<PropertyMap>();
	qDBusRegisterMetaType<InterfaceMap>();
	qDBusRegisterMetaType<ObjectMap>();
	qDBusRegisterMetaType<QStringList>();
		
	OrgFreedesktopDBusObjectManagerInterface* objectManager = 
		new OrgFreedesktopDBusObjectManagerInterface(BLUEZ_SERVICE, "/", SYS_BUS);

	qDebug() << "Connecting interfaces added";
	connect(objectManager, 
		    SIGNAL(InterfacesAdded(const QDBusObjectPath&, InterfaceMap)),
		    SLOT(onInterfacesAdded(const QDBusObjectPath&, InterfaceMap)));

	qDebug() << "Connecting interfaces removed";
	connect(objectManager, 
		    SIGNAL(InterfacesRemoved(const QDBusObjectPath&, const QStringList&)),
		    SLOT(onInterfacesRemoved(const QDBusObjectPath&, const QStringList&)));

		
	ObjectMap objectMap = objectManager->GetManagedObjects();

	// Find adapters, turn them on, make them scan
	for (QDBusObjectPath path: objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(ADAPTER1_IF) && interfaceMap.contains(PROPS_IF)) {
			Adapter adapter(path.path());
			adapter.setPowered(true);
			if (adapter.powered()) {
				adapter.StartDiscovery();
			}
			else {
				qWarning() << "Unable to turn on adapter" << adapter.name();
			}
		}
	}

	// Find all devices and add them
	for (QDBusObjectPath path : objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(DEVICE1_IF)) {
			add(path);
		}
	}

}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent* paintEvent) {
	int knownDevices = widget.knownDevicesBox->layout()->count() - 1;
	int otherDevices = widget.otherDevicesBox->layout()->count() - 1;	
	widget.noKnownLabel->setVisible(knownDevices <= 0);
	widget.noOtherLabel->setVisible(otherDevices <= 0);
	QMainWindow::paintEvent(paintEvent);
}


void MainWindow::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		add(path);	
	}
}

void MainWindow::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		remove(path);	
	}
}

void MainWindow::add(const QDBusObjectPath& objectPath)
{
	remove(objectPath);

	QString path = objectPath.path();

	DeviceFrame* frame = new DeviceFrame(path);
	if (frame->device->paired()) {
		knownDevices[path] = frame;
		widget.knownDevicesBox->layout()->addWidget(frame);
	}
	else {
		otherDevices[path] = frame;
		widget.otherDevicesBox->layout()->addWidget(frame);
	}
}

void MainWindow::remove(const QDBusObjectPath& objectPath)
{
	if (knownDevices.contains(objectPath.path())) {
		knownDevices.take(objectPath.path())->deleteLater();
	}
		
	if (otherDevices.contains(objectPath.path())) {
		otherDevices.take(objectPath.path())->deleteLater();
	}
}
