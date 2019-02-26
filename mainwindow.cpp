#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qApp->addLibraryPath("C:/Users/Lichuza/Documents/build-test2-Desktop_Qt_5_12_1_MSVC2017_32bit-Release/release");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    QString way1 = ui->textEdit1->toPlainText();
    QString way2 = ui->textEdit5->toPlainText();
    QString way3 = ui->textEdit2->toPlainText();

    std::string way1_s = way1.toLocal8Bit().constData();
    QByteArray ba = way2.toLocal8Bit();
    const char *way2_c = ba.data();
    std::string way3_s = way3.toLocal8Bit().constData();

    graphics packing(way1_s, way2_c, way3_s);
    packing.checkVersion();
    packing.readBMP();
    packing.packingDW();
    packing.writeBMP();

    QPixmap pix1(way2);
    pix1 = pix1.scaled(pix1.size()*0.5,Qt::KeepAspectRatio);
    ui->label4->setPixmap(pix1);

    QPixmap pix2(way3);
    ui->label5->setPixmap(pix2);
    i1=packing.getI();
    key=packing.getkey();


}

void MainWindow::on_pushButton_2_clicked()
{

   QString way1 = ui->textEdit6->toPlainText();
   QString way2 = ui->textEdit7->toPlainText();

   std::string way1_s = way1.toLocal8Bit().constData();
   std::string way2_s = way2.toLocal8Bit().constData();

   graphics extraction(way1_s, way2_s);
   extraction.checkVersion();
   extraction.readBMP();
   extraction.extractionDW(key, i1);

   double mse=extraction.assessmentMSE("C:/qr.bmp", "E:/outqr.bmp");
   double snr = extraction.assessmentSNR("C:/114.bmp", "E:/21.bmp");
   double nmse = extraction.assessmentNMSE("C:/qr.bmp", "E:/outqr.bmp");

   QPixmap pix3(way2);
   ui->label6->setPixmap(pix3);

   QString mse_s = QString::number(mse);
   QString snr_s = QString::number(snr);
   QString nmse_s = QString::number(nmse);

   ui->textEdit3->insertPlainText(mse_s +"\n");
   ui->textEdit4->insertPlainText(snr_s +"\n");


}
