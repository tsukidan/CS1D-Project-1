#ifndef DISPLAYFOODSFORCITY_H
#define DISPLAYFOODSFORCITY_H

#include "Header.h"

namespace Ui {
class DisplayFoodsForCity;
}

class DisplayFoodsForCity : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayFoodsForCity(QWidget *parent, QSqlQuery query);
    ~DisplayFoodsForCity();

private:
    Ui::DisplayFoodsForCity *ui;
    QSqlQueryModel *model;

};

#endif // DISPLAYFOODSFORCITY_H
