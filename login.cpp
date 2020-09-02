#include "login.h"
#include "ui_login.h"


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QString username = ui -> UsernameLineEdit -> text();
    QString password = ui -> PasswordLineEdit -> text();

    if(username == "admin" && password == "1234")
    {
        adminUI = new Administrator(this);
        adminUI -> setWindowTitle("Administrator Page");

        //Hide Login Page
        hide();

        adminUI -> show();
    }
    else
    {
        QMessageBox::information(this, "", "Username and/or password is incorrect!");
    }
}
