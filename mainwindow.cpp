/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include "mainwindow.h"

MainWindow::MainWindow(DeviceListModel* pairedDevices, DeviceListModel* otherDevices)
{
	widget.setupUi(this);
	widget.pairedList->setModel(pairedDevices);
	widget.otherList->setModel(otherDevices);
}

MainWindow::~MainWindow()
{
}

