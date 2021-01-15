#include "graphdigitiser.h"
#include "ui_graphdigitiser.h"

GraphDigitiser::GraphDigitiser(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphDigitiser)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(p_scene);
    ui->graphicsView->setCursor(Qt::CrossCursor);

    ui->clearButton->setEnabled(false);
    ui->actionSet_Axes_Scales->setEnabled(false);
    ui->actionView_Data->setEnabled(false);
    ui->actionExport_Data->setEnabled(false);
    ui->checkBox->setEnabled(false);

    constructMenus();
}

GraphDigitiser::~GraphDigitiser()
{
    delete ui;

    delete p_scene;
    delete p_menu;
    delete p_map;
}

void GraphDigitiser::mousePressEvent(QMouseEvent *event)
{
    if(isMeasuring)
    {
        QPoint remapped = ui->graphicsView->mapFromParent(event->pos());

        if (ui->graphicsView->rect().contains(remapped) && event->button() == Qt::LeftButton)
        {
            QPointF mousePoint = ui->graphicsView->mapToScene(remapped);

            if(isAutoMode)
            {
                mousePoint.setY(mousePoint.y()-58);
                endPointsVector.push_back(mousePoint);


                if(endPointsVector.size() == 1)
                {
                    p_scene->addLine(mousePoint.x(),mousePoint.y(),
                                    p_map->width(),mousePoint.y(),
                                    QPen(Qt::red));

                    p_scene->addLine(mousePoint.x(),mousePoint.y(),
                                    mousePoint.x(),p_map->height(),
                                    QPen(Qt::red));
                }
                else
                {
                    p_scene->addLine(mousePoint.x(),mousePoint.y(),
                                    0,mousePoint.y(),
                                    QPen(Qt::red));

                    p_scene->addLine(mousePoint.x(),mousePoint.y(),
                                    mousePoint.x(),0,
                                    QPen(Qt::red));
                }

                if(endPointsVector.size() == 2)
                {
                    if(endPointsVector[0].x() < endPointsVector[1].x() && endPointsVector[0].y() < endPointsVector[1].y())
                    {
                        autoMeasuring();
                        QMessageBox::about(this,tr("Auto Mode"),tr("Data is generated successfully"));
                    }
                    else
                    {
                        QMessageBox::about(this,tr("Auto Mode"),tr("The selected range is invalid"));

                        ui->clearButton->click();
                    }
                }
            }
            else
            {
                mousePoint.setY(mousePoint.y()-58);
                dataVector.push_back(pixelToReal(mousePoint));

                double radius = 4;

                p_scene->addEllipse(mousePoint.x()-radius,
                                 mousePoint.y()-radius,
                                 radius*2.0, radius*2.0,
                QPen(Qt::red), QBrush(Qt::red, Qt::SolidPattern));
            }
        }
    }
}

void GraphDigitiser::autoMeasuring()
{
    isAutoMode = false;

    QImage image(p_map->toImage());

    double x_start = endPointsVector[0].x();
    double x_end = endPointsVector[1].x();
    double y_start = endPointsVector[0].y();
    double y_end = endPointsVector[1].y();

    for(int i = 0; i < x_end-x_start; i++)
    {
        for(int j = 0; j < y_end-y_start; j++)
        {
            double radius = 1;

            QColor color(image.pixelColor(x_start+i, y_start+j));

            if(color.rgb() <= 0xFF646464)
            {
                QPointF point(x_start+i, y_start+j);

                p_scene->addEllipse(point.x()-radius,
                                    point.y()-radius,
                                    radius*2.0, radius*2.0,
                QPen(Qt::red), QBrush(Qt::red, Qt::SolidPattern));

                dataVector.push_back(pixelToReal(point));

                break;
            }
        }
    }
}

void GraphDigitiser::contextMenuEvent(QContextMenuEvent *event)
{
    p_menu->popup(event->globalPos());
}

QPointF GraphDigitiser::pixelToReal(const QPointF &point)
{
    double x, y;

    if(isXLog)
        x = qPow(10, x_offset + point.x() * x_calibration_factor);
    else
        x = x_offset + point.x() * x_calibration_factor;

    if(isYLog)
        y = qPow(10, y_offset + point.y() * y_calibration_factor);
    else
        y = y_offset + point.y() * y_calibration_factor;

    QPointF real_point(x,y);

    return real_point;
}

void GraphDigitiser::constructMenus()
{
    p_menu->addAction(ui->actionSet_Axes_Scales);
}

void GraphDigitiser::on_actionAdd_Calibration_triggered()
{
    CalibrationDialog *p_calibration_dialog = new CalibrationDialog(this);
    p_calibration_dialog->exec();

    if(p_calibration_dialog->getIsCalibrated())
    {
        ui->clearButton->setEnabled(true);
        ui->actionSet_Axes_Scales->setEnabled(true);
        ui->actionView_Data->setEnabled(true);
        ui->actionExport_Data->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox->setCheckState(Qt::Unchecked);

        x_calibration_factor = p_calibration_dialog->getXCalibrationFactor();
        y_calibration_factor = p_calibration_dialog->getYCalibrationFactor();

        x_offset = p_calibration_dialog->getXOffset();
        y_offset = p_calibration_dialog->getYOffset();

        isXLog = p_calibration_dialog->getIsXLog();
        isYLog = p_calibration_dialog->getIsYLog();

        p_map = new QPixmap(p_calibration_dialog->getFileName());

        p_scene->clear();
        p_scene->addPixmap(*p_map);

        dataVector.clear();
        endPointsVector.clear();

        isMeasuring = true;
    }
}

void GraphDigitiser::on_actionSet_Axes_Scales_triggered()
{
    SetScaleDialog *p_set_scale_dialog = new SetScaleDialog(isXLog, isYLog);
    p_set_scale_dialog->exec();

    isXLog = p_set_scale_dialog->getIsXLog();
    isYLog = p_set_scale_dialog->getIsYLog();
}

void GraphDigitiser::on_actionView_Data_triggered()
{
    ViewDataDialog *p_view_data_dialog = new ViewDataDialog(dataVector);
    p_view_data_dialog->exec();
}

void GraphDigitiser::on_actionExport_Data_triggered()
{
    QString text;

    for(int i = 0; i < dataVector.size(); i++)
    {
       text.append(QString::number(dataVector[i].x()));
       text.append(", ");
       text.append(QString::number(dataVector[i].y()));
       text.append('\n');
    }


    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Data"),
        "untitled.csv",
        tr("Comma Seperated Values (*.csv);;Text File (*.txt);;All Files (*)"));

    if(filename.isEmpty())
        return;
    else
    {
        QFile file(filename);

        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QDataStream out(&file);
        out << text;
    }
}

void GraphDigitiser::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0)
        isAutoMode = false;
    else
    {
        isAutoMode = true;

        p_scene->clear();
        p_scene->addPixmap(*p_map);

        endPointsVector.clear();
        dataVector.clear();

        QMessageBox::about(this, tr("Auto Mode"),
                            tr("Set a range for auto measuring\nFirstly click on the upper-left corner of the graph\nSecondly click on the lower-right corner of the graph"));
    }
}

void GraphDigitiser::on_clearButton_clicked()
{
    dataVector.clear();
    endPointsVector.clear();

    p_scene->clear();
    p_scene->addPixmap(*p_map);

    ui->checkBox->setCheckState(Qt::Unchecked);
}
