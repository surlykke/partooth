/*
 * File:   devicedialog.cpp
 * Author: christian
 *
 * Created on 16. august 2015, 18:07
 */

#include <qt/QtWidgets/qmessagebox.h>

#include "devicedialog.h"
#include "device.h"
#include "adapter.h"

DeviceDialog::DeviceDialog(QString devicePath) : device(devicePath)
{
	widget.setupUi(this);
	connect(&device, SIGNAL(propertiesChanged(QString)), SLOT(update()));
	connect(widget.pairButton, SIGNAL(clicked()), SLOT(onPairButtonClicked()));
	connect(widget.forgetButton, SIGNAL(clicked()), SLOT(onForgetButtonClicked()));
	connect(widget.closeButton, SIGNAL(clicked()), SLOT(close()));
	update();
}

DeviceDialog::~DeviceDialog()
{
}

void DeviceDialog::update()
{
	widget.iconLabel->setPixmap(QIcon::fromTheme(device.icon()).pixmap(QSize(40,40)));
	widget.aliasLabel->setText(device.alias());
	if (device.paired()) {
		widget.pairButton->hide();
		widget.forgetButton->show();
	}
	else {
		widget.forgetButton->hide();
		widget.pairButton->show();
	}
}

void DeviceDialog::onForgetButtonClicked()
{
	QString title = tr("Forget");
	QString text = tr("Forget %1?").arg(device.alias());
	if (QMessageBox::question(this, title, text) == QMessageBox::Yes) {
		Adapter(device.adapter().path()).RemoveDevice(QDBusObjectPath(device.path()));
	}
}

void DeviceDialog::onPairButtonClicked()
{
	device.Pair();
}

