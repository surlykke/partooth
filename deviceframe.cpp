/*
 * File:   DeviceWidget.cpp
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */
#include <QMouseEvent>

#include "deviceframe.h"
#include "device.h"
#include "serviceframe.h"

DeviceFrame::DeviceFrame(QString path, QWidget* parent) : 
	QFrame(parent),
	device(new Device(path))
{
	frame.setupUi(this);
	setStyleSheet("QLabel {background: rgb(0,0,0,0);}\n"
				  "QFrame:hover,QLabel:hover {background: rgb(210,210,210);}\n"
                  "QFrame[pressed=\"true\"] {background: rgb(150,150,180)}");

	frame.iconLabel->setPixmap(QIcon::fromTheme(device->icon()).pixmap(40,40));
	frame.nameLabel->setText(device->alias());
}

DeviceFrame::~DeviceFrame()
{
}

void DeviceFrame::mousePressEvent(QMouseEvent*)
{
	setProperty("pressed", true);
	style()->polish(this);
}


void DeviceFrame::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
	setProperty("pressed", false);
	style()->polish(this);
	if (device->paired()) {
		// Open dialog
	}
	else {
		connect();
	}
}

void DeviceFrame::connect()
{

}
