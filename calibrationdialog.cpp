#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(p_scene);
    ui->graphicsView->setCursor(Qt::CrossCursor);

    ui->lineEdit_x_point_1->setValidator(new QDoubleValidator(0,1000,3,this));
    ui->lineEdit_x_point_2->setValidator(new QDoubleValidator(0,1000,3,this));
    ui->lineEdit_y_point_1->setValidator(new QDoubleValidator(0,1000,3,this));
    ui->lineEdit_y_point_2->setValidator(new QDoubleValidator(0,1000,3,this));

    ui->clearButton->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->checkBox_x->setEnabled(false);
    ui->checkBox_y->setEnabled(false);
    ui->groupBox_x->setEnabled(false);
    ui->groupBox_y->setEnabled(false);
    ui->label_point_1->setEnabled(false);
    ui->label_point_2->setEnabled(false);
    ui->label_point_3->setEnabled(false);
    ui->label_point_4->setEnabled(false);
    ui->lineEdit_x_point_1->setEnabled(false);
    ui->lineEdit_x_point_2->setEnabled(false);
    ui->lineEdit_y_point_1->setEnabled(false);
    ui->lineEdit_y_point_2->setEnabled(false);

    tip.setText("Please click on points with the following order");
    tip.setStandardButtons(QMessageBox::Ok);
    tip.setIconPixmap(QPixmap(":/img/img/tip.png"));
    tip.setDefaultButton(QMessageBox::Ok);
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;

    delete p_scene;
    delete p_map;

    delete p_x_point_1;
    delete p_x_point_2;
    delete p_y_point_1;
    delete p_y_point_2;
}

void CalibrationDialog::mousePressEvent(QMouseEvent *event)
{
    QPoint remapped = ui->graphicsView->mapFromParent(event->pos());

    if (ui->graphicsView->rect().contains(remapped) && event->button() == Qt::LeftButton)
    {
         QPointF mousePoint = ui->graphicsView->mapToScene(remapped);

         if(isCalibrating)
         {
             double radius = 4;

             p_scene->addEllipse(mousePoint.x()-radius,
                                 mousePoint.y()-radius,
                                 radius*2.0, radius*2.0,
             QPen(Qt::red), QBrush(Qt::red, Qt::SolidPattern));

             calibrationVector.push_back(mousePoint);

             calibrationCheck();
         }
    }
}

void CalibrationDialog::on_CalibrationDialog_accepted()
{
    double x_length, y_length;

    p_x_point_1 = new QString(ui->lineEdit_x_point_1->text());
    p_x_point_2 = new QString(ui->lineEdit_x_point_2->text());
    p_y_point_1 = new QString(ui->lineEdit_y_point_1->text());
    p_y_point_2 = new QString(ui->lineEdit_y_point_2->text());

    x_point_1_num = p_x_point_1->toDouble();
    x_point_2_num = p_x_point_2->toDouble();
    y_point_1_num = p_y_point_1->toDouble();
    y_point_2_num = p_y_point_2->toDouble();

    x_length = x_point_2_num - x_point_1_num;
    y_length = y_point_2_num - y_point_1_num;

    // calculate the x-y calibration factors unit: real_length/pixel
    x_calibration_factor = x_length / (calibrationVector[1].x() - calibrationVector[0].x());
    y_calibration_factor = y_length / (calibrationVector[3].y() - calibrationVector[2].y());

    // calculate the offset of the origin
    x_offset = x_point_1_num - x_calibration_factor * calibrationVector[0].x();
    y_offset = y_point_1_num - y_calibration_factor * calibrationVector[2].y();

    isCalibrated = true;
}

void CalibrationDialog::calibrationCheck()
{
    if(calibrationVector.size() == 4)
    {
        isCalibrating = false;

        ui->groupBox_x->setEnabled(true);
        ui->groupBox_y->setEnabled(true);
        ui->label_point_1->setEnabled(true);
        ui->label_point_2->setEnabled(true);
        ui->label_point_3->setEnabled(true);
        ui->label_point_4->setEnabled(true);
        ui->lineEdit_x_point_1->setEnabled(true);
        ui->lineEdit_x_point_2->setEnabled(true);
        ui->lineEdit_y_point_1->setEnabled(true);
        ui->lineEdit_y_point_2->setEnabled(true);
    }
}



void CalibrationDialog::on_lineEdit_x_point_1_textEdited(const QString &text_x_1)
{
    if(!text_x_1.isEmpty())
        hasTextX1 = true;
    else
        hasTextX1 = false;

    if(!text_x_1.isEmpty() && hasTextX2)
        ui->checkBox_x->setEnabled(true);
    else
        ui->checkBox_x->setEnabled(false);

    if(!text_x_1.isEmpty() && hasTextX2 && hasTextY1 && hasTextY2
        && text_x_1 != ui->lineEdit_x_point_2->text()
        && ui->lineEdit_y_point_1->text() != ui->lineEdit_y_point_2->text())
        ui->buttonBox->setEnabled(true);
    else
        ui->buttonBox->setEnabled(false);
}

