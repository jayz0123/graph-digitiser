#include "setscaledialog.h"
#include "ui_setscaledialog.h"

SetScaleDialog::SetScaleDialog(bool isXLog, bool isYLog, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetScaleDialog),
    isXLog(isXLog),
    isYLog(isYLog)
{
    ui->setupUi(this);

    ui->linearButton_x->setChecked(!isXLog);
    ui->logButton_x->setChecked(isXLog);
    ui->linearButton_y->setChecked(!isYLog);
    ui->logButton_y->setChecked(isYLog);
}

SetScaleDialog::~SetScaleDialog()
{
    delete ui;
}

void SetScaleDialog::on_SetScaleDialog_accepted()
{
    if(ui->linearButton_x->isChecked())
        isXLog = false;

    if(ui->logButton_x->isChecked())
        isXLog = true;

    if(ui->linearButton_y->isChecked())
        isYLog = false;

    if(ui->logButton_y->isChecked())
        isYLog = true;
}
