/* 
 * File:   DeviceWidget.h
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */

#ifndef _DEVICEWIDGET_H
#define	_DEVICEWIDGET_H

#include "ui_deviceframe.h"
class Device1;

class DeviceFrame : public QFrame
{
	Q_OBJECT
public:
	DeviceFrame(Device1 *device, QWidget* parent = 0);
	virtual ~DeviceFrame();
private:
	Ui::DeviceFrame frame;
};

#endif	/* _DEVICEWIDGET_H */
