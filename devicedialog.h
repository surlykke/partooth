/* 
 * File:   devicedialog.h
 * Author: christian
 *
 * Created on 16. august 2015, 18:07
 */

#ifndef _DEVICEDIALOG_H
#define	_DEVICEDIALOG_H

#include "device.h"
#include "ui_devicedialog.h"

class DeviceDialog : public QDialog
{
	Q_OBJECT
public:
	DeviceDialog(QString devicePath);
	virtual ~DeviceDialog();

private slots:
	void update();
	void onPairButtonClicked();
	void onConnectButtonClicked();
	void onForgetButtonClicked();

private:
	Ui::devicedialog widget;
	Device device;
	bool paired;
};

#endif	/* _DEVICEDIALOG_H */
