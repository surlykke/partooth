/* 
 * File:   device.h
 * Author: christian
 *
 * Created on 15. august 2015, 09:56
 */

#ifndef DEVICE_H
#define	DEVICE_H

#include <QObject>
#include <QFrame>

#include "org.bluez.Device1.h"
#include "org.freedesktop.DBus.Properties.h"
#include "ui_device.h"

class Device : public QFrame
{
	Q_OBJECT

public:
    static QString serviceName(QString uuid);
    static QMap<QString, Device*> devices;

	Device(QString path, QWidget* parent = 0);
	virtual ~Device();
	void showDetails(bool show);
	inline bool shown() { return frame.detailsFrame->isVisible(); }
    OrgBluezDevice1Interface* deviceInterface;

signals:
	void clicked();
	void paired(QString path);

protected:	
    virtual void paintEvent(QPaintEvent* event);


private slots:
    void onPropertiesChanged(const QString &interface,
                             const QVariantMap &changed_properties,
                             const QStringList &invalidated_properties);
    void pair();
    void pairingFinished(QDBusPendingCallWatcher* watcher);
    void forget();
    void trustedClicked(int newState);
    void connectedClicked(int newState);

private:
	static const QMap<QString, QString> uuid2ServiceName;

	void update();
	OrgFreedesktopDBusPropertiesInterface* propertiesInterface;

	Ui::device frame;
	QList<QLabel*> services;
	bool pairing;
};



#endif	/* DEVICE_H */

