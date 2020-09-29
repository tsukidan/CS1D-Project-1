#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Header.h"

namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = nullptr);
    ~Administrator();

private slots:
    void on_cities_Button_clicked();

    void on_food_Button_clicked();

    void on_distances_Button_clicked();

    void on_returnFromAdminUI_clicked();

private:
    Ui::Administrator *ui;
};

#endif // ADMINISTRATOR_H
