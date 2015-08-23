#ifndef _SERVICEFRAME_H
#define	_SERVICEFRAME_H

#include "ui_serviceframe.h"

class ServiceFrame : public QWidget
{
	Q_OBJECT
public:
	ServiceFrame(QString uuid);
	virtual ~ServiceFrame();

	const QString uuid;
private:
	Ui::ServiceFrame widget;

};

#endif	/* _SERVICEFRAME_H */
