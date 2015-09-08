/* 
 * File:   device.cpp
 * Author: christian
 * 
 * Created on 15. august 2015, 09:56
 */

#include <QWidget>
#include <QBoxLayout>
#include <QMessageBox>
#include <QDBusPendingCall>
#include <QBluetoothUuid>
#include <QBluetoothServiceInfo>

#include "device.h"
#include "constants.h"
#include "org.bluez.Adapter1.h"
	
const QMap<QString, QString> Device::uuid2ServiceName = {}; // FIXME

QString Device::serviceName(QString uuid)
{
	return uuid2ServiceName.value(uuid, uuid);
}

Device::Device(QString path, QWidget* parent) :
	QFrame(parent),
	deviceInterface(new OrgBluezDevice1Interface(BLUEZ_SERVICE, path, SYS_BUS, this)),
	propertiesInterface(new OrgFreedesktopDBusPropertiesInterface(BLUEZ_SERVICE, path, SYS_BUS, this)),
	pairing(false)
{
	frame.setupUi(this);
	frame.detailsFrame->hide();
	update();
	
	connect(frame.deviceButton, SIGNAL(clicked()), SIGNAL(clicked()));
	connect(frame.forgetButton, SIGNAL(clicked()), SLOT(forget()));
	connect(frame.pairButton, SIGNAL(clicked()), SLOT(pair()));
	connect(frame.trustedCheckBox, SIGNAL(stateChanged(int)), SLOT(trustedClicked(int)));
	connect(frame.connectedCheckBox, SIGNAL(stateChanged(int)), SLOT(connectedClicked(int)));

	connect(propertiesInterface,
		    SIGNAL(PropertiesChanged(const QString&, const QVariantMap&, const QStringList&)),
			SLOT(onPropertiesChanged(const QString&, const QVariantMap&, const QStringList&)));

}

Device::~Device()
{
}

void Device::showDetails(bool show) {
	if (show) {
		frame.detailsFrame->show();
		setStyleSheet(
			"QFrame#device {"
			"	border-style: outset;"
			"	border-width: 2px;"
			"	border-radius: 10px;"
			"	border-color: black;"
			"}"
		);
	}
	else {
		frame.detailsFrame->hide();
		setStyleSheet("#device {}");
	}
}

void Device::paintEvent(QPaintEvent* event)
{
	frame.noServicesLabel->setVisible(services.isEmpty());
	QFrame::paintEvent(event);
}



void Device::onPropertiesChanged(const QString& interface, const QVariantMap& changed_properties, const QStringList& invalidated_properties)
{
	update();
	if (changed_properties.contains("Paired") && changed_properties["Paired"].toBool()) {
		emit paired(deviceInterface->path());
	}
}

void Device::pair()
{
	if (QMessageBox::question(this, tr("Pair"), tr("Pair with %1?").arg(deviceInterface->alias())) == QMessageBox::Yes) {
		QDBusPendingReply<> reply = deviceInterface->Pair();
		QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(reply, this);
		connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)), SLOT(pairingFinished(QDBusPendingCallWatcher*)));  
		pairing = true;
		update();
	}	
}

void Device::pairingFinished(QDBusPendingCallWatcher* reply)
{
	pairing = false;
	if (reply->isError()) {
		qWarning() << reply->error().message();
	}
	else {
		deviceInterface->setTrusted(true);
		deviceInterface->Connect();
	}
	reply->deleteLater();
	update();
}


void Device::forget()
{
	if (QMessageBox::question(this, tr("Forget"), tr("Forget %1?").arg(deviceInterface->alias())) == QMessageBox::Yes) {
		QDBusPendingReply<> reply = OrgBluezAdapter1Interface(BLUEZ_SERVICE, deviceInterface->adapter().path(), SYS_BUS).RemoveDevice(QDBusObjectPath(deviceInterface->path()));
		if (reply.isError()) {
			qDebug() << reply.error().message();
		}
	}	
}

void Device::connectedClicked(int newState)
{
	if (newState == Qt::Unchecked) {
		deviceInterface->Disconnect();
	}
	else if (newState == Qt::Checked) {
		deviceInterface->Connect();
	}
}


void Device::trustedClicked(int newState)
{
	if (newState == Qt::Unchecked)	{
		deviceInterface->setTrusted(false);
	}
	else if (newState == Qt::Checked) {
		deviceInterface->setTrusted(true);
	} 
}


void Device::update()
{
	frame.iconLabel->setPixmap(QIcon::fromTheme(deviceInterface->icon()).pixmap(40, 40));
	frame.aliasLabel->setText(deviceInterface->alias());
	frame.trustedCheckBox->setChecked(deviceInterface->trusted());
	frame.trustedCheckBox->setVisible(deviceInterface->paired());
	frame.connectedCheckBox->setChecked(deviceInterface->connected());
	frame.connectedCheckBox->setVisible(deviceInterface->paired());
	frame.forgetFrame->setVisible(deviceInterface->paired());
	frame.pairFrame->setVisible(!pairing && !deviceInterface->paired());
	frame.pairingFrame->setVisible(pairing && !deviceInterface->paired());

	// Adjust list of services brute force
	while (! services.isEmpty()) {
		services.takeFirst()->deleteLater();
	}

    for (QString uuid : deviceInterface->uUIDs()) {
        QBluetoothUuid bluetoothUuid(uuid);
        quint32 uuidAsNumber = bluetoothUuid.toUInt32();
        if (uuidAsNumber > 0)  {
            QBluetoothUuid::ServiceClassUuid scUuid =
                    static_cast<QBluetoothUuid::ServiceClassUuid>(uuidAsNumber);
            services.append(new QLabel(QBluetoothUuid::serviceClassToString(scUuid)));
            frame.servicesFrame->layout()->addWidget(services.last());
        }
    }
}
