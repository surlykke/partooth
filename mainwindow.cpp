/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include <QMessageBox>
#include "devicelistmodel.h"
#include "controller.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
	widget.setupUi(this);
	DeviceListModel* pairedDevices = new DeviceListModel();
	DeviceListModel* otherDevices = new DeviceListModel();
	Controller* controller = new Controller(pairedDevices, otherDevices);
	widget.pairedList->setModel(pairedDevices);
	widget.otherList->setModel(otherDevices);

	connect(widget.pairedList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onDoubleClicked(const QModelIndex&)));
	connect(widget.otherList, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(onDoubleClicked(const QModelIndex&)));
	connect(this, SIGNAL(togglePairing(QString)), controller, SLOT(togglePairing(QString)));

	controller->initialize();
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

	if (sender() == widget.pairedList) {
		QString title = tr("Forget device");
		QString text = tr("Forget %1?").arg(index.data(Qt::DisplayRole).toString());
		if (QMessageBox::question(this, title, text) != QMessageBox::Yes) {
			return;	
		}
	}

	qDebug() << "Emit togglePairing " << index.data(PathRole).toString();
	emit togglePairing(index.data(PathRole).toString()); 	
}
