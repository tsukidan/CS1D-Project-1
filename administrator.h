#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "Header.h"
#include "sqldatabase.h"

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

    void on_DeleteFoodPushButton_clicked();

    void on_databaseView_pressed(const QModelIndex &index);

    void on_AddFoodPushButton_clicked();

    void on_UpdateFoodPushButton_clicked();

    void on_AddFromFilePushButton_clicked();

private:
    Ui::Administrator *ui;
    QSqlQueryModel *sqlModel;
    QString queryVal;

    void resetDatabaseView(QAbstractItemModel* model);
    QSqlRelationalTableModel* rebuildQuery();
};


#endif // ADMINISTRATOR_H