void CalibrationDialog::on_lineEdit_x_point_2_textEdited(const QString &text_x_2)
{
    if(!text_x_2.isEmpty())
        hasTextX2 = true;
    else
        hasTextX2 = false;

    if(!text_x_2.isEmpty() && hasTextX1)
        ui->checkBox_x->setEnabled(true);
    else
        ui->checkBox_x->setEnabled(false);

    if(!text_x_2.isEmpty() && hasTextX2 && hasTextY1 && hasTextY2
        && text_x_2 != ui->lineEdit_x_point_1->text()
        && ui->lineEdit_y_point_1->text() != ui->lineEdit_y_point_2->text())
        ui->buttonBox->setEnabled(true);
    else
        ui->buttonBox->setEnabled(false);
}


void CalibrationDialog::on_lineEdit_y_point_1_textEdited(const QString &text_y_1)
{
    if(!text_y_1.isEmpty())
        hasTextY1 = true;
    else
        hasTextY1 = false;

    if(!text_y_1.isEmpty() && hasTextY2)
        ui->checkBox_y->setEnabled(true);
    else
        ui->checkBox_y->setEnabled(false);

    if(!text_y_1.isEmpty() && hasTextX2 && hasTextY1 && hasTextY2
        && text_y_1 != ui->lineEdit_y_point_2->text()
        && ui->lineEdit_x_point_1->text() != ui->lineEdit_x_point_2->text())
        ui->buttonBox->setEnabled(true);
    else
        ui->buttonBox->setEnabled(false);
}

void CalibrationDialog::on_lineEdit_y_point_2_textEdited(const QString &text_y_2)
{
    if(!text_y_2.isEmpty())
        hasTextY2 = true;
    else
        hasTextY2 = false;

    if(!text_y_2.isEmpty() && hasTextY1)
        ui->checkBox_y->setEnabled(true);
    else
        ui->checkBox_y->setEnabled(false);

    if(!text_y_2.isEmpty() && hasTextX2 && hasTextY1 && hasTextY2
        && text_y_2 != ui->lineEdit_y_point_1->text()
        && ui->lineEdit_x_point_1->text() != ui->lineEdit_x_point_2->text())
        ui->buttonBox->setEnabled(true);
    else
        ui->buttonBox->setEnabled(false);
}

void CalibrationDialog::on_checkBox_x_stateChanged(int arg1)
{
    if(arg1 == 0)
        isXLog = false;
    else
        isXLog = true;
}

void CalibrationDialog::on_checkBox_y_stateChanged(int arg1)
{
    if(arg1 == 0)
        isYLog = false;
    else
        isYLog = true;
}

void CalibrationDialog::on_importImageButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Import Graph"),"*.png",tr("Images (*.png *.jpg)"));

    if(!filename.isNull())
    {
        isCalibrating = true;
        calibrationVector.clear();

        p_map = new QPixmap(filename);

        p_scene->clear();
        p_scene->addPixmap(*p_map);

        ui->clearButton->setEnabled(true);

        _disableWidgets();

        tip.exec();
    }
}

void CalibrationDialog::on_helpButton_clicked()
{
    tip.exec();
}

void CalibrationDialog::on_clearButton_clicked()
{
    isCalibrating = true;
    hasTextX1 = false;
    hasTextX2 = false;
    hasTextY1 = false;
    hasTextY2 = false;

    calibrationVector.clear();

    p_scene->clear();
    p_scene->addPixmap(*p_map);

    ui->lineEdit_x_point_1->clear();
    ui->lineEdit_x_point_2->clear();
    ui->lineEdit_y_point_1->clear();
    ui->lineEdit_y_point_2->clear();

    ui->checkBox_x->setCheckState(Qt::Unchecked);
    ui->checkBox_y->setCheckState(Qt::Unchecked);

    _disableWidgets();
}

void CalibrationDialog::_disableWidgets()
{
    ui->buttonBox->setEnabled(false);
    ui->checkBox_x->setEnabled(false);
    ui->checkBox_y->setEnabled(false);
    ui->groupBox_x->setEnabled(false);
    ui->groupBox_y->setEnabled(false);
    ui->label_point_1->setEnabled(false);
    ui->label_point_2->setEnabled(false);
    ui->label_point_3->setEnabled(false);
    ui->label_point_4->setEnabled(false);
    ui->lineEdit_x_point_1->setEnabled(false);
    ui->lineEdit_x_point_2->setEnabled(false);
    ui->lineEdit_y_point_1->setEnabled(false);
    ui->lineEdit_y_point_2->setEnabled(false);
}
