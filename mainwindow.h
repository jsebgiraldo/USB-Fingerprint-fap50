#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IMD/FAP20/fap20reader.h"
#include "IMD/FAP50/Fap50reader.h"
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
    void onImageReady(Fingerprint Finger);

private:
    void deviceConnected_action();

private slots:
    void on_pushButton_clicked();

    void on_ConnectButton_clicked();

    void on_DisconnectButton_clicked();

    void on_CaptureLiveModeButton_clicked();

    void on_start_capture_clicked();

private:
    Ui::MainWindow *ui;

    Fap20Reader *reader20 = nullptr;
    Fap50Reader *reader50 = nullptr;
};
#endif // MAINWINDOW_H
