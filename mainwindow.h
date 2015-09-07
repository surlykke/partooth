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
#include "rfkill.h"
#include "ui_mainwindow.h"

class Device;
class Adapter;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

private slots:
    void updateErrorMessage();
    void onInterfacesAdded(const QDBusObjectPath& objectPath, InterfaceMap interfaces);
    void onInterfacesRemoved(const QDBusObjectPath& objectPath, const QStringList& interfaces);
	void onDeviceClicked();
	void onDevicePaired(QString path); 

private:
    void addDevice(const QString& path, bool paired);
    void removeDevice(const QString& path);

	Ui::mainwindow widget;
    QVBoxLayout* adaptersLayout;
    QVBoxLayout* knownDevicesLayout;
    QVBoxLayout* otherDevicesLayout;

    Rfkill rfkill;

    QMap<QString, Adapter*> adapters;
    QMap<QString, Device*> devices;
};

#endif	/* _MAINWINDOW_H */
