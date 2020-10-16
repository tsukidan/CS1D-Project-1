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
    //PROCESSING - Setting Main Window Title
    this->setWindowTitle("European Vacation Planner");

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

/*************************************************************************
 * void MainWindow::on_pushButton_clicked()
 * ----------------------------------------------------------------------
 * Upon clicking on the customer page button, the main window will become
 * hidden and the prospective travler page will open.
 ************************************************************************/
void MainWindow::on_pushButton_clicked()
{
    customerUI = new CustomerPage(this);

    //Hide MainWindow
    hide();

    customerUI->setWindowTitle("Travel Options");
    customerUI -> show();
}

/*************************************************************************
 * void MainWindow::on_AdminPushButton_clicked()
 * ----------------------------------------------------------------------
 * Upon clicking on the administrator page button, the main window will become
 * hidden and the administrator page will open.
 ************************************************************************/
void MainWindow::on_AdminPushButton_clicked()
{
    loginUI = new Login(this);

    //Hide MainWindow
    hide();

    loginUI -> show();
}

