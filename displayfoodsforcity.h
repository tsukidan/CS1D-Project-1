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
    /*!
     * \brief DisplayFoodsForCity constructor
     */
    explicit DisplayFoodsForCity(QWidget *parent, QSqlQuery query);
    /*!
     * \brief DisplayFoodsForCity destructor
     */
    ~DisplayFoodsForCity();

private:
    Ui::DisplayFoodsForCity *ui;
    QSqlQueryModel *model;
    QString selectedCity;

};

#endif // DISPLAYFOODSFORCITY_H
