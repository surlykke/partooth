#include "adapter.h"
#include "constants.h"
#include "ui_adapter.h"

Adapter::Adapter(QString path, QWidget *parent) :
    QFrame(parent),
    adapterInterface(BLUEZ_SERVICE, path, SYS_BUS, this),
    ui(new Ui::Adapter)
{
    ui->setupUi(this);
}

Adapter::~Adapter()
{
    delete ui;
}

void Adapter::initialize()
{
    adapterInterface.setPowered(true);
    if (adapterInterface.powered()) {
        adapterInterface.StartDiscovery();
    }
    else {
        qWarning() << "Unable to turn on adapter" << adapterInterface.name();
    }

    ui->nameLabel->setText(adapterInterface.alias());
    ui->poweredCheckBox->setChecked(adapterInterface.powered());
}
