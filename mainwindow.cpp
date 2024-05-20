#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    reader20 = new class Fap20Reader();

    reader50 = new class Fap50Reader();

    ui->DisconnectButton->setHidden(true);

    connect(reader20, &Fap20Reader::sig_ImageReady, this, &MainWindow::onImageReady);
    //connect(reader50, &Fap50Reader::sig_ImageReady, this, &MainWindow::onImageReady);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onImageReady(Fingerprint Finger)
{
    //ui->preview_enroll->setPixmap(Image.scaled(ui->preview_enroll->width(),ui->preview_enroll->height(),Qt::KeepAspectRatio));
    ui->preview_enroll->setPixmap(Finger.pixmap.scaled(ui->preview_enroll->width(),ui->preview_enroll->height(),Qt::KeepAspectRatio));
    ui->score_label->setText(QString::number(Finger.Score));
}

void MainWindow::deviceConnected_action()
{
    ui->radio_status_devices->setChecked(true);
    ui->radio_status_devices->setText("Connected");



    ui->DisconnectButton->setHidden(false);
    ui->ConnectButton->setHidden(true);
    ui->widget_options->setEnabled(true);
    ui->widget_13->setEnabled(true);
}

void MainWindow::on_DisconnectButton_clicked()
{
    ui->radio_status_devices->setChecked(false);

    ui->radio_status_devices->setText("Disconnected");

    ui->DisconnectButton->setHidden(true);
    ui->ConnectButton->setHidden(false);

    ui->widget_options->setEnabled(false);
    ui->widget_13->setEnabled(false);
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_ConnectButton_clicked()
{
    if(ui->device_combo_box->currentIndex() == 0)
    {
        if (!reader20->Detect())
        {
            QMessageBox::warning(this, "Device Error", "No devices found.", QMessageBox::Ok);
            return;
        }

        if (!reader20->Connect())
        {
            QMessageBox::warning(this, "Error", "Could not be connected.", QMessageBox::Ok);
            return;
        }
    }
    else if(ui->device_combo_box->currentIndex() == 1)
    {
        if(!reader50->Detect())
        {
            QMessageBox::warning(this, "Error", "No devices found.", QMessageBox::Ok);
            return;
        }

        if(!reader50->Connect())
        {
            if(!reader50->Connect()){
                QMessageBox::warning(this, "Error", "Could not be connected.", QMessageBox::Ok);
                return;
            }
        }
    }


    deviceConnected_action();
}





void MainWindow::on_CaptureLiveModeButton_clicked()
{
    reader50->get_flat_finger();
}


void MainWindow::on_start_capture_clicked()
{
    reader20->Start(e_capture_modes::authenticationAuto,0);
}

