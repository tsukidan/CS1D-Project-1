#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
