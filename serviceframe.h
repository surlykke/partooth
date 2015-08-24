#ifndef _SERVICEFRAME_H
#define	_SERVICEFRAME_H

#include "ui_serviceframe.h"

class ServiceFrame : public QWidget
{
	Q_OBJECT
public:
	ServiceFrame(QByteArray shortFormFormUuid);
	virtual ~ServiceFrame();
	const QByteArray shortFormUuid;

private:
	Ui::ServiceFrame widget;

};

#endif	/* _SERVICEFRAME_H */
