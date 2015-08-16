/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include <QMessageBox>
#include "devicelistmodel.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
	widget.setupUi(this);
	DeviceListModel* deviceListModel = new DeviceListModel();
	widget.deviceList->setModel(deviceListModel);
	deviceListModel->initialize();

	connect(widget.deviceList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onDoubleClicked(const QModelIndex&)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onDoubleClicked(const QModelIndex& index)
{
	qDebug() << "MainWindow::onDoubleClicked";
	if (!index.isValid()) {
		return; // necessary ?
	}

	qDebug() << "Device" << index.data(PathRole) << "double clicked";
}
