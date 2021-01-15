#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QtMath>
#include <QPixmap>
#include <QFileDialog>
#include <QVector>
#include <QPointF>
#include <QMouseEvent>
#include <QDoubleValidator>
#include <QGraphicsEllipseItem>
#include <QMessageBox>

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(QWidget *parent = nullptr);
    ~CalibrationDialog();

    void mousePressEvent(QMouseEvent *event);

    void calibrationCheck();

    double getXCalibrationFactor() {return x_calibration_factor;}
    double getYCalibrationFactor() {return y_calibration_factor;}
    double getXOffset() {return x_offset;}
    double getYOffset() {return y_offset;}
    bool getIsXLog() {return isXLog;}
    bool getIsYLog() {return isYLog;}

    void setXCalibrationFactor(bool log) {isXLog = log;}
    void setYCalibrationFactor(bool log) {isYLog = log;}

    bool getIsCalibrated() {return isCalibrated;}

    QString getFileName() {return filename;}


private slots:

    void on_CalibrationDialog_accepted();

    void on_lineEdit_x_point_1_textEdited(const QString &text_x_1);

    void on_lineEdit_x_point_2_textEdited(const QString &text_x_2);

    void on_lineEdit_y_point_1_textEdited(const QString &text_y_1);

    void on_lineEdit_y_point_2_textEdited(const QString &text_y_2);

    void on_checkBox_x_stateChanged(int arg1);

    void on_checkBox_y_stateChanged(int arg1);

    void on_importImageButton_clicked();

    void on_helpButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::CalibrationDialog *ui;

    QGraphicsScene *p_scene = new QGraphicsScene(this);

    QPixmap *p_map = new QPixmap();

    QMessageBox tip;

    QVector<QPointF> calibrationVector;

    QString filename;

    QString *p_x_point_1 = nullptr;
    QString *p_x_point_2 = nullptr;
    QString *p_y_point_1 = nullptr;
    QString *p_y_point_2 = nullptr;

    bool isXLog = false;
    bool isYLog = false;
    bool isCalibrated = false;
    bool isCalibrating = false;

    bool hasTextX1 = false;
    bool hasTextX2 = false;
    bool hasTextY1 = false;
    bool hasTextY2 = false;

    double x_point_1_num;
    double x_point_2_num;
    double y_point_1_num;
    double y_point_2_num;

    double x_offset;
    double y_offset;
    double x_calibration_factor;
    double y_calibration_factor;

    void _disableWidgets();
};

#endif // CALIBRATIONDIALOG_H
