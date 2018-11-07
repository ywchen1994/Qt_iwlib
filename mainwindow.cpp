#include "mainwindow.h"
#include "ui_mainwindow.h"

static char *NetCardID = strdup("wlan0");

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
    header<<"Frequency"<<"Numbers";
    ui->m_ScanTable->setHorizontalHeaderLabels(header);

    for(uint_fast8_t i=0;i<13;i++)
    {
        ui->m_ScanTable->setItem(i,0,new QTableWidgetItem(QString::number(2.412+(double)i*0.005)));
         ui->m_ScanTable->setItem(i,1,new QTableWidgetItem(QString::number(0)));
    }
    /***********************************************************/
    sock=iw_sockets_open();
    if (iw_get_range_info(sock, NetCardID, &range) < 0) {
        QMessageBox::critical(this,tr("Error"),tr("iw_get_range_info. Aborting"));
    }
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
    WifiScanData.resize(0);
    /*********************************/
    do_scan();
    for(uint_fast8_t i=0;i<13;i++)
        if(channelCounter[i]!=0)
            ui->m_ScanTable->setItem(i,1,new QTableWidgetItem(QString::number(channelCounter[i])));

}
void MainWindow::do_scan()
{
    wireless_scan_head head;
    wireless_scan *result;

    if (iw_scan(sock,NetCardID,range.we_version_compiled, &head) < 0) {
        printf("Error during iw_scan. Aborting.\n");
        return;
    }


    result = head.result;
    while (result != nullptr) {
        string ESSID=result->b.essid;
        /************************************/
        double freq=0;
        if (result->b.has_freq) {
            freq=result->b.freq/pow(10,9);
            int index=(freq-2.412)/0.005;
            channelCounter[index]++;
        }
        /************************************/
        int level=0;
        if (result->has_stats) {
            level=result->stats.qual.level- 0x100;
        }
        WifiScanData.push_back(WifiInfoPack(ESSID,freq,level));
        result = result->next;
    }

    result = head.result;
    /* free results */
    while (result && result->next) {
        wireless_scan *tmp;
        tmp = result->next;
        free (result);
        result = tmp;
    }
    if (result) {
        free(result);
    }
}
