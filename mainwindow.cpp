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
    connect(reader50, &Fap50Reader::sig_cancelsampling, this, &MainWindow::onCancelSamplingFap50);

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
    ui->preview_capture->setPixmap(QPixmap::fromImage(image).scaled(ui->preview_capture->width(),ui->preview_capture->height(),Qt::KeepAspectRatio));
}

void MainWindow::onWrongHandFap50()
{
    int res = QMessageBox::warning(this, "Wrong Hand", "You are using the wrong hand. \nDo you want to scan again?", QMessageBox::Yes | QMessageBox::No);
    if(res == QMessageBox::Yes)on_start_capture_clicked();
    else if(res == QMessageBox::No)ui->preview_enroll->clear();
}

void MainWindow::onCancelSamplingFap50()
{
    status_label->setText("Cancel Sampling!.");
}

void MainWindow::onSamplingDoneFap50(ImageProperty res)
{
    if(res.pos == FINGER_POSITION_RIGHT_FOUR)
    {
        QImage image(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->right_four_fingers_label->setPixmap((QPixmap::fromImage(image).scaled(ui->right_four_fingers_label->width(),ui->right_four_fingers_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_RIGHT_INDEX;
        get_image(res);
        QImage r_index(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->r_index_label->setPixmap((QPixmap::fromImage(r_index).scaled(ui->r_index_label->width(),ui->r_index_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_RIGHT_MIDDLE;
        get_image(res);
        QImage r_middle(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->r_middle_label->setPixmap((QPixmap::fromImage(r_middle).scaled(ui->r_middle_label->width(),ui->r_middle_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_RIGHT_RING;
        get_image(res);
        QImage r_ring(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->r_ring_label->setPixmap((QPixmap::fromImage(r_ring).scaled(ui->r_ring_label->width(),ui->r_ring_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_RIGHT_LITTLE;
        get_image(res);
        QImage r_little(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->r_little_label->setPixmap((QPixmap::fromImage(r_little).scaled(ui->r_little_label->width(),ui->r_little_label->height(),Qt::KeepAspectRatio)));

        status_label->setText("Please put left 4 fingers on the sensor and keep your fingers steady.");
    }

    else if(res.pos == FINGER_POSITION_LEFT_FOUR)
    {
        QImage image(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->left_four_fingers_label->setPixmap((QPixmap::fromImage(image).scaled(ui->left_four_fingers_label->width(),ui->left_four_fingers_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_LEFT_INDEX;
        get_image(res);
        QImage l_index(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->l_index_label->setPixmap((QPixmap::fromImage(l_index).scaled(ui->l_index_label->width(),ui->l_index_label->height(),Qt::KeepAspectRatio)));


        res.pos = FINGER_POSITION_LEFT_MIDDLE;
        get_image(res);
        QImage l_middle(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->l_middle_label->setPixmap((QPixmap::fromImage(l_middle).scaled(ui->l_middle_label->width(),ui->l_middle_label->height(),Qt::KeepAspectRatio)));


        res.pos = FINGER_POSITION_LEFT_RING;
        get_image(res);
        QImage l_ring(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->l_ring_label->setPixmap((QPixmap::fromImage(l_ring).scaled(ui->l_ring_label->width(),ui->l_ring_label->height(),Qt::KeepAspectRatio)));


        res.pos = FINGER_POSITION_LEFT_LITTLE;
        get_image(res);
        QImage l_little(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->l_little_label->setPixmap((QPixmap::fromImage(l_little).scaled(ui->l_little_label->width(),ui->l_little_label->height(),Qt::KeepAspectRatio)));


        status_label->setText("Please put both thumbs on the sensor and keep your fingers steady.");
    }


    else if(res.pos == FINGER_POSITION_BOTH_THUMBS) {

        QImage image(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->bouth_thumb_fingers->setPixmap((QPixmap::fromImage(image).scaled(ui->bouth_thumb_fingers->width(),ui->bouth_thumb_fingers->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_RIGHT_THUMB;
        get_image(res);
        QImage r_thumb(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->r_thumb_label->setPixmap((QPixmap::fromImage(r_thumb).scaled(ui->r_thumb_label->width(),ui->r_thumb_label->height(),Qt::KeepAspectRatio)));

        res.pos = FINGER_POSITION_LEFT_INDEX;
        get_image(res);
        QImage l_thumb(res.img, res.width, res.height, QImage::Format_Grayscale8);
        ui->l_thumb_label->setPixmap((QPixmap::fromImage(l_thumb).scaled(ui->l_thumb_label->width(),ui->l_thumb_label->height(),Qt::KeepAspectRatio)));


        status_label->setText("Sampling Done!");
        ui->stop_capture_btn->setHidden(true);
        ui->start_capture->setHidden(false);
    }

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

}

void MainWindow::on_start_capture_clicked()
{

    if(ui->tabWidget->currentIndex() == E_TAB_TYPE_CAPTURE)
    {
        qDebug() << "Start Capture";
        on_start_tab_capture();
    }else if(ui->tabWidget->currentIndex() == E_TAB_TYPE_ENROLL)
    {
        qDebug() << "Start Enroll";
        on_start_tab_enroll();
    }


}


void MainWindow::on_start_tab_capture()
{
    reader50->get_flat_finger("GUI_SHOW_MODE_ROLL","FINGER_POSITION_RIGHT_INDEX");

    ui->start_capture->setHidden(true);
    ui->stop_capture_btn->setHidden(false);
}

void MainWindow::on_start_tab_enroll()
{
    int sampling_type = E_SAMPLING_TYPE_ERROR;

    if(ui->comboBox_2->currentIndex() == 1)
    {
        sampling_type = E_SAMPLING_TYPE_FLAT_ANY_FINGER;
    }
    else if(ui->comboBox_2->currentIndex() == 0)
    {
        if(ui->flat_check->isChecked()) sampling_type = E_SAMPLING_TYPE_FLAT_442;
        else if(ui->roll_check->isChecked()) sampling_type = E_SAMPLING_TYPE_FLAT_442_ROLL;
    }


    switch (sampling_type) {
    case E_SAMPLING_TYPE_FLAT_442:
        status_label->setText("Please put right 4 fingers on the sensor and keep your fingers steady.");

        reader50->get_flat_finger("GUI_SHOW_MODE_FLAT","FINGER_POSITION_RIGHT_FOUR");
        reader50->get_flat_finger("GUI_SHOW_MODE_FLAT","FINGER_POSITION_LEFT_FOUR");
        reader50->get_flat_finger("GUI_SHOW_MODE_FLAT","FINGER_POSITION_BOTH_THUMBS");

        break;
    case E_SAMPLING_TYPE_FLAT_442_ROLL:
        status_label->setText("Please put left 4 fingers on the sensor and keep your fingers steady. (ROLL)");

        reader50->get_flat_finger("GUI_SHOW_MODE_ROLL","FINGER_POSITION_RIGHT_FOUR");
        reader50->get_flat_finger("GUI_SHOW_MODE_ROLL","FINGER_POSITION_LEFT_FOUR");
        reader50->get_flat_finger("GUI_SHOW_MODE_ROLL","FINGER_POSITION_BOTH_THUMBS");

        break;
    case E_SAMPLING_TYPE_FLAT_ANY_FINGER:
        reader50->sampling_finger(GUI_SHOW_MODE_FLAT,FINGER_POSITION_RIGHT_THUMB);
        break;
    }

    ui->start_capture->setHidden(true);
    ui->stop_capture_btn->setHidden(false);


}

void MainWindow::on_AutoCaptureModeButton_clicked()
{

}


void MainWindow::on_stop_capture_btn_clicked()
{
    ui->stop_capture_btn->setHidden(true);
    ui->start_capture->setHidden(false);

    reader50->reset();

}

