#ifndef CUSTOMERPAGE_H
#define CUSTOMERPAGE_H

#include "Header.h"

namespace Ui {
class CustomerPage;
}

class CustomerPage : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerPage(QWidget *parent = nullptr);
    ~CustomerPage();

private slots:
    void on_CitySelect_currentIndexChanged(const QString &arg1);


    void on_returnButton_clicked();

private:
    Ui::CustomerPage *ui;
//    QSqlTableModel *sqlModel;
    QSqlQueryModel *sqlModel;

};

#endif // CUSTOMERPAGE_H
