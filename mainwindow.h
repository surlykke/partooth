/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include <QDBusObjectPath>

#include "dbus_types.h"
#include "ui_mainwindow.h"

class Device;
class DeviceFrame;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

signals:
	void frameClicked(DeviceFrame* deviceFrame);

protected:
	virtual void paintEvent(QPaintEvent* paintEvent);

private slots:
	void onInterfacesAdded(const QDBusObjectPath& path, InterfaceMap interfaces);
	void onInterfacesRemoved(const QDBusObjectPath& path, const QStringList& interfaces);

private:
	void add(const QDBusObjectPath& objectPath);
	Ui::mainwindow widget;
	QMap<QString, DeviceFrame*> frames;	
};

#endif	/* _MAINWINDOW_H */
