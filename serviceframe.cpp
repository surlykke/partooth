/*
 * File:   ServiceFrame.cpp
 * Author: christian
 *
 * Created on 22. august 2015, 10:23
 */

#include "serviceframe.h"

ServiceFrame::ServiceFrame(QByteArray shortFormUuid) {
	widget.setupUi(this);
	widget.serviceNameLabel->setText(shortFormUuid);
}

ServiceFrame::~ServiceFrame() {}

