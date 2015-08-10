/*
 * File:   DeviceWidget.cpp
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */

#include "deviceframe.h"
#include "org.bluez.Device1.h"

DeviceFrame::DeviceFrame(Device1* device, QWidget* parent) : QFrame(parent)
{
	frame.setupUi(this);
	frame.nameLabel->setText(device->name());
	frame.iconLabel->setPixmap(QIcon::fromTheme(device->icon()).pixmap(QSize(60,60)));
}

DeviceFrame::~DeviceFrame()
{
}
