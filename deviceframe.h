/* 
 * File:   DeviceWidget.h
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */

#ifndef _DEVICEWIDGET_H
#define	_DEVICEWIDGET_H

#include "ui_deviceframe.h"
#include "org.bluez.Device1.h"
class Device;
class ServiceFrame;

class DeviceFrame : public QFrame
{
	Q_OBJECT
public:
	DeviceFrame(QString path, QWidget* parent = 0);
	virtual ~DeviceFrame();
	Device* device;

protected:
	virtual void mouseReleaseEvent(QMouseEvent* mouseEvent);
	virtual void mousePressEvent(QMouseEvent*);

private:
	void connect();	
	Ui::DeviceFrame frame;
};

#endif	/* _DEVICEWIDGET_H */
