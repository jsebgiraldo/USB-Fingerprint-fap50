#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    custom_status_bar_style();

    reader20 = new class Fap20Reader();

    reader50 = new class Fap50Reader();

    ui->DisconnectButton->setHidden(true);
    ui->stop_capture_btn->setHidden(true);

    connect(reader20, &Fap20Reader::sig_ImageReady, this, &MainWindow::onImageReadyFap20);

    connect(reader50, &Fap50Reader::sig_ImageReady, this, &MainWindow::onImageReadyFap50);
    connect(reader50, &Fap50Reader::sig_wronghand, this, &MainWindow::onWrongHandFap50);
    connect(reader50, &Fap50Reader::sig_samplingdone, this, &MainWindow::onSamplingDoneFap50);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::custom_status_bar_style()
{
    ui->statusbar->addWidget(status_label);

    QString style =    "font-size:18px;"
    "background-color: rgb(255, 255, 255);"
    "top: 1027px; \
    left: 72px; \
    width: 126px;   \
    height: 21px;   \
    /* UI Properties */ \
    letter-spacing: var(--unnamed-character-spacing-0); \
    color: var(--unnamed-color-616e85); \
    text-align: left;   \
    letter-spacing: 0px;    \
    color: #616E85; \
    opacity: 1;";

    ui->statusbar->setStyleSheet(style);
}

void MainWindow::onImageReadyFap20(Fingerprint Finger)
{
    ui->preview_enroll->setPixmap(Finger.pixmap.scaled(ui->preview_enroll->width(),ui->preview_enroll->height(),Qt::KeepAspectRatio));
    ui->score_label->setText(QString::number(Finger.Score));
}

void MainWindow::onImageReadyFap50(ImageStatus m_image)
{
    int img_w = 1600;
    int img_h = 1000;

    QImage image(m_image.img, img_w, img_h, QImage::Format_Grayscale8);
    ui->preview_enroll->setPixmap(QPixmap::fromImage(image).scaled(ui->preview_enroll->width(),ui->preview_enroll->height(),Qt::KeepAspectRatio));
    //ui->score_label->setText(QString::number(m_image.frame_rate));
}

void MainWindow::onWrongHandFap50()
{
    int res = QMessageBox::warning(this, "Wrong Hand", "You are using the wrong hand. \nDo you want to scan again?", QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::Yes)on_start_capture_clicked();
    else if(res == QMessageBox::No)ui->preview_enroll->clear();
}

void MainWindow::onSamplingDoneFap50()
{
    status_label->setText("Sampling Done!");

    ui->stop_capture_btn->setHidden(true);
    ui->start_capture->setHidden(false);
}

void MainWindow::deviceConnected_action()
{

    ui->radio_status_devices->setChecked(true);
    ui->radio_status_devices->setText("Connected");


    ui->ConnectButton->setHidden(true);
    ui->DisconnectButton->setHidden(false);

    ui->widget_options->setEnabled(true);
    ui->widget_13->setEnabled(true);

    status_label->setText("Welcome! Device connected");

}

void MainWindow::on_DisconnectButton_clicked()
{
    ui->radio_status_devices->setChecked(false);

    ui->radio_status_devices->setText("Disconnected");

    ui->DisconnectButton->setHidden(true);
    ui->ConnectButton->setHidden(false);

    ui->widget_options->setEnabled(false);
    ui->widget_13->setEnabled(false);

    ui->device_combo_box->setCurrentIndex(0);
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_ConnectButton_clicked()
{
    if (!reader20->Detect() && !reader50->Detect())
    {
        QMessageBox::warning(this, "Device Error", "No devices found.", QMessageBox::Ok);
        return;
    }

    if(reader20->Connect())
    {
        deviceConnected_action();
        ui->device_combo_box->setCurrentIndex(1);
        return;
    }

    if(reader50->Connect())
    {
        deviceConnected_action();
        ui->device_combo_box->setCurrentIndex(2);
        return;
    }

    QMessageBox::warning(this, "Error", "Could not be connected.", QMessageBox::Ok);
}

void MainWindow::on_CaptureLiveModeButton_clicked()
{
    reader50->get_flat_finger();
}


void MainWindow::on_start_capture_clicked()
{



    reader50->get_flat_finger();

    ui->start_capture->setHidden(true);
    ui->stop_capture_btn->setHidden(false);

    status_label->setText("Please put left 4 fingers on the sensor and keep your fingers steady.");
}


void MainWindow::on_AutoCaptureModeButton_clicked()
{

}

