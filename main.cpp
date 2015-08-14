/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#include <QDebug>
#include <QApplication>
#include <QDBusConnection>

#include "mainwindow.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "controller.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	DeviceListModel pairedDevices;
	DeviceListModel otherDevices;

	Controller controller(&pairedDevices, &otherDevices);
	controller.initialize();

	MainWindow mw(&pairedDevices, &otherDevices);
	mw.show();

	return app.exec();	
}
