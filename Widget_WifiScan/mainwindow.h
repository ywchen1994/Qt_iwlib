#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<qdebug.h>
#include <QMainWindow>
#include <iwlib.h>
#include<QTimer>
#include<QMessageBox>
#include<QVector>
#include<wifiinfopack.h>
#include<iostream>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QVector<WifiInfoPack> WifiScanData;
    void do_scan();
    Ui::MainWindow *ui;
    QTimer *timer1;
    int sock;
    iwrange range;
    uint8_t channelCounter[13]={0};
private slots:
    void Timer1_tricker();
};

#endif // MAINWINDOW_H
