#ifndef ADAPTER_H
#define ADAPTER_H

#include <QFrame>
#include "org.bluez.Adapter1.h"
#include "org.freedesktop.DBus.Properties.h"

namespace Ui {
class Adapter;

}

class QDBusPendingCallWatcher;

class Adapter : public QFrame
{
    Q_OBJECT

public:
    explicit Adapter(QString path, QWidget *parent = 0);
    ~Adapter();
    OrgBluezAdapter1Interface adapterInterface;

signals:
    void powered(bool isPowered);

private slots:
    void onPropertiesChanged(const QString &interface,
                             const QVariantMap &changed_properties,
                             const QStringList &invalidated_properties);

    void poweredClicked(bool checked);

private:
    Ui::Adapter *ui;
};

#endif // ADAPTER_H
