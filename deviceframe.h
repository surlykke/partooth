/* 
 * File:   DeviceWidget.h
 * Author: christian
 *
 * Created on 9. august 2015, 18:33
 */

#ifndef _DEVICEWIDGET_H
#define	_DEVICEWIDGET_H

#include "ui_deviceframe.h"
class Device;

class DeviceFrame : public QFrame
{
	Q_OBJECT
public:
	DeviceFrame(Device *device, QWidget* parent = 0);
	virtual ~DeviceFrame();

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
	
	Ui::DeviceFrame frame;
};

#endif	/* _DEVICEWIDGET_H */
