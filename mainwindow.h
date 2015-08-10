/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include <QMap>

#include "ui_mainwindow.h"
#include "org.bluez.Adapter1.h"
#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.ObjectManager.h"
#include "devices.h"
#include "deviceframe.h"

class mainwindow : public QMainWindow
{
	Q_OBJECT
public:
	mainwindow();
	virtual ~mainwindow();

private slots:
	void onDeviceAdded(Device1* device);
	void onDeviceAboutToBeRemoved(Device1* device);

private:
	Ui::mainwindow widget;
	Devices devices;
	QMap<Device1*, DeviceFrame*> deviceFrames;
};

#endif	/* _MAINWINDOW_H */
