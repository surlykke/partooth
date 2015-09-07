#include <QDBusPendingCallWatcher>
#include <QDBusError>
#include "adapter.h"
#include "constants.h"
#include "ui_adapter.h"

Adapter::Adapter(QString path, QWidget *parent) :
    QFrame(parent),
    adapterInterface(BLUEZ_SERVICE, path, SYS_BUS, this),
    ui(new Ui::Adapter)
{
    ui->setupUi(this);
    connect(ui->poweredCheckBox, SIGNAL(clicked(bool)), SLOT(poweredClicked(bool)));
    connect(new OrgFreedesktopDBusPropertiesInterface(BLUEZ_SERVICE, path, SYS_BUS, this),
            SIGNAL(PropertiesChanged(QString,QVariantMap,QStringList)),
            SLOT(onPropertiesChanged(QString,QVariantMap,QStringList)));

    poweredClicked(true);
    adapterInterface.StartDiscovery();


    ui->nameLabel->setText(adapterInterface.alias());
    ui->poweredCheckBox->setChecked(adapterInterface.powered());

}

Adapter::~Adapter()
{
    delete ui;
}

void Adapter::onPropertiesChanged(const QString &interface, const QVariantMap &changed_properties, const QStringList &invalidated_properties)
{
    qDebug() << "Changed properties:" << changed_properties;
    if (changed_properties.contains("Powered")) {
        bool powerIsOn =  changed_properties["Powered"].toBool();
        emit powered(powerIsOn);
        ui->poweredCheckBox->setChecked(powerIsOn);
        if (powerIsOn) {
            adapterInterface.StartDiscovery();
        }
    }
}

void Adapter::poweredClicked(bool checked)
{
    adapterInterface.setPowered(checked);
    ui->poweredCheckBox->setCheckState(Qt::PartiallyChecked);
}

