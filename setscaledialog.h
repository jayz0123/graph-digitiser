#ifndef SETSCALEDIALOG_H
#define SETSCALEDIALOG_H

#include <QDialog>

namespace Ui {
class SetScaleDialog;
}

class SetScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetScaleDialog(bool isXLog, bool isYLog, QWidget *parent = nullptr);
    ~SetScaleDialog();

    bool getIsXLog() {return isXLog;}
    bool getIsYLog() {return isYLog;}

private slots:

    void on_SetScaleDialog_accepted();

private:
    Ui::SetScaleDialog *ui;

    bool isXLog = false;
    bool isYLog = false;
};

#endif // SETSCALEDIALOG_H
