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

    void on_databaseView_activated(const QModelIndex &index);

    void on_delete_Food_clicked();

    void on_delete_City_clicked();

private:
    Ui::Administrator *ui;
    QSqlQueryModel *sqlModel;
    QString queryVal;
};

#endif // ADMINISTRATOR_H
