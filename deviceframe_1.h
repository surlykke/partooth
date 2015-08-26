/* 
 * File:   DeviceWidget.h
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */

#ifndef _DEVICEWIDGET_H
#define	_DEVICEWIDGET_H

#include <Q>
#include "ui_deviceframe.h"
#include "org.bluez.Device1.h"
class Device;
class ServiceFrame;

class DeviceFrame : public QPu
{
	Q_OBJECT
public:
	DeviceFrame(QString path, QWidget* parent = 0);
	virtual ~DeviceFrame();
	
	bool paired();

protected:
	virtual void paintEvent(QPaintEvent*);

signals:
	void pairingChanged(DeviceFrame* deviceFrame, bool paired);
	void clicked(DeviceFrame* deviceFrame);
public slots:
	void onDeviceFrameClicked(DeviceFrame* deviceFrame);

protected:
	virtual void mouseReleaseEvent(QMouseEvent* mouseEvent);

private slots:
    void devicePropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties);
	void onPairClicked(); 
private:
	void update();

	Device* device;
	bool selected;
	QMap<QByteArray, ServiceFrame*> serviceFrames;

	QByteArray uuid2shortForm(QString uuid);

	Ui::DeviceFrame frame;
};

#endif	/* _DEVICEWIDGET_H */
