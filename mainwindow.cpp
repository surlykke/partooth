/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "devicelistmodel.h"
#include "mainwindow.h"
#include "devicedialog.h"
#include "deviceframe.h"

MainWindow::MainWindow()
{
	widget.setupUi(this);
	DeviceListModel* deviceListModel = new DeviceListModel();
	connect(deviceListModel, SIGNAL(deviceAdded(Device*)), SLOT(onDeviceAdded(Device*)));
	deviceListModel->initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent* paintEvent) {
	int knownDevices = widget.knownDevicesFrame->layout()->count() - 1;
	int otherDevices = widget.otherDevicesFrame->layout()->count() - 1;	
	widget.noKnownLabel->setVisible(knownDevices <= 0);
	widget.noOtherLabel->setVisible(otherDevices <= 0);
	QMainWindow::paintEvent(paintEvent);
}


void MainWindow::onDeviceAdded(Device* device)
{
	DeviceFrame* frame = new DeviceFrame(device, widget.knownDevicesFrame);
	connect(frame, SIGNAL(pairingChanged(DeviceFrame*, bool)), SLOT(onPairingChanged(DeviceFrame*, bool)));
	connect(frame, SIGNAL(clicked(DeviceFrame*)), SIGNAL(frameClicked(DeviceFrame*)));
	connect(this, SIGNAL(frameClicked(DeviceFrame*)), frame, SLOT(onDeviceFrameClicked(DeviceFrame*)));
	if (device->paired()) {
		widget.knownDevicesFrame->layout()->addWidget(frame);
	}
	else {
		widget.otherDevicesFrame->layout()->addWidget(frame);
	}
}

void MainWindow::onPairingChanged(DeviceFrame* deviceFrame, bool paired)
{
	if(paired && widget.knownDevicesFrame->layout()->indexOf(deviceFrame) == -1) {
		widget.knownDevicesFrame->layout()->addWidget(deviceFrame);
	}
	else if (!paired && widget.otherDevicesFrame->layout()->indexOf(deviceFrame) == -1) {
		widget.otherDevicesFrame->layout()->addWidget(deviceFrame);
	}
}

