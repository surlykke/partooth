/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "mainwindow.h"
#include "device.h"
#include "constants.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "adapter.h"

MainWindow::MainWindow()
{
    widget.setupUi(this);
    adaptersLayout = dynamic_cast<QVBoxLayout*>(widget.adaptersFrame->layout());
    knownDevicesLayout = dynamic_cast<QVBoxLayout*>(widget.knownDevicesFrame->layout());
    otherDevicesLayout = dynamic_cast<QVBoxLayout*>(widget.otherDevicesFrame->layout());

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
    for (QDBusObjectPath objectPath: objectMap.keys()) {
        InterfaceMap interfaceMap = objectMap[objectPath];
        if (interfaceMap.contains(ADAPTER1_IF) &&
            interfaceMap.contains(PROPS_IF) &&
            !adapters.contains(objectPath.path()))
        {
            QString path = objectPath.path();
            adapters[path] = new Adapter(path);
            adapters[path]->initialize();
            adaptersLayout->addWidget(adapters[path]);
        }
	}

	// Find all devices and add them
	for (QDBusObjectPath path : objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(DEVICE1_IF)) {
			bool paired = interfaceMap[DEVICE1_IF]["Paired"].toBool();
			add(path.path(), paired);
		}
	}

}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent* paintEvent) {
    widget.noAdaptersLabel->setVisible(adaptersLayout->count() <= 1);
    widget.noKnownLabel->setVisible(knownDevicesLayout->count() <= 1);
    widget.noOtherLabel->setVisible(otherDevicesLayout->count() <= 1);
	QMainWindow::paintEvent(paintEvent);
}


void MainWindow::onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		bool paired = interfaces[DEVICE1_IF]["Paired"].toBool();
		add(path.path(), paired);	
	}
}

void MainWindow::onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces)
{
	if (interfaces.contains(DEVICE1_IF)) {
		remove(path.path());	
	}
}

void MainWindow::onDeviceClicked()
{
	Device* clickedDevice = dynamic_cast<Device*>(sender());
	for (Device* device : devices.values()) {
		if (device == clickedDevice) {
			device->showDetails(! device->shown());
		}
		else {
			device->showDetails(false);
		}
	}
}

void MainWindow::onDevicePaired(QString path)
{
	if (devices.contains(path)) {
		Device* device = devices[path];
        otherDevicesLayout->removeWidget(device);
        knownDevicesLayout->addWidget(device);
	}
}


void MainWindow::add(const QString& path, bool paired)
{
	remove(path);

	devices[path] = new Device(path, this);
	connect(devices[path], SIGNAL(clicked()), SLOT(onDeviceClicked()));
	connect(devices[path], SIGNAL(paired(QString)), SLOT(onDevicePaired(QString)));
    if (paired) {
        knownDevicesLayout->addWidget(devices[path]);
    }
    else {
        otherDevicesLayout->addWidget(devices[path]);
    }
}

void MainWindow::remove(const QString& path)
{
	if (devices.contains(path)) {
		Device* device = devices.take(path);
		qDebug() << "Deleting:" << path;
		device->deleteLater();
	}
	
}
