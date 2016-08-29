#include "dialogshowupdatemsg.h"
#include "ui_dialogshowupdatemsg.h"

#include "inisettings.h"

DialogShowUpdateMsg::DialogShowUpdateMsg(QWidget *parent, const QString &caption, const QString &message) :
    QDialog(parent),
    ui(new Ui::DialogShowUpdateMsg)
{
    ui->setupUi(this);

    this->setWindowTitle(caption);
    ui->labelMsg->setText(message);
}

DialogShowUpdateMsg::~DialogShowUpdateMsg()
{
    delete ui;
}

void DialogShowUpdateMsg::setAutoUpdates(int arg1)
{
    bool enabled;

    if (arg1 == Qt::Checked)
        enabled = false;
    else
        enabled = true;

    IniSettings::setAutoChechUpdates(enabled);
}

void DialogShowUpdateMsg::on_pushOk_clicked()
{
    accept();
}

void DialogShowUpdateMsg::on_checkBoxDisable_stateChanged(int arg1)
{
    setAutoUpdates(arg1);
}

void DialogShowUpdateMsg::on_pushCancel_clicked()
{
    reject();
}
