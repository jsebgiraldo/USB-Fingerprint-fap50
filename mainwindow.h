#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IMD/FAP20/fap20reader.h"
#include "IMD/FAP50/Fap50reader.h"
#include "qlabel.h"
#include <QMainWindow>

enum E_SAMPLING_TYPE {
    E_SAMPLING_TYPE_ERROR,
    E_SAMPLING_TYPE_FLAT_442,
    E_SAMPLING_TYPE_FLAT_442_ROLL,
    E_SAMPLING_TYPE_FLAT_ANY_FINGER,
    E_SAMPLING_TYPE_FLAT,
    E_SAMPLING_TYPE_ROLL,
    E_SAMPLING_TYPE_CAPTURE,
};

enum E_TAB_TYPE {
    E_TAB_TYPE_CAPTURE,
    E_TAB_TYPE_ENROLL,
};

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
    void onSamplingDoneFap50(ImageProperty res);
    void onCancelSamplingFap50();

private:
    void deviceConnected_action();

    void custom_status_bar_style();

private slots:
    void on_pushButton_clicked();

    void on_ConnectButton_clicked();

    void on_DisconnectButton_clicked();

    void on_CaptureLiveModeButton_clicked();

    void extracted(int &sampling_type);


    void on_start_tab_capture();
    void on_start_tab_enroll();

    void on_start_capture_clicked();

    void on_AutoCaptureModeButton_clicked();

    void on_stop_capture_btn_clicked();

public:
    Ui::MainWindow *ui;

    QLabel *status_label = new QLabel();

    Fap20Reader *reader20 = nullptr;
    Fap50Reader *reader50 = nullptr;
};
#endif // MAINWINDOW_H
