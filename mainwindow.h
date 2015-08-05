/* 
 * File:   mainwindow.h
 * Author: christian
 *
 * Created on 4. august 2015, 21:45
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
