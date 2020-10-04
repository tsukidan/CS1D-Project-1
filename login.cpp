#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

/******************************
 *** CONSTRUCTOR/DESTRUCTOR ***
 ******************************/
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

/*************************************************************************
 * void Login::on_pushButton_clicked()
 * ----------------------------------------------------------------------
 * This function will read the text from the username and password line edits
 * and check if they are equal to both the administrator's username and
 * password. If both the correct username and password are entered, then
 * the administrator page will open; otherwise an error message will appear
 * telling the user that the username/password is incorrect.
 ************************************************************************/
void Login::on_pushButton_clicked()
{
    QString username = ui -> UsernameLineEdit -> text(); //IN & PROC - Check if text is the same as the username
    QString password = ui -> PasswordLineEdit -> text(); //IN & PROC - Check if text is the same as the password

    /*************************************************************************
     * PROCESSING - Check if correct username and password are entered
     *              Else show an error message.
     ************************************************************************/
    if(username == "a" && password == "a")
    {//begin if else

        adminUI = new Administrator(this);
        adminUI -> setWindowTitle("Administrator Page");

        //Hide Login Page
        hide();

        adminUI -> show();
    }
    //Display error message
    else
    {
        QMessageBox::information(this, "", "Username and/or password is incorrect!");

    }//end if else
}

/*************************************************************************
 * void Login::on_returnButton_clicked()
 * ----------------------------------------------------------------------
 * When clicking the button labeled return, the program will exit the current
 * page that is being viewed, and take the user to the previous page they
 * were on.
 ************************************************************************/
void Login::on_returnButton_clicked()
{
    MainWindow *mainWindow;
    mainWindow = new MainWindow(this);
    hide();
    mainWindow -> show();
}
