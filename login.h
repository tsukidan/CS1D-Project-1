#ifndef LOGIN_H
#define LOGIN_H

#include "Header.h"
#include "administrator.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:

    /*************************************************************************
     * void Login::on_pushButton_clicked()
     * ----------------------------------------------------------------------
     * This function will read the text from the username and password line edits
     * and check if they are equal to both the administrator's username and
     * password. If both the correct username and password are entered, then
     * the administrator page will open; otherwise an error message will appear
     * telling the user that the username/password is incorrect.
     ************************************************************************/
    void on_pushButton_clicked();

    /*************************************************************************
     * void Login::on_returnButton_clicked()
     * ----------------------------------------------------------------------
     * When clicking the button labeled return, the program will exit the current
     * page that is being viewed, and take the user to the previous page they
     * were on.
     ************************************************************************/
    void on_returnButton_clicked();

private:
    Ui::Login *ui;
    Administrator *adminUI;
};

#endif // LOGIN_H
