#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer1=new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(Timer1_tricker()));
    timer1->start(1000);
    /************************************************************/
    ui->m_ScanTable->setRowCount(13);
    ui->m_ScanTable->setColumnCount(2);

    QStringList header;
    header<<"Frequency(GHz)"<<"Numbers";
    ui->m_ScanTable->setHorizontalHeaderLabels(header);

    for(uint_fast8_t i=0;i<13;i++)
    {
        ui->m_ScanTable->setItem(i,0,new QTableWidgetItem(QString::number(2.412+(double)i*0.005)));
        ui->m_ScanTable->setItem(i,1,new QTableWidgetItem(QString::number(0)));
    }
    /***********************************************************/

    ui->m_ScanTableID->setRowCount(15);
    ui->m_ScanTableID->setColumnCount(4);
    header.clear();
    header<<"ID"<<"Frequency(MHz)"<<"Amptitude(dB)"<<"BandWidth(MHz)";
    ui->m_ScanTableID->setHorizontalHeaderLabels(header);

    /***********************************************************/


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Timer1_tricker()
{
    /*********************************/
    for(uint_fast8_t i=0;i<13;i++)
        channelCounter[i]=0;
    scanlist.resize(0);
    /*********************************/
    do_scan();
    for(uint_fast8_t i=0;i<13;i++)
        if(channelCounter[i]!=0)
            ui->m_ScanTable->setItem(i,1,new QTableWidgetItem(QString::number(channelCounter[i])));
    ui->m_ScanTableID->setRowCount(scanlist.size());
    for(uint_fast8_t i=0;i<scanlist.size();i++)
    {
        ui->m_ScanTableID->setItem(i,0,new QTableWidgetItem(QString::fromStdString((scanlist[i].SSID))));
        ui->m_ScanTableID->setItem(i,1,new QTableWidgetItem(QString::number(scanlist[i].centerFrq)));
        ui->m_ScanTableID->setItem(i,2,new QTableWidgetItem(QString::number(scanlist[i].SignalStrength)));
        ui->m_ScanTableID->setItem(i,3,new QTableWidgetItem(QString::number(scanlist[i].bandwidth)));
    }
}

void MainWindow::do_scan()
{
  system("sudo rm scan.txt ");
  system("sudo iw dev wlan0 scan>>scan.txt");
  fstream fp;
      fp.open("scan.txt",ios::in);
      char linebuffer[1000];

      wifi  wi;

      while (fp.getline(linebuffer, sizeof(linebuffer), '\n'))
      {
          QString qstr(linebuffer);

          if (qstr.indexOf("freq: ")>0)
          {
              QString frq = qstr.split(" ").at(1);
              wi.centerFrq = frq.toInt();
              channelCounter[(wi.centerFrq-2412)/5]++;
          }

          if (qstr.indexOf("signal: ")>0)
          {
              QString signal = qstr.split(" ").at(1);
              wi.SignalStrength = signal.toFloat();
          }
          if (qstr.indexOf("SSID: ")>0)
          {
              QString SSID = qstr.split(" ").at(1);
              wi.SSID = SSID.toStdString();
          }
          if (qstr.indexOf("STA channel width: ")>0)
          {
              QString bandwitdth = qstr.split(": ").at(1);
              if (bandwitdth == "any")
              {
                  wi.bandwidth = 40;
              }
              else
              {
                  wi.bandwidth = bandwitdth.split(" ").at(0).toInt();
              }
              scanlist.push_back(wi);
          }

      }
      fp.close();
}
