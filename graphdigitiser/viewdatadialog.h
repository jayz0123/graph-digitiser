#ifndef VIEWDATADIALOG_H
#define VIEWDATADIALOG_H

#include <QDialog>
#include <QVector>
#include <QPointF>
#include <QString>
#include <QDebug>

namespace Ui {
class ViewDataDialog;
}

class ViewDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewDataDialog(const QVector<QPointF> &points, QWidget *parent = nullptr);
    ~ViewDataDialog();

    void setDataText(const QVector<QPointF> &points);

private:
    Ui::ViewDataDialog *ui;

    QVector<QPointF> points;
};

#endif // VIEWDATADIALOG_H
