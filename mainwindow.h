#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    void deviceConnected_action();

private slots:
    void on_pushButton_clicked();

    void on_ConnectButton_clicked();

    void on_DisconnectButton_clicked();

    void on_CaptureLiveModeButton_clicked();

private:
    Ui::MainWindow *ui;

    Fap50Reader *reader50 = nullptr;
};
#endif // MAINWINDOW_H
