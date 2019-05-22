#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<qdebug.h>
#include <QMainWindow>
#include<QTimer>
#include<QMessageBox>
#include<QVector>
#include<fstream>
#include<iostream>

using namespace std;
struct wifi
{
    string SSID;
    uint centerFrq;
    float SignalStrength;
    uint bandwidth;
};
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
    QVector<wifi> scanlist;
    void do_scan();
    Ui::MainWindow *ui;
    QTimer *timer1;
    uint8_t channelCounter[13]={0};
private slots:
    void Timer1_tricker();
};

#endif // MAINWINDOW_H
