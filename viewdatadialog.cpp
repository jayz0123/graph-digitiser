#include "viewdatadialog.h"
#include "ui_viewdatadialog.h"

ViewDataDialog::ViewDataDialog(const QVector<QPointF> &points, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewDataDialog),
    points(points)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);

    setDataText(points);
}

ViewDataDialog::~ViewDataDialog()
{
    delete ui;
}

void ViewDataDialog::setDataText(const QVector<QPointF> &points)
{
    QString text;

    for(int i = 0; i < points.size(); i++)
    {
       text.append(QString::number(points[i].x()));
       text.append(", ");
       text.append(QString::number(points[i].y()));
       text.append('\n');
    }

    ui->textEdit->setText(text);
}
