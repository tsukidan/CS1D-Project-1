#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customerpage.h"
#include "login.h"
#include "Header.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// on_pushButton_clicked
    ///
    /// Upon clicking on the customer page button, the main window will become
    /// hidden and the prospective travler page will open.
    /// @return void
    void on_pushButton_clicked();

    /// on_AdminPushButton_clicked
    ///
    /// Upon clicking on the administrator page button, the main window will become
    /// hidden and the administrator page will open.
    /// @return void
    void on_AdminPushButton_clicked();

private:
    Ui::MainWindow *ui;
    CustomerPage   *customerUI;
    Login          *loginUI;
};
#endif // MAINWINDOW_H
