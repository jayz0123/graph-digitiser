#ifndef GRAPHDIGITISER_H
#define GRAPHDIGITISER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include "setscaledialog.h"
#include "calibrationdialog.h"
#include "viewdatadialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GraphDigitiser; }
QT_END_NAMESPACE

class GraphDigitiser : public QMainWindow
{
    Q_OBJECT

public:
    GraphDigitiser(QWidget *parent = nullptr);
    ~GraphDigitiser();

    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    QPointF pixelToReal(const QPointF &point);

    void constructMenus();
    void autoMeasuring();

private slots:

    void on_actionAdd_Calibration_triggered();

    void on_actionSet_Axes_Scales_triggered();

    void on_actionView_Data_triggered();

    void on_actionExport_Data_triggered();

    void on_checkBox_stateChanged(int arg1);

    void on_clearButton_clicked();

private:
    Ui::GraphDigitiser *ui;

    QGraphicsScene *p_scene = new QGraphicsScene(this);

    QMenu *p_menu = new QMenu(this);

    QPixmap *p_map = new QPixmap();

    QVector<QPointF> dataVector;
    QVector<QPointF> endPointsVector;

    double x_calibration_factor;
    double y_calibration_factor;
    double x_offset;
    double y_offset;

    bool isXLog = false;
    bool isYLog = false;

    bool isMeasuring = false;
    bool isAutoMode = false;
};
#endif // GRAPHDIGITISER_H
