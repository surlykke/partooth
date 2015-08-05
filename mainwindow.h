/*
 * Copyright (c) Christian Surlykke, 2015
 *
 * This file is part of Partooth
 * It is distributed under the GPL 3 license.
 * Please refer to the LICENSE file for a copy of the license.
 */

#ifndef _MAINWINDOW_H
#define	_MAINWINDOW_H

#include "ui_mainwindow.h"

class mainwindow : public QMainWindow
{
	Q_OBJECT
public:
	mainwindow();
	virtual ~mainwindow();
private:
	Ui::mainwindow widget;
};

#endif	/* _MAINWINDOW_H */
