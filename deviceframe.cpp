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

DeviceFrame::DeviceFrame(Device* device, QWidget* parent) : 
	QFrame(parent),
	device(device),
	selected(false)
{
	frame.setupUi(this);
	connect(device->propertiesInterface,
	        SIGNAL(PropertiesChanged(const QString&, const QVariantMap&, const QStringList&)),
			SLOT(devicePropertiesChanged(const QString&, const QVariantMap&, const QStringList&)));
	connect(frame.pairButton, SIGNAL(clicked()), SLOT(onPairClicked()));

	frame.trustCheckBox->setText(tr("Trust %1").arg(device->alias()));
	frame.useServicesLabel->setText(tr("Use these services from %1").arg(device->alias()));
	frame.selectedFrame->hide();

	connect(device, SIGNAL(destroyed(QObject*)), SLOT(deleteLater()));
	update();
}

DeviceFrame::~DeviceFrame()
{
}

void DeviceFrame::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton) {
		emit clicked(this);
	}
}

void DeviceFrame::onDeviceFrameClicked(DeviceFrame* deviceFrame)
{
	if (deviceFrame != this) {
		selected = false;
	}
	else {
		selected = !selected;
	}

	frame.selectedFrame->setVisible(selected);
	setFrameStyle(selected ? QFrame::StyledPanel : QFrame::NoFrame);
}

void DeviceFrame::devicePropertiesChanged(const QString& interface, const QVariantMap& changed_properties, const QStringList& invalidated_properties)
{
	if (changed_properties.contains("Paired")) {
		emit pairingChanged(this, device->paired());
	}
	update();
}

void DeviceFrame::onPairClicked()
{
	device->Pair();
}


void DeviceFrame::update() {
	QIcon deviceIcon = QIcon::fromTheme(device->icon());
	if (deviceIcon.isNull()) {
		frame.iconLabel->setPixmap(QPixmap());
		frame.iconLabel->setText("?");
	}
	else {
		frame.iconLabel->setPixmap(deviceIcon.pixmap(QSize(40,40)));
	}
	
	frame.nameLabel->setText(device->name());

	for (ServiceFrame* serviceFrame : serviceFrames.values()) {
		serviceFrame->deleteLater();
	}
	serviceFrames.clear();

	for (QString uuid : device->uUIDs()) {
		ServiceFrame* serviceFrame = new ServiceFrame(uuid);
		frame.servicesFrame->layout()->addWidget(serviceFrame);
		serviceFrames[uuid] = serviceFrame;
	}
}
