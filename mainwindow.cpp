#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

                /******************************
                 **** SPECIFIC TO HOME TAB ****
                 ******************************/

    /************************************************************
    * PROCESSING - Assign an image into new QPixmap variable
    ************************************************************/
    QPixmap homePageBackground (":/new/Images/european-cities.jpg");

    /************************************************************
    * PROCESSING - Set the label to hold the image specified
    ************************************************************/
    int ImageLabelW =  ui -> ImageLabel -> width ();
    int ImageLabelH =  ui -> ImageLabel -> height();
    ui -> ImageLabel -> setPixmap(homePageBackground.scaled
                      (ImageLabelW, ImageLabelH,
                       Qt::KeepAspectRatio));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    customerUI = new CustomerPage(this);

    //Hide MainWindow
    hide();

    customerUI -> show();
}

void MainWindow::on_AdminPushButton_clicked()
{
    loginUI = new Login(this);

    //Hide MainWindow
    hide();

    loginUI -> show();
}
