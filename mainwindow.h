#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IMD/FAP20/fap20reader.h"
#include "IMD/FAP50/Fap50reader.h"
#include "qlabel.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void onImageReadyFap20(Fingerprint Finger);
    void onImageReadyFap50(ImageStatus image);

    void onWrongHandFap50();
    void onSamplingDoneFap50();

private:
    void deviceConnected_action();

    void custom_status_bar_style();

private slots:
    void on_pushButton_clicked();

    void on_ConnectButton_clicked();

    void on_DisconnectButton_clicked();

    void on_CaptureLiveModeButton_clicked();

    void on_start_capture_clicked();

    void on_AutoCaptureModeButton_clicked();

public:
    Ui::MainWindow *ui;

    QLabel *status_label = new QLabel();

    Fap20Reader *reader20 = nullptr;
    Fap50Reader *reader50 = nullptr;
};
#endif // MAINWINDOW_H
