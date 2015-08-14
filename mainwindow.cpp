/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include <qt/QtWidgets/qmessagebox.h>

#include "mainwindow.h"

MainWindow::MainWindow()
{
	widget.setupUi(this);
	DeviceListModel* pairedDevices = new DeviceListModel();
	DeviceListModel* otherDevices = new DeviceListModel();
	Controller* controller = new Controller(pairedDevices, otherDevices);
	widget.pairedList->setModel(pairedDevices);
	widget.otherList->setModel(otherDevices);


	connect(widget.pairedList, 
		    SIGNAL(doubleClicked(const QModelIndex&)), 
		    SLOT(onPairedDoubleClicked(const QModelIndex&)));

	connect(widget.otherList,
		    SIGNAL(doubleClicked(const QModelIndex&)), 
		    SLOT(onOtherDoubleClicked(const QModelIndex&)));


	connect(this, SIGNAL(forgetDevice(int)), controller, SLOT(forgetDevice(int)));
	connect(this, SIGNAL(pairDevice(int)), controller, SLOT(pairDevice(int)));

	controller->initialize();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onPairedDoubleClicked(const QModelIndex& index)
{
	if (!index.isValid()) {
		return; // necessary ?
	}

	QString title = tr("Forget device");
	QString text = tr("Forget %1?").arg(index.data(Qt::DisplayRole).toString());
	if (QMessageBox::question(this, title, text) == QMessageBox::Yes) {
		emit forgetDevice(index.row());
	}
}

void MainWindow::onOtherDoubleClicked(const QModelIndex& index)
{
	if (!index.isValid()) {
		return;
	}

	emit pairDevice(index.row());
}
