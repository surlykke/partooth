/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "mainwindow.h"

mainwindow::mainwindow() : proxies(0)
{
	widget.setupUi(this);
}

mainwindow::~mainwindow()
{
}

void mainwindow::show()
{
	proxies = new Proxies();
	connect(proxies, SIGNAL(deviceAdded(QString, Device1*)), SLOT(onDeviceAdded(QString, Device1*)));
	connect(proxies, SIGNAL(deviceRemoved(QString)), SLOT(onDeviceRemoved(QString)));
	connect(proxies, SIGNAL(deviceChanged(QString)), SLOT(onDeviceChanged(QString)));
	proxies->initialize();

	QMainWindow::show();

}

void mainwindow::onDeviceAdded(QString path, Device1* device)
{
	if (device->paired()) {
		widget.pairedDevicesList->addItem(device->name());
	}
	else {
		widget.visibleDevicesList->addItem(device->name());
	}
}

void mainwindow::onDeviceRemoved(QString path)
{
}

void mainwindow::onDeviceChanged(QString path)
{
}
