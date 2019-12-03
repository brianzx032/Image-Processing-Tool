#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "cv2ImageProcessing.h"
using namespace cv;
using namespace std;
//cv2ImageProcessing cv2ip;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   imread("ref1.jpeg");
//    CvImage img_orig=cv2ip.ImRead("ref1.jpeg");
}
