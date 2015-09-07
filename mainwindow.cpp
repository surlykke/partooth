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

	connect(objectManager, 
		    SIGNAL(InterfacesAdded(const QDBusObjectPath&, InterfaceMap)),
		    SLOT(onInterfacesAdded(const QDBusObjectPath&, InterfaceMap)));

	connect(objectManager, 
		    SIGNAL(InterfacesRemoved(const QDBusObjectPath&, const QStringList&)),
		    SLOT(onInterfacesRemoved(const QDBusObjectPath&, const QStringList&)));

    connect(&rfkill, SIGNAL(changed()), SLOT(updateErrorMessage()));
    rfkill.startMonitoring();

	ObjectMap objectMap = objectManager->GetManagedObjects();

    // Find all adapters
    for (QDBusObjectPath objectPath: objectMap.keys()) {
        InterfaceMap interfaceMap = objectMap[objectPath];
        if (interfaceMap.contains(ADAPTER1_IF) &&
            interfaceMap.contains(PROPS_IF) &&
            !adapters.contains(objectPath.path()))
        {
            QString path = objectPath.path();
            adapters[path] = new Adapter(path);
            adaptersLayout->addWidget(adapters[path]);
        }
	}

	// Find all devices and add them
	for (QDBusObjectPath path : objectMap.keys()) {
		InterfaceMap interfaceMap = objectMap[path];
		if (interfaceMap.contains(DEVICE1_IF)) {
			bool paired = interfaceMap[DEVICE1_IF]["Paired"].toBool();
            addDevice(path.path(), paired);
		}
	}

}

MainWindow::~MainWindow()
{
}

void MainWindow::updateErrorMessage()
{
    if (adapters.isEmpty()) {
        widget.messageLabel->setText(tr("No bluetooth adapter found"));
        widget.messageLabel->show();
        widget.objectArea->hide();
    }
    else if (rfkill.hardBlocked) {
        widget.messageLabel->setText(tr("Bluetooth is hard-blocked.<br/>Please unblock it."));
        widget.messageLabel->show();
        widget.objectArea->hide();

    }
    else if (rfkill.softBlocked) {
        widget.messageLabel->setText(tr("Bluetooth is soft-blocked.<br/>Please unblock it."));
        widget.messageLabel->show();
        widget.objectArea->hide();
    }
    else {
        widget.messageLabel->hide();
        widget.objectArea->show();
    }
}


void MainWindow::onInterfacesAdded(const QDBusObjectPath& objectPath, InterfaceMap interfaces)
{
    qDebug() << "Interfaces added:" << objectPath.path() << "->" << interfaces.keys();
    QString path = objectPath.path();

    if (interfaces.contains(DEVICE1_IF) && !devices.contains(path)) {
		bool paired = interfaces[DEVICE1_IF]["Paired"].toBool();
        addDevice(path, paired);
    }

    if (interfaces.contains(ADAPTER1_IF) && !adapters.contains(path)) {
        Adapter* adapter = new Adapter(objectPath.path(), this);
        adapters[objectPath.path()] = adapter;
        adaptersLayout->addWidget(adapter);
        updateErrorMessage();
    }

}

void MainWindow::onInterfacesRemoved(const QDBusObjectPath& objectPath, const QStringList& interfaces)
{
    qDebug() << "Interfaces removed:" << objectPath.path() << interfaces;
    QString path = objectPath.path();

    if (interfaces.contains(DEVICE1_IF) && devices.contains(path)) {
        removeDevice(objectPath.path());
    }

    if (interfaces.contains(ADAPTER1_IF) && adapters.contains(path)) {
        Adapter* adapter = adapters.take(path);
        adaptersLayout->removeWidget(adapter);
        adapter->deleteLater();
        updateErrorMessage();
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


void MainWindow::addDevice(const QString& path, bool paired)
{
    removeDevice(path);

    Device* dev = new Device(path, this);
    Adapter* adapter = adapters[dev->deviceInterface->adapter().path()];
    dev->setEnabled(adapter->adapterInterface.powered());
    connect(adapter, SIGNAL(powered(bool)), dev, SLOT(setEnabled(bool)));
    connect(dev, SIGNAL(clicked()), SLOT(onDeviceClicked()));
    connect(dev, SIGNAL(paired(QString)), SLOT(onDevicePaired(QString)));
    devices[path] = dev;
    (dev->deviceInterface->paired() ? knownDevicesLayout : otherDevicesLayout)->
            addWidget(dev);

    widget.noKnownLabel->setVisible(knownDevicesLayout->count() <= 1);
    widget.noOtherLabel->setVisible(otherDevicesLayout->count() <= 1);
}

void MainWindow::removeDevice(const QString& path)
{
	if (devices.contains(path)) {
		Device* device = devices.take(path);
        knownDevicesLayout->removeWidget(device);
        otherDevicesLayout->removeWidget(device);
        device->deleteLater();
	}
	
    widget.noKnownLabel->setVisible(knownDevicesLayout->count() <= 1);
    widget.noOtherLabel->setVisible(otherDevicesLayout->count() <= 1);
}
