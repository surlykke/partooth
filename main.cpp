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

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow mw;
	mw.show();

	return app.exec();	
}
