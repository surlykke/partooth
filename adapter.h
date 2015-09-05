#ifndef ADAPTER_H
#define ADAPTER_H

#include <QFrame>
#include "org.bluez.Adapter1.h"

namespace Ui {
class Adapter;

}

class Adapter : public QFrame
{
    Q_OBJECT

public:

    explicit Adapter(QString path, QWidget *parent = 0);
    ~Adapter();

    void initialize();

private:
    OrgBluezAdapter1Interface adapterInterface;
    Ui::Adapter *ui;
};

#endif // ADAPTER_H
