#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->DisconnectButton->setHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::deviceConnected_action()
{
    ui->radio_status_devices->setChecked(true);
    ui->DisconnectButton->setHidden(false);
    ui->ConnectButton->setHidden(true);
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_ConnectButton_clicked()
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

    deviceConnected_action();
}


void MainWindow::on_DisconnectButton_clicked()
{
    ui->radio_status_devices->setChecked(false);
    ui->DisconnectButton->setHidden(true);
    ui->ConnectButton->setHidden(false);
}


void MainWindow::on_CaptureLiveModeButton_clicked()
{

}

