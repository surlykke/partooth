/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_mainwindow.h"
#include "org.bluez.Adapter1.h"
#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "proxies.h"

class mainwindow : public QMainWindow
{
	Q_OBJECT
public:
	mainwindow();
	virtual ~mainwindow();
	virtual void show();

private slots:
	void onDeviceAdded(QString path, Device1 *device);
	void onDeviceRemoved(QString path);
	void onDeviceChanged(QString path);

private:
	Ui::mainwindow widget;
	Proxies* proxies;
};

#endif	/* _MAINWINDOW_H */
